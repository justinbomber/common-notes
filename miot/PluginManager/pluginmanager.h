#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "pluginspec.h"
#include "../PluginInterface/plugininterface.h"

#include <QObject>
#include <QPluginLoader>
#include <QVariant>
#include "pluginmanagerinterface.h"

class PluginSpec;

class PluginManager : public QObject, public PluginManagerInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.foyatech.miot.PluginManager" FILE "pluginmanager.json")
    Q_INTERFACES(PluginInterface PluginManagerInterface)
  private:
    static PluginManager *pm_instance;

    // Plugin Spec meta-data
    PluginSpec *pluginSpec;

    // get Meta-Data of plugin from json file
    void scanMetaData(const QString &filepath);

    // remove Meta-Data of pluginspec object dynamically
    // when plugin file was removed from filepath
    // 目前不提供使用（因程式執行中不允許動態移除plugin）
    // ToDo: 相依的plugin未處理
    void removeMetaData();
  public:
    PluginManager();
    ~PluginManager();

    /**
     * @brief instance return a static PluginManager Pointer
     * @return *PluginManager
     */
    static PluginManager *instance() {
        if(pm_instance == nullptr)
            pm_instance = new PluginManager();
        return pm_instance;
    }

    /**
     * @brief loadAllPlugins 依據指定路徑與plugin名稱清單載入所需使用plugins
     * @param pluginspath plugins存放路徑
     * @param pluginsNameList 使用plugins名稱清單(JSON format)
     *        {"plugins" : ["plugin_name_a","plugin_name_b"]}
     */
    void loadAllPlugins(const QString& pluginspath, const QString& pluginsNameList, QString& errMsg) override;

    /**
     * @brief loadAllPlugins依據指定檔案路徑載入plugins
     * @param pluginspath plugins檔案路徑
     */
    void loadAllPlugins(const QString& pluginspath) override;

    /**
     * @brief loadAllPlugins load all plugins in plugins directory
     */
    void loadAllPlugins() override;

    QList<QVariant> getAllPluginNames() const override;

    // get plugin object with plugin name
    QPluginLoader* getPlugin(const QString &name) override;

    bool pluginValidation(const QString &filepath) override;

    // loading s specified plugin
    void loadPlugin(const QString &filepath) override;

    // unload all plugins
    void unloadAllPlugins() override;

    // unload a specified plugin
    void unloadPlugin(const QString &filepath) override;

    // get all pluginloader objects
    QList<QPluginLoader *> getAllPlugins() override;

    // get name of specified plugin
    QVariant getPluginName(QPluginLoader *loader) override;

    QObject *getPluginObject(QString &pluginName) const override;

};

#endif // PLUGINMANAGER_H
