#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T18:42:55
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WhatsProg
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


SOURCES +=\
    mysocket.cpp \
    whatsprog_dados.cpp \
    whatsprog_dados_cliente.cpp \
    whatsprog_login.cpp \
    whatsprog_main.cpp \
    whatsprog_novaconversa.cpp \
    main_cliente.cpp \
    whatsprog_thread.cpp

HEADERS  += \
    MySocket/mysocket.h \
    mysocket.h \
    whatsprog_dados.h \
    whatsprog_dados_cliente.h \
    whatsprog_dados_servidor.h \
    whatsprog_login.h \
    whatsprog_main.h \
    whatsprog_novaconversa.h \
    whatsprog_thread.h

FORMS    += \
    whatsprog_login.ui \
    whatsprog_main.ui \
    whatsprog_novaconversa.ui

LIBS   += \
    -lWs2_32
