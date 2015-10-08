#-------------------------------------------------
#
# Project created by QtCreator 2015-10-06T13:49:09
#
#-------------------------------------------------

QT       += core gui network
QT -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = scanner

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    commandreader.cpp \
    strategyselector.cpp \
    lazystrategy.cpp \
    threadpoolstrategy.cpp \
    runnableseeker.cpp

HEADERS += \
    server.h \
    commandreader.h \
    strategyselector.h \
    lazystrategy.h \
    threadpoolstrategy.h \
    runnableseeker.h
