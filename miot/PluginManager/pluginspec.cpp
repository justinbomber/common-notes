#include "pluginspec.h"

PluginSpec::PluginSpec() {

}

bool PluginSpec::dependencyCheck(const QString &filepath) {

    for(const QVariant &item : p_dependencies.value(filepath)) {
        // 取得JSON所紀錄相依plugin資料
        QMap<QString, QVariant> map = item.toMap();
        QVariant name = map.value("Name");
        QVariant version = map.value("Version");
        QString path = p_names.key(name);
        QString errorMsg;
        QMap<QString, QVariant>::iterator it = std::find(p_names.begin(), p_names.end(), name);
        // 檢查plugin 相依的plugin是否存在在清單當中
        if(it == p_names.end()) {
            errorMsg = "Missing dependency: " + name.toString() + " for plugin " + path;
            qDebug() << Q_FUNC_INFO << errorMsg;
            return false;
        }
        // 檢查相依plguin與json所紀錄版本使否一致
        if(p_versions.value(path) != version) {
            errorMsg = "Version mismatched: " + name.toString() +
                       " version " + p_versions.value(p_names.key(name)).toString() +
                       " but " + version.toString() + " required by plugin " + path;
            qDebug() << Q_FUNC_INFO << errorMsg;
            return false;
        }
        // 循環檢查依賴
        if(!dependencyCheck(path)) {
            errorMsg = "Dependency corrupted: " + name.toString() +
                       " for plugin" + path;
            qDebug() << Q_FUNC_INFO << errorMsg;
            return false;
        }
    }
    return true;
}

bool PluginSpec::versionValidation(const QStringList &version) {
    for(int j = 0; j < version.size(); j++) {
        if(version[j].toInt() < 0)
            return false;
    }
    return true;
}

QStringList PluginSpec::fixVersion(const QString &type, const QString &filepath) {
    QChar sep = '.';
    QStringList version;
    if(type == "version") {
        if(p_versions.value(filepath).toString().contains(sep)) {
            if(version.size() < 3) {
                version = (p_versions.value(filepath).toString() + ".0").split(sep); // major & minor only
            } else {
                version = p_versions.value(filepath).toString().split(sep);
            }
        } else
            version = (p_versions.value(filepath).toString() + ".0.0").split(sep); // major only
    } else {
        if(p_compat_versions.value(filepath).toString().contains(sep)) {
            if(version.size() < 3) {
                version = (p_compat_versions.value(filepath).toString() + ".0").split(sep); // major & minor only
            } else {
                version = p_compat_versions.value(filepath).toString().split(sep);
            }
        } else
            version = (p_compat_versions.value(filepath).toString() + ".0.0").split(sep); // major only

    }
    return version;
}

bool PluginSpec::compatVersionCheck(const QString &filepath) {
    QString errorMsg;
    QStringList version = fixVersion("version", filepath);
    if(!versionValidation(version)) {
        errorMsg = "Version format mismatched for plugin " + filepath;
        qDebug() << Q_FUNC_INFO << errorMsg;
        return false;
    }

    QStringList compatVersion = fixVersion("compatversion", filepath);
    if(!versionValidation(compatVersion)) {
        errorMsg = "Version format mismatched for plugin " + filepath;
        qDebug() << Q_FUNC_INFO << errorMsg;
        return false;
    }

    for(int i = 0; i < compatVersion.size(); i++) {
        // major.minor.patch依次取相同位置進行比較
        if(compatVersion[i].toInt() > version[i].toInt()) {
            errorMsg = "Version not compatible for plugin " + filepath;
            qDebug() << Q_FUNC_INFO << errorMsg;
            return false;
        }
    }
    return true;
}

bool PluginSpec::categoryCheck(const QString &filepath, const QString &targetCategory) {
    QString errorMsg;
    if(p_categories.value(filepath) != targetCategory) {
        errorMsg = filepath + "'s Category not belong to " + targetCategory;
        qDebug() << Q_FUNC_INFO << errorMsg;
        return false;
    }
    return true;
}
