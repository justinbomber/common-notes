#ifndef PLUGINMANAGERINTERFACE_H
#define PLUGINMANAGERINTERFACE_H
#include <QtCore>
#include "../PluginInterface/plugininterface.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>

#define PluginManagerInterface_iid "com.foyatech.miot.PluginManagerInterface"

class PluginManagerInterface : public PluginInterface {

  public:
    /**
     * @brief loadAllPlugins 依據指定路徑與plugin名稱清單載入所需使用plugins
     * @param pluginspath plugins存放路徑
     * @param pluginsNameList 使用plugins名稱清單(JSON format)
     *        {"plugins" : ["plugin_name_a.so","plugin_name_b.so"]}
     * @param errMsg
     */
    virtual void loadAllPlugins(const QString& pluginspath, const QString& pluginsNameList, QString& errMsg) = 0;

    /**
     * @brief loadAllPlugins  loadAllPlugins load all plugins in pluginspath
     * @param pluginspath Plugins so檔案放置路徑上層（內部強制so檔案放在plugins目錄）
     *        所以,只須帶到plugins上層目錄
     */
    virtual void loadAllPlugins(const QString& pluginspath) = 0;

    /**
     * @brief loadAllPlugins load all plugins in plugins directory
     */
    virtual void loadAllPlugins() = 0;

    /**
     * @brief getAllPluginNames 取得目前載入Plugin名稱
     * @return
     */
    virtual QList<QVariant> getAllPluginNames() const = 0;

    // get plugin object with plugin name
    /**
     * @brief getPlugin get plugin object with plugin name
     * @param name Plugin名稱
     * @return
     */
    virtual QPluginLoader* getPlugin(const QString &name) = 0;

    /**
     * @brief pluginValidation
     * @param filepath
     */
    virtual bool pluginValidation(const QString &filepath) = 0;

    // loading s specified plugin
    /**
     * @brief loadPlugin 載入指定路徑檔名plugin檔案
     * @param filepath plugin所在路徑檔名
     */
    virtual void loadPlugin(const QString &filepath) = 0;

    // unload all plugins
    /**
     * @brief unloadAllPlugins 卸載目前已載入plugin
     */
    virtual void unloadAllPlugins() = 0;

    // unload a specified plugin
    /**
     * @brief unloadPlugin 卸載指定路徑檔名plugin檔案
     * @param filepath plugin所在路徑檔名
     */
    virtual void unloadPlugin(const QString &filepath) = 0;

    // get all pluginloader objects
    /**
     * @brief getAllPlugins 取得目前載入Plugin Loader物件
     * @return QList<QPluginLoader *>
     */
    virtual QList<QPluginLoader *> getAllPlugins() = 0;

    // get name of specified plugin
    /**
     * @brief getPluginName 使用Plugin Loader物件取得plugin名稱
     * @param loader lugin Loader物件
     * @return QVariant
     */
    virtual QVariant getPluginName(QPluginLoader *loader) = 0;

    /**
     * @brief getPluginObject 以plugin名稱取得plugin實際物件
     * @param pluginName plugin名稱
     * @return QObject *
     */
    virtual QObject *getPluginObject(QString &pluginName) const = 0;

    virtual ~PluginManagerInterface() {}
};

Q_DECLARE_INTERFACE(PluginManagerInterface, PluginManagerInterface_iid)

#endif // PLUGINMANAGERINTERFACE_H
