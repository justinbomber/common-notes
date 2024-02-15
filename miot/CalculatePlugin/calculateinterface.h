#ifndef CALCULATEINTERFACE_H
#define CALCULATEINTERFACE_H

#include <QtCore>
#include "plugininterface.h"

#define CalculateInterface_iid "com.foyatech.miot.CalculateInterface"

class CalculateInterface : public PluginInterface
{

public:
    virtual QString getName() const = 0;
    virtual double getSum(double x, double y) const = 0;
    virtual ~CalculateInterface(){}
};
Q_DECLARE_INTERFACE(CalculateInterface, CalculateInterface_iid)

#endif // CALCULATEINTERFACE_H
