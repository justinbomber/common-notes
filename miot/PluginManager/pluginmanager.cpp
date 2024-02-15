#include "pluginmanager.h"
#include "pluginspec.h"
#include <QDir>
#include <QCoreApplication>

PluginManager *PluginManager::pm_instance = nullptr;

PluginManager::PluginManager()
{
    pluginSpec = new PluginSpec();
}

PluginManager::~PluginManager()
{
    if (pluginSpec)
        delete pluginSpec;
}

void PluginManager::loadAllPlugins(const QString &pluginsPath, const QString &pluginsNameList, QString &errMsg)
{
    QDir pluginDir = QDir(pluginsPath);
    pluginDir.cd("plugins");

    // get plugin names from pluginsNameList
    QJsonParseError errorPtr;
    QJsonDocument pluginNames = QJsonDocument::fromJson(pluginsNameList.toUtf8(), &errorPtr);
    if (errorPtr.error != QJsonParseError::NoError)
    {
        errMsg = errorPtr.errorString();
        qCritical() << Q_FUNC_INFO << errorPtr.errorString();
        return;
    }
    if (pluginNames.isNull())
    {
        errMsg = "Loading plugin names configuration error.";
        qCritical() << Q_FUNC_INFO << " loading plugin names configuration error.";
        return;
    }
    QJsonArray names = pluginNames.object().take("plugins").toArray();

    QFileInfoList pluginInfoList = pluginDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    // initialize plugin meta data
    QJsonArray::ConstIterator cit;
    QFileInfoList::Iterator fileInfo;
    for (cit = names.cbegin(); cit != names.cend(); ++cit)
    {
        bool exists = false;
        for (fileInfo = pluginInfoList.begin(); fileInfo != pluginInfoList.end(); ++fileInfo)
        {
            if (QString::compare((pluginsPath + cit->toString()), fileInfo->absoluteFilePath(), Qt::CaseSensitive) == 0)
            {
                exists = true;
                break;
            }
        }
        if (exists)
        {
            scanMetaData(fileInfo->absoluteFilePath());
        }
        else
        {
            errMsg = pluginsPath + cit->toString() + " not found.";
            return;
        }
    }
    for (cit = names.cbegin(); cit != names.cend(); ++cit)
    {
        loadPlugin((pluginsPath + cit->toString()));
    }
}

void PluginManager::loadAllPlugins(const QString &pluginsPath)
{
    QDir pluginDir = QDir(pluginsPath);
    pluginDir.cd("plugins");

    QFileInfoList pluginInfoList = pluginDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    // initialize plugin meta data
    QFileInfoList::Iterator fileInfo;
    for (fileInfo = pluginInfoList.begin(); fileInfo != pluginInfoList.end(); ++fileInfo)
    {
        scanMetaData(fileInfo->absoluteFilePath());
    }

    // load plugins
    for (fileInfo = pluginInfoList.begin(); fileInfo != pluginInfoList.end(); ++fileInfo)
    {
        loadPlugin(fileInfo->absoluteFilePath());
    }
}

void PluginManager::loadAllPlugins()
{
    QDir pluginDir = QDir(qApp->applicationDirPath());
    pluginDir.cd("plugins");

    QFileInfoList pluginInfoList = pluginDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    // initialize plugin meta data
    QFileInfoList::Iterator fileInfo;
    for (fileInfo = pluginInfoList.begin(); fileInfo != pluginInfoList.end(); ++fileInfo)
    {
        scanMetaData(fileInfo->absoluteFilePath());
    }

    // load plugins
    for (fileInfo = pluginInfoList.begin(); fileInfo != pluginInfoList.end(); ++fileInfo)
    {
        loadPlugin(fileInfo->absoluteFilePath());
    }
}

QList<QVariant> PluginManager::getAllPluginNames() const
{
    return pluginSpec->p_names.values();
}

