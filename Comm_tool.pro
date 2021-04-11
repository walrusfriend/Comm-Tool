QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += headers/

TEMPLATE = app
SOURCES += \
    src/audiointerface.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settings.cpp \
    src/tcpclient.cpp \
    src/tcpserver.cpp \
    src/volumebar.cpp

HEADERS += \
    headers/mainwindow.h \
    headers/settings.h \
    headers/tcpclient.h \
    headers/tcpserver.h \
    headers/volumebar.h \
    headers/audiointerface.h

FORMS += \
    ui/mainwindow.ui \
    ui/settings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    sources.qrc
