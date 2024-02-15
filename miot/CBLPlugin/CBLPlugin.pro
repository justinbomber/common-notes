QT -= gui

TEMPLATE = lib
DEFINES += CBLPLUGIN_LIBRARY \
           QT_MESSAGELOGCONTEXT

CONFIG += c++11

VERSION += 0.1.0

#QMAKE_CFLAGS += -Wno-unknown-pragmas
QMAKE_CXXFLAGS += -Wno-unknown-pragmas \
                  -Wno-unused-result \
                  -Wno-unused-parameter \
                  -Wno-elaborated-enum-base

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cblplugin.cpp

HEADERS += \
    CBLPlugin_global.h \
    cblinterface.h \
    cblplugin.h \
    fhir_resources.h

#INCLUDEPATH += \
#    $$PWD/../PluginInterface \
#    $$PWD/../../../../couchbase-lite-C/build/output/include

#DEPENDPATH += $$PWD/../../../../couchbase-lite-C/build/output/lib/x86_64-linux-gnu
#LIBS += -L$$PWD/../../../../couchbase-lite-C/build/output/lib/x86_64-linux-gnu -lcblite

# For new version 3.1.0 libcblite
INCLUDEPATH += \
    $$PWD/../PluginInterface \
    /usr/include/cbl \
    /usr/include/cbl++

DEPENDPATH += /usr/lib/x86_64-linux-gnu
LIBS += -L/usr/lib/x86_64-linux-gnu -lcblite

# Default rules for deployment.
unix {
    target.path = $$PWD/../../../../plugins
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    cblplugin.json
