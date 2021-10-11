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
    about_window.cpp \
    aes.cpp \
    cbc_decrypt.cpp \
    cbc_encrypt.cpp \
    cryptographic_thread.cpp \
    cryptography.cpp \
    file.cpp \
    file_operations.cpp \
    loading_window.cpp \
    main_window.cpp \
    null_cryptography.cpp \
    path.cpp

HEADERS  += \
    about_window.h \
    aes.h \
    cbc_decrypt.h \
    cbc_encrypt.h \
    cryptographic_thread.h \
    cryptography.h \
    file.h \
    file_operations.h \
    loading_window.h \
    main_window.h \
    null_cryptography.h \
    path.h

FORMS    += \
    about_window.ui \
    loading_window.ui \
    main_window.ui
