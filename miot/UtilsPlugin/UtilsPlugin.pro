QT -= gui

TEMPLATE = lib
DEFINES += UTILSPLUGIN_LIBRARY

CONFIG += c++11

VERSION += 0.1.0

LIBS += -lacpi

QMAKE_CXXFLAGS += -Wno-unknown-pragmas \
                  -Wno-unused-result \
                  -Wno-unused-parameter

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    utilsplugin.cpp

HEADERS += \
    UtilsPlugin_global.h \
    utilsinterface.h \
    utilsplugin.h

INCLUDEPATH += $$PWD/../PluginManager \
    $$PWD/../PluginInterface


# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    utilsplugin.json
