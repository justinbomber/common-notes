#ifndef UTILSPLUGIN_GLOBAL_H
#define UTILSPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UTILSPLUGIN_LIBRARY)
#  define UTILSPLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define UTILSPLUGIN_EXPORT Q_DECL_IMPORT
#endif

#endif // UTILSPLUGIN_GLOBAL_H
