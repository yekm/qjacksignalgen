#-------------------------------------------------
#
# Project created by QtCreator 2017-05-28T21:36:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qjacksignalgen
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

QMAKE_CXXFLAGS += -Wall -Wextra

LIBS += ../qjacksignalgen/build/libjackcpp.a -ljack

INCLUDEPATH += jackcpp/include

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    signalgenerator.cpp \
    freqwidget.cpp

HEADERS += \
        mainwindow.hpp \
    signalgenerator.hpp \
    freqwidget.hpp

FORMS += \
        mainwindow.ui