// get Meta-Data of plugin from json file
void PluginManager::scanMetaData(const QString &filepath)
{
    // 檢查是否為公用套件
    if (!QLibrary::isLibrary(filepath))
    {
        qWarning() << "PW301:" << filepath + " is not a library.";
        return;
    }

    QPluginLoader *loader = new QPluginLoader(filepath);

    QJsonObject json = loader->metaData().value("MetaData").toObject();
    // 排除pluginmanager被載入,PluginManager理應由使用程式載入併卸載
    // 不重複載入相同版本plugin
    // 未載入plugin
    if ("PluginManager" != json.value("Name").toVariant().toString() &&
        (!pluginSpec->p_names.contains(filepath) ||
         (pluginSpec->p_names.contains(filepath) && pluginSpec->p_versions.value(filepath) != json.value("Version").toVariant())))
    {
        pluginSpec->p_names.insert(filepath, json.value("Name").toVariant());
        pluginSpec->p_categories.insert(filepath, json.value("Category").toVariant());
        pluginSpec->p_versions.insert(filepath, json.value("Version").toVariant());
        pluginSpec->p_compat_versions.insert(filepath, json.value("CompatVersion").toVariant());
        pluginSpec->p_dependencies.insert(filepath, json.value("Dependencies").toArray().toVariantList());
        pluginSpec->p_arguments.insert(filepath, json.value("Arguments").toArray().toVariantList());
        pluginSpec->p_states.insert(filepath, pluginSpec->Read);
    }
    delete loader;
    loader = nullptr;
}

void PluginManager::removeMetaData()
{
    QDir pluginDir = QDir(qApp->applicationDirPath());
    pluginDir.cd("plugins");

    QFileInfoList pluginInfoList = pluginDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    // initialize plugin meta data
    int removeFlag = 0;
    QFileInfoList::Iterator fileInfo;
    for (QMap<QString, QVariant>::const_iterator it = pluginSpec->p_names.cbegin(), end = pluginSpec->p_names.cend(); it != end; ++it)
    {
        for (fileInfo = pluginInfoList.begin(); fileInfo != pluginInfoList.end(); ++fileInfo)
        {
            if (fileInfo->absoluteFilePath() == it.key())
            {
                removeFlag += 1;
                break;
            }
        }
        if (removeFlag == 0)
        {
            QPluginLoader *loader = pluginSpec->p_loaders.value(it.key());
            if (loader->unload())
            {
                pluginSpec->p_names.remove(it.key());
                pluginSpec->p_categories.remove(it.key());
                pluginSpec->p_versions.remove(it.key());
                pluginSpec->p_compat_versions.remove(it.key());
                pluginSpec->p_dependencies.remove(it.key());
                pluginSpec->p_arguments.remove(it.key());
                pluginSpec->p_states.remove(it.key());
            }
            delete loader;
            loader = nullptr;
            pluginSpec->p_loaders.remove(it.key());
            qInfo() << "PI301:Remove plugin success" << it.key();
        }
        else
        {
            removeFlag = 0;
        }
    }
}

bool PluginManager::pluginValidation(const QString &filepath)
{

    if (!pluginSpec->p_names.contains(filepath))
        return false;

    // 判斷版本相容性
    if (!pluginSpec->compatVersionCheck(filepath))
    {
        qWarning() << "PW302:" << filepath + " not compatible.";
        return false;
    }
    // 判斷Category
    if (!pluginSpec->categoryCheck(filepath, "MIoT"))
    {
        qWarning() << "PW302:" << filepath + " wrong category.";
        return false;
    }
    // 前面檢查完成後state=Resolved
    pluginSpec->p_states.insert(filepath, pluginSpec->Resolved);
    return true;
}

