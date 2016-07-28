#-------------------------------------------------
#
# Project created by QtCreator 2016-02-13T21:54:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += network

TARGET = chat_client
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    chatclient.cpp \
    user.cpp \
    message.cpp \
    loginwidget.cpp \
    registerwidget.cpp \
    friendfindwidget.cpp \
    chatwidget.cpp \
    util.cpp

HEADERS  += widget.h \
    chatclient.h \
    user.h \
    message.h \
    loginwidget.h \
    registerwidget.h \
    util.h \
    friendfindwidget.h \
    chatwidget.h

FORMS    += widget.ui \
    loginwidget.ui \
    registerwidget.ui \
    friendfindwidget.ui \
    chatwidget.ui

OTHER_FILES += \
    doc.txt
