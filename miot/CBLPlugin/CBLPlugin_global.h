#ifndef CBLPLUGIN_GLOBAL_H
#define CBLPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CBLPLUGIN_LIBRARY)
#  define CBLPLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define CBLPLUGIN_EXPORT Q_DECL_IMPORT
#endif

#endif // CBLPLUGIN_GLOBAL_H
