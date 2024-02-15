#include "calculateplugin.h"

QString CalculatePlugin::getName() const
{
    return "CalculatePlugin";
}

double CalculatePlugin::getSum(double x, double y) const
{
    return x + y;
}
