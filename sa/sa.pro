QT       += core

QT       -= gui

TARGET = sa
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Activ.cpp \
    Mode.cpp \
    Resource.cpp \
    debug.cpp \
    RCPSP.cpp \
    Solution.cpp

HEADERS += \
    Activ.h \
    Mode.h \
    Resource.h \
    debug.h \
    RCPSP.h \
    Solution.h \
    defs.h
