QT -= gui

TEMPLATE = lib
DEFINES += CALCULATEPLUGIN_LIBRARY \
           QT_MESSAGELOGCONTEXT

CONFIG += c++11

# 須與json檔案中Version內容一致
VERSION += 0.1.0

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calculateplugin.cpp

HEADERS += \
    CalculatePlugin_global.h \
    calculateinterface.h \
    calculateplugin.h


INCLUDEPATH += \
    $$PWD/../PluginInterface

# Default rules for deployment.
unix {
    # target.path = /usr/lib
    target.path = $$PWD/../../../../plugins
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    calculateplugin.json
