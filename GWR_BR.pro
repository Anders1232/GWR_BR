#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T13:35:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD/DoubleMatrixLib \
            $$PWD/Core

TARGET = GWR_BR
TEMPLATE = app

QMAKE_CXXFLAGS += -finline-functions -Wall -pedantic

SOURCES +=\
        Qt/mainwindow.cpp \
    DoubleMatrixLib/DoubleMatrixLib.c \
    NamedColumnDoubleTable/NamedColumnDoubleTable.c \
    Qt/QtMain.cpp \
    Core/main.c

HEADERS  += Qt/mainwindow.h \
    DoubleMatrixLib/DoubleMatrixLib.h \
    NamedColumnDoubleTable/NamedColumnDoubleTable.h \
    Core/GuiInterface.h

FORMS    += Qt/mainwindow.ui
