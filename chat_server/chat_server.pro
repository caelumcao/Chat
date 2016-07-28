TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    util.cpp \
    chatserver.cpp \
    user.cpp \
    message.cpp \
    dbconnector.cpp \
    dataprocess.cpp

HEADERS += \
    util.h \
    chatserver.h \
    user.h \
    message.h \
    dbconnector.h \
    dataprocess.h

LIBS += -lmysqlclient \
    -lpthread

DISTFILES += \
    doc

