QT -= gui
#QT += statemachine

TARGET = pluginmanager
TEMPLATE = lib
DEFINES += PLUGINMANAGER_LIBRARY \
           QT_MESSAGELOGCONTEXT

#CONFIG += staticlib

CONFIG += c++11

VERSION += 0.1.0


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    pluginmanager.cpp \
    pluginspec.cpp

HEADERS += \
    pluginmanager.h \
    pluginmanagerinterface.h \
    pluginspec.h


INCLUDEPATH += \
     $$PWD/../PluginInterface

# Default rules for deployment.
unix {
    target.path = $$PWD/../../../../plugins
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    pluginmanager.json
