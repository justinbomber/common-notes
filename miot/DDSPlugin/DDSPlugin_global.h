#ifndef DDSPLUGIN_GLOBAL_H
#define DDSPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DDSPLUGIN_LIBRARY)
#  define DDSPLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define DDSPLUGIN_EXPORT Q_DECL_IMPORT
#endif

#endif // DDSPLUGIN_GLOBAL_H
