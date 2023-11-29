#-------------------------------------------------
#
# Project created by QtCreator 2023-08-31T21:25:47
#
#-------------------------------------------------

QT       += core gui network sql

LIBS += -lpthread libwsock32 libws2_32 libiphlpapi

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tencent
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    account.cpp \
    group.cpp \
    smtp.cpp \
    qqui_kdz.cpp \
    qqlistui_kdz.cpp \
    qqinfoui_kdz.cpp \
    qqchatui_kdz.cpp \
    qqexpui_kdz.cpp \
    database.cpp \
    client_kdz.cpp \
    wcui_kdz.cpp \
    wclist_kdz.cpp \
    wbui_kdz.cpp \
    wblist_kdz.cpp \
    getip_kdz.cpp

HEADERS += \
        widget.h \
    account.h \
    group.h \
    smtp.h \
    qqui_kdz.h \
    qqlistui_kdz.h \
    qqinfoui_kdz.h \
    qqchatui_kdz.h \
    qqexpui_kdz.h \
    database.h \
    client_kdz.h \
    wcui_kdz.h \
    wclist_kdz.h \
    wbui_kdz.h \
    wblist_kdz.h \
    getip_kdz.h

FORMS += \
        widget.ui \
    qqui_kdz.ui \
    qqlistui_kdz.ui \
    qqinfoui_kdz.ui \
    qqchatui_kdz.ui \
    qqexpui_kdz.ui \
    client_kdz.ui \
    wcui_kdz.ui \
    wclist_kdz.ui \
    wbui_kdz.ui \
    wblist_kdz.ui

RESOURCES += \
    image.qrc
