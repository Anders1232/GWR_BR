#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T13:35:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD/DoubleMatrixLib \
            $$PWD/Core \
            $$PWD/NamedColumnDoubleTable\
            $$PWD/GWR-Lib

TARGET = GWR_BR
TEMPLATE = app
CONFIG+= static
RESOURCES     = resources.qrc

QMAKE_CXXFLAGS += -finline-functions -Wall -pedantic -std=c++11
QMAKE_CFLAGS += -finline-functions -Wall -pedantic -std=c99

SOURCES +=\
        Qt/mainwindow.cpp \
    DoubleMatrixLib/DoubleMatrixLib.c \
    NamedColumnDoubleTable/NamedColumnDoubleTable.c \
    Qt/QtMain.cpp \
    Qt/GuiDriver.cpp \
    Core/main.cpp \
    GWR-Lib/gwr-lib.c \

HEADERS  += \
    DoubleMatrixLib/DoubleMatrixLib.h \
    NamedColumnDoubleTable/NamedColumnDoubleTable.h \
    Qt/GuiDriver.hpp \
    Core/GuiInterface.h \
    Qt/mainwindow.hpp \
    GWR-Lib/gwr-lib.h

FORMS    += Qt/mainwindow.ui

