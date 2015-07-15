#-------------------------------------------------
#
# Project created by QtCreator 2015-07-13T21:52:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GraphicsClassifier
TEMPLATE = app

CONFIG += c++11

SOURCES     +=  ui/main.cpp\
                ui/mainwindow.cpp \
                comparer/comparer.cpp \
                plot/qcustomplot.cpp \
    manager/datamanager.cpp \
    util/loader.cpp \
    manager/algorithmmanager.cpp

HEADERS     +=  ui/mainwindow.h \
                comparer/comparer.h \
                plot/qcustomplot.h \
    manager/datamanager.h \
    util/loader.h \
    util/datastructures.h \
    util/constants.h \
    manager/algorithmmanager.h

FORMS       +=  ui/mainwindow.ui

RESOURCES   +=  resources.qrc
