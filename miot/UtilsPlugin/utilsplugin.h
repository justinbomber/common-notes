#ifndef UTILSPLUGIN_H
#define UTILSPLUGIN_H

#include "UtilsPlugin_global.h"
#include "utilsinterface.h"

class UTILSPLUGIN_EXPORT UtilsPlugin : public QObject, public UtilsInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.foyatech.miot.UtilsPlugin" FILE "utilsplugin.json")
    Q_INTERFACES(PluginInterface UtilsInterface)
  public:
    UtilsPlugin();
    std::string getUUID() const override;
    powerInfo getPowerInfo() const override;
    powerInfo getPowerInfoProc() const override;
    QStringList getNICNames() const override;
    bool isInterfaceOnline(const std::string &interface, const int protocol, const int socketType) const override;
    QJsonObject loadJsonFile(const std::string &filepath, const std::string &mode) override;
    void loadJsonFile(const std::string &filepath, const std::string &mode, QJsonObject &jobj) override;
    void sqlStringFormat(const std::map<int, std::string> &data, std::string &sqlString) override;
    std::string str_toupper(std::string s) override;
    void mdcValueFormatter(const std::string& dataType, const std::string& code, const float value, const std::string& unit, char* result) override;
};

#endif // UTILSPLUGIN_H
