#-------------------------------------------------
#
# Project created by QtCreator 2014-08-11T15:40:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fileCrypt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    waitwindow.cpp \
    thread.cpp

HEADERS  += mainwindow.h \
    waitwindow.h \
    thread.h

FORMS    += mainwindow.ui \
    waitwindow.ui

LIBS += -lcryptopp
