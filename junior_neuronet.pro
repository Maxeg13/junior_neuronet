#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T13:13:06
#
#-------------------------------------------------

#QT       += core gui
LIBS += -LC:/QWT/lib -lqwt
CONFIG   += qwt
INCLUDEPATH+=C:/QWT/include
#CONFIG   += console
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = junior_neuronet
TEMPLATE = app

SOURCES += main.cpp\
        dialog.cpp \
    neuronizh.cpp \
    net.cpp \
    pattern.cpp \
    drawing.cpp

HEADERS  += dialog.h \
    neuronizh.h \
    cnet.h \
    pattern.h \
    drawing.h
