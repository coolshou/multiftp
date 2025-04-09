QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


unix:!android {
    CONFIG += link_pkgconfig
    PKGCONFIG += libcurl
    #LIBS += -lcurl
}
# TODO: windows curl

include(qtcurl/src/qtcurl.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/comm.cpp \
    src/dirdelegate.cpp \
    src/ftpclient.cpp \
    src/ftpmanager.cpp \
    src/ftpmodel.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/remotelist.cpp

HEADERS += \
    src/comm.h \
    src/dirdelegate.h \
    src/ftpclient.h \
    src/ftpmanager.h \
    src/ftpmodel.h \
    src/mainwindow.h \
    src/remotelist.h \
    src/versions.h

FORMS += \
    src/mainwindow.ui \
    src/remotelist.ui

OTHERS += \
    multiftp.nsi

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/multiftp.qrc

win32 {
    # windows resources
    #    CONFIG += embed_manifest_exe
    # nmake release-windeployqt
    CONFIG += windeployqt

    RC_ICONS=$$PWD/images/multiftp.ico #：指定應該被包含進一個.rc檔案中的圖示，僅適用於Windows

    QMAKE_TARGET_PRODUCT=$${TARGET} #：指定項目目標的產品名稱，僅適用於Windows
    QMAKE_TARGET_DESCRIPTION="multi ftp test" #：指定項目目標的描述資訊，僅適用於Windows
    DIST_DIRECTORY =  $$shell_quote($$shell_path($${PWD}/$${TARGET}_$${QT_ARCH}))
    DIST_FILE = $$shell_quote($$shell_path($$DIST_DIRECTORY/$${TARGET}.exe))
    CONFIG(release, debug|release) {
        release: multiftpbin.commands = \
            $$sprintf($$QMAKE_MKDIR_CMD, $$DIST_DIRECTORY) $$escape_expand(\\n\\t) \
            $$QMAKE_COPY $$shell_quote($$shell_path($${PWD}/Release/$${TARGET}.exe)) $$shell_quote($$shell_path($$DIST_FILE))
    } else {
        debug: multiftpbin.commands = \
            $$sprintf($$QMAKE_MKDIR_CMD, $$DIST_DIRECTORY) $$escape_expand(\\n\\t) \
            $$QMAKE_COPY $$shell_quote($$shell_path($${PWD}/Debug/$${TARGET}.exe)) $$shell_quote($$shell_path($$DIST_FILE))
    }

    first.depends = $(first) multiftpbin
    export(multiftpbin.commands)
    QMAKE_EXTRA_TARGETS += first multiftpbin

    DATAFILES.files += \
       500M.qz
    DATAFILES.path += \
       "$$DIST_DIRECTORY"
    #PACKAGE_DOMAIN：
    #PACKAGE_VERSION：
    RC_CODEPAGE=0x04b0 #unicode：指定應該被包含進一個.rc檔案中的字碼頁，僅適用於Windows
    RC_LANG=0x0409 #en_US：指定應該被包含進一個.rc檔案中的語言，僅適用於Windows
    DISTFILES += $$PWD/images/multiftp.icon
    # nmake install
    INSTALLS += DATAFILES
}
