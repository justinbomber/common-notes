#ifndef CALCULATEPLUGIN_H
#define CALCULATEPLUGIN_H

#include "CalculatePlugin_global.h"
#include "calculateinterface.h"

class CALCULATEPLUGIN_EXPORT CalculatePlugin : public QObject, public CalculateInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.foyatech.miot.CalculatePlugin" FILE "calculateplugin.json")
    Q_INTERFACES(PluginInterface CalculateInterface)
public:
    QString getName() const;
    double getSum(double x, double y) const;
    ~CalculatePlugin(){}
};

#endif // CALCULATEPLUGIN_H
