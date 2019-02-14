QT += core widgets network

CONFIG += c++11
CONFIG += static

TARGET = Tray
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
    src/trayicon.cpp \
    src/activityagent.cpp \
    src/gatewayclient.cpp

HEADERS += \
    src/trayicon.h \
    src/activityagent.h \
    src/gatewayclient.h

RESOURCES += \
    src/bin.qrc

LIBS += -L/home/smena/projects/cpp/libuiohook/.libs -luiohook
