#ifndef PLUGINSPEC_H
#define PLUGINSPEC_H

#include <QString>
#include <QPluginLoader>
#include <QDebug>

/**
 * @brief The PluginSpec class 提供Plugin Meta Data資料檢查與儲存
 */

class PluginSpec {
  public:
    /**
     * @brief dependencyCheck 檢查Plugin相依性
     * @param filepath Plugin存放路徑/檔名
     * @return bool 檢查結果
     */
    bool dependencyCheck(const QString &filepath);

    /**
     * @brief compatVersionCheck 檢查Plugin相容性
     * @param filepath Plugin存放路徑/檔名
     * @return bool 檢查結果
     */
    bool compatVersionCheck(const QString &filepath);

    bool versionValidation(const QStringList &version);

    QStringList fixVersion(const QString &type, const QString &filepath);

    bool categoryCheck(const QString &filepath, const QString &targetCategory);

    void setPluginState(const QString &filepath, const int state);

    /**
     * @brief p_names: plugin file path - plugin name
     */
    QMap<QString, QVariant> p_names; // plugin file path - plugin name

    /**
     * @brief p_versions: plugin file path - version of plguin
     */
    QHash<QString, QVariant> p_versions; // plugin file path - version of plguin

    /**
     * @brief p_compat_versions:  plugin file path - backward compatible with all versions of the plugin down to compatVersion (inclusive)
     */
    QHash<QString, QVariant> p_compat_versions; // plugin file path -  backward compatible with all versions of the plugin down to compatVersion (inclusive)

    /**
     * @brief p_categories: plugin file path - category of plguin
     */
    QHash<QString, QVariant> p_categories; // plugin file path - category of plguin

    /**
     * @brief p_arguments: plugin file path - arguments of plugin
     */
    QMap<QString, QVariantList> p_arguments; // plugin file path - arguments of plugin

    /**
     * @brief p_dependencies: plugin file path - dependencies of plugin
     */
    QMap<QString, QVariantList> p_dependencies; // plugin file path - dependencies of plugin

    /**
     * @brief p_loaders: plugin file path - QPluginLoader instances (pointer)
     */
    QMap<QString, QPluginLoader *> p_loaders; // plugin file path - QPluginLoader instances (pointer)

    /**
     * @brief The PluginStat enum: state-value of plugin
     */
    enum PluginStat {
        Read, Resolved, Loaded, Initialized, Running, Unloaded, IException, UException, Remove
    };

    /**
     * @brief p_states: plugin file path - current state
     */
    QHash<QString, QVariant> p_states; // plugin file path - state of plguin

    PluginSpec();
    ~PluginSpec() {
        qInfo() << "IP300:Destrcuting PluginSpec.";
    };
};

#endif // PLUGINSPEC_H
