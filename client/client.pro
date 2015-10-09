#-------------------------------------------------
#
# Project created by QtCreator 2015-10-06T15:48:37
#
#-------------------------------------------------

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


HEADERS       = client.h \
    mainwindow.h
SOURCES       = client.cpp \
                 main.cpp \
    mainwindow.cpp

FORMS += \
    mainwindow.ui
