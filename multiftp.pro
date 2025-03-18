QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

include(qtcurl/src/qtcurl.pri)
# CONFIG += link_pkgconfig
# PKGCONFIG += curl-gnutls
LIBS += -lcurl

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/ftpclient.cpp \
    src/ftpmanager.cpp \
    src/ftpmodel.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/ftpclient.h \
    src/ftpmanager.h \
    src/ftpmodel.h \
    src/mainwindow.h

FORMS += \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/multiftp.qrc
