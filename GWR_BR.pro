#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T13:35:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD/DoubleMatrixLib \
            $$PWD/Core \
            $$PWD/NamedColumnDoubleTable

TARGET = GWR_BR
TEMPLATE = app

QMAKE_CXXFLAGS += -finline-functions -Wall -pedantic

SOURCES +=\
        Qt/mainwindow.cpp \
    DoubleMatrixLib/DoubleMatrixLib.c \
    NamedColumnDoubleTable/NamedColumnDoubleTable.c \
    Qt/QtMain.cpp \
    Qt/GuiDriver.cpp \
    Core/main.cpp

HEADERS  += \
    DoubleMatrixLib/DoubleMatrixLib.h \
    NamedColumnDoubleTable/NamedColumnDoubleTable.h \
    Qt/GuiDriver.hpp \
    Core/GuiInterface.h \
    Qt/mainwindow.hpp

FORMS    += Qt/mainwindow.ui
