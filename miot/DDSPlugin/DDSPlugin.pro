QT -= gui

TEMPLATE = lib
DEFINES += DDSPLUGIN_LIBRARY \
    RTI_UNIX \
    RTI_LINUX \
    RTI_64BIT
#    RTI_STATIC

CONFIG += c++11
#          staticlib

VERSION += 0.1.0

QMAKE_CXXFLAGS += -m64 -Wall -std=c++11

QMAKE_LINK += -m64

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ddsplugin.cpp

HEADERS += \
    DDSPlugin_global.h \
    ddsinterface.h \
    ddsplugin.h

INCLUDEPATH += \
    $$PWD/../PluginInterface \
    $$PWD/../../../../rti_connext_dds-6.1.2/include \
    $$PWD/../../../../rti_connext_dds-6.1.2/include/ndds \
    $$PWD/../../../../rti_connext_dds-6.1.2/include/ndds/hpp

QMAKE_LIBDIR += $$PWD/../../../../rti_connext_dds-6.1.2/lib/x64Linux4gcc7.3.0

LIBS += -L$$PWD/../../../../rti_connext_dds-6.1.2/lib/x64Linux4gcc7.3.0 -lnddscpp2 -lnddsc -lnddscore -ldl -lm -lpthread -rdynamic

# Default rules for deployment.
unix {
    target.path = $$PWD/../../../../plugins
}
!isEmpty(target.path): INSTALLS += target

#LIBS += -L$$QMAKE_LIBDIR -lnddsc -lnddscore -lnddscpp -ldl -lm -lpthread -lrt -no-pie -rdynamic

DISTFILES += \
    ddsplugin.json
