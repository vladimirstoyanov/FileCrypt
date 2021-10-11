#-------------------------------------------------
#
# Project created by QtCreator 2014-08-11T15:40:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = fileCrypt
TEMPLATE = app


SOURCES += main.cpp\
    aboutwindow.cpp \
    aes.cpp \
    cryptography_thread.cpp \
    file.cpp \
    fileoperations.cpp \
    mainwindow.cpp \
    path.cpp \
    loadingwindow.cpp

HEADERS  += mainwindow.h \
    aboutwindow.h \
    aes.h \
    cryptography_thread.h \
    file.h \
    fileoperations.h \
    path.h \
    loadingwindow.h

FORMS    += mainwindow.ui \
    aboutwindow.ui \
    loadingwindow.ui