// loading s specified plugin
// 控制plugin載入順序(需要實做嗎？目前版本已實做,理論上所有plugin應該盡量各自獨立不要有相依性存在)
void PluginManager::loadPlugin(const QString &filepath)
{
    if (!pluginSpec->p_loaders.contains(filepath))
    {
        //
        if (!pluginValidation(filepath))
            return;

        // 判斷是否有依賴關係
        if (pluginSpec->dependencyCheck(filepath))
        {
            for (const QVariant &item : pluginSpec->p_dependencies.value(filepath))
            {
                // 取得JSON所紀錄相依plugin資料
                QMap<QString, QVariant> map = item.toMap();
                if (map.value("Type") != "Optional")
                {
                    QString path = pluginSpec->p_names.key(map.value("Name"));
                    loadPlugin(path);
                }
            }
        }

        // 載入plugin
        QPluginLoader *loader = new QPluginLoader(filepath);
        if (loader->load())
        {
            // 判斷是否為相容界面所建置plguin
            PluginInterface *plugin = qobject_cast<PluginInterface *>(loader->instance());
            if (plugin)
            {
                qDebug() << "DP301:insert loader into p_loaders " + filepath;
                pluginSpec->p_loaders.insert(filepath, loader);
                pluginSpec->p_states.insert(filepath, pluginSpec->Loaded);
            }
            else
            {
                qDebug() << "DP301:Plugin Interface not compatible " + loader->errorString();
                pluginSpec->p_states.insert(filepath, pluginSpec->IException);
                if (loader)
                {
                    delete loader;
                    loader = nullptr;
                }
            }
        }
        else
        {
            qCritical() << "FP301:Load plugin " + filepath + " error:" + loader->errorString();
            pluginSpec->p_states.insert(filepath, pluginSpec->IException);
            if (loader)
            {
                delete loader;
                loader = nullptr;
            }
        }
    }
}

// unload all plugins
void PluginManager::unloadAllPlugins()
{
    QMap<QString, QPluginLoader *>::Iterator it;
    for (it = pluginSpec->p_loaders.begin(); it != pluginSpec->p_loaders.end(); ++it)
    {
        unloadPlugin(it.key());
    }
}

// unload a specified plugin
// 控制plugin卸載順序(需要實做嗎？目前版本已實做,理論上所有plugin應該盡量各自獨立不要有相依性存在)
void PluginManager::unloadPlugin(const QString &filepath)
{
    bool unload = true;
    QPluginLoader *loader = pluginSpec->p_loaders.value(filepath);
    if (loader)
    {

        // 判斷是否有主動依賴關係
        if (pluginSpec->dependencyCheck(filepath) && pluginSpec->p_dependencies.value(filepath).toVector().size() > 0)
        {
            for (const QVariant &item : pluginSpec->p_dependencies.value(filepath))
            {
                // 取得JSON所紀錄相依plugin資料
                QMap<QString, QVariant> map = item.toMap();
                if (map.value("Type") != "Optional")
                {
                    QString path = pluginSpec->p_names.key(map.value("Name"));
                    unloadPlugin(path);
                }
            }
        }
        else
        {
            for (const QVariant &item : pluginSpec->p_dependencies.value(filepath))
            {
                // 取得JSON所紀錄相依plugin資料
                QMap<QString, QVariant> map = item.toMap();
                if (map.value("Name") == pluginSpec->p_names.value(filepath))
                {
                    unload = false;
                    break;
                }
            }
        }
        if (unload)
        {
            if (loader->unload())
            {
                pluginSpec->p_loaders.remove(filepath);
                pluginSpec->p_states.remove(filepath);
            }
            else
            {
                qCritical() << "FP302:UnLoad plugin " + filepath + " error:" + loader->errorString();
                pluginSpec->p_states.insert(filepath, pluginSpec->UException);
            }
        }
    }
    if (loader)
    {
        delete loader;
        loader = nullptr;
    }
}

// get all pluginloader objects
QList<QPluginLoader *> PluginManager::getAllPlugins()
{
    return pluginSpec->p_loaders.values();
}

// get name of specified plugin
QVariant PluginManager::getPluginName(QPluginLoader *loader)
{
    if (loader)
        return pluginSpec->p_names.value(pluginSpec->p_loaders.key(loader));
    else
        return "";
}

// get plugin object with plugin name
QPluginLoader *PluginManager::getPlugin(const QString &name)
{
    return pluginSpec->p_loaders.value(pluginSpec->p_names.key(name));
}

QObject *PluginManager::getPluginObject(QString &pluginName) const
{
    QPluginLoader *loader = PluginManager::instance()->getPlugin(pluginName);
    if (loader != nullptr && loader->isLoaded())
    {
        QObject *object = loader->instance();
        return object;
    }
    return nullptr;
}
