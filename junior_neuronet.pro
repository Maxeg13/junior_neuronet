#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T13:13:06
#
#-------------------------------------------------

#QT       += core gui
LIBS+=-L$$OUT_PWD/release -lmyo32
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
    drawing.cpp \
    datacollector.cpp \
#    datacollector.cpp \
#    dialog.cpp \
#    drawing.cpp \
#    main.cpp \
#    net.cpp \
#    neuronizh.cpp \
#    pattern.cpp

HEADERS  += dialog.h \
    neuronizh.h \
    cnet.h \
    pattern.h \
    drawing.h \
    datacollector.h \
    myo/cxx/detail/ThrowOnError.hpp \
    myo/cxx/impl/Hub_impl.hpp \
    myo/cxx/impl/Myo_impl.hpp \
    myo/cxx/impl/Pose_impl.hpp \
    myo/cxx/DeviceListener.hpp \
    myo/cxx/Hub.hpp \
    myo/cxx/Myo.hpp \
    myo/cxx/Pose.hpp \
    myo/cxx/Quaternion.hpp \
    myo/cxx/Vector3.hpp \
    myo/libmyo/detail/visibility.h \
    myo/libmyo.h \
    myo/myo.hpp \


DISTFILES += \
    myo/libmyo/myo32.dll \
    myo/libmyo/myo64.dll
