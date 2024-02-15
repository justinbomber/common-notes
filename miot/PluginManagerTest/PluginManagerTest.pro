QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += RTI_UNIX \
           RTI_LINUX \
           RTI_64BIT \
           QT_MESSAGELOGCONTEXT

CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG(release, debug|release): DEFINES += QT_NO_INFO_OUTPUT
CONFIG(release, debug|release): DEFINES += QT_NO_WARNING_OUTPUT


CONFIG += c++11

VERSION += 0.1.0


QMAKE_CXXFLAGS += -Wno-unknown-pragmas

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    testpubobject.cpp \
    testsubobject.cpp \
    widget.cpp

HEADERS += \
    testpubobject.h \
    testsubobject.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../PluginManager \
               $$PWD/../PluginInterface \
               $$PWD/../CalculatePlugin \
               $$PWD/../DDSPlugin \
               $$PWD/../../../../rti_connext_dds-6.1.2/include \
               $$PWD/../../../../rti_connext_dds-6.1.2/include/ndds \
               $$PWD/../../../../rti_connext_dds-6.1.2/include/ndds/hpp \
               $$PWD/../CBLPlugin \
               $$PWD/../UtilsPlugin \
              /usr/include/cbl \
              /usr/include/cbl++

DEPENDPATH += $$PWD/../PluginManager \
              $$PWD/../CalculatePlugin \
              $$PWD/../DDSPlugin \
              $$PWD/../../../../rti_connext_dds-6.1.2/lib/x64Linux4gcc7.3.0 \
              $$PWD/../CBLPlugin \
              $$PWD/../UtilsPlugin \
              /usr/lib/x86_64-linux-gnu -lcblite

unix:!macx: LIBS += -L$$OUT_PWD/../PluginManager -lpluginmanager \
                    -L$$OUT_PWD/../CalculatePlugin -lCalculatePlugin \
                    -L$$OUT_PWD/../DDSPlugin -lDDSPlugin \
                    -L$$OUT_PWD/../../../../rti_connext_dds-6.1.2/lib/x64Linux4gcc7.3.0 -lnddscpp2 -lnddsc -lnddscore \
                    -L$$OUT_PWD/../CBLPlugin \
                    -L$$OUT_PWD/../UtilsPlugin \
                    -L /usr/lib/x86_64-linux-gnu -lcblite


DISTFILES += \
    Test.xml \
    config.json
