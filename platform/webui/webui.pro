#-------------------------------------------------
#
# Project created by QtCreator 2019-06-07T10:43:07
#
#-------------------------------------------------

QT += core gui webenginewidgets webchannel

contains( DEFINES, debug){
CONFIG+=console
}


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = webui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_FILE = src/webui.rc

INCLUDEPATH += $$PWD/../quark/liblua/src
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../quark/liblua/output/ -llualib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../quark/liblua/output/ -llualib
else:unix: LIBS += -L$$PWD/../quark/liblua/output/ -lLua -ldl

SOURCES += \
    src/webdocument.cpp \
    src/webwidget.cpp \
    src/attestor.cpp \
    src/define.cpp \
    src/sha1.cpp \
    src/main.cpp

HEADERS  += \
    src/webdocument.h \
    src/webwidget.h \
    src/attestor.h \
    src/define.h \
    src/sha1.h

DISTFILES += \
    src/webui.rc \
    src/martx.ico

RESOURCES += \
    src/webui.qrc


