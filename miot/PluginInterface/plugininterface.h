#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QtPlugin>

#include "PluginInterface_global.h"

#define PluginInterface_iid "com.foyatech.miot.PluginInterface"

class PluginInterface
{
public:
    virtual ~PluginInterface(){}
};

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif // PLUGININTERFACE_H
