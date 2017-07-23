#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T13:35:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD/DoubleMatrixLib \
            $$PWD/Init \
            $$PWD/NamedColumnDoubleTable\
            $$PWD/GWR-Lib\
            $$PWD/FowardList

TARGET = GWR_BR
TEMPLATE = app
CONFIG+= static
RESOURCES     = resources.qrc

QMAKE_CXXFLAGS += -finline-functions -Wall -pedantic -std=c++11
QMAKE_CFLAGS += -finline-functions -Wall -pedantic -std=c99

QMAKE_CFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_DEBUG += -O0

QMAKE_CFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE += -O3

SOURCES +=\
        Qt/mainwindow.cpp \
    DoubleMatrixLib/DoubleMatrixLib.c \
    NamedColumnDoubleTable/NamedColumnDoubleTable.c \
    Qt/QtMain.cpp \
    Qt/GuiDriver.cpp \
    Init/main.cpp \
    GWR-Lib/gwr-lib.c \
    FowardList/FowardList.c \
    LeitorArquivoSimples/LeitorArquivoSimples.c

HEADERS  += \
    DoubleMatrixLib/DoubleMatrixLib.h \
    NamedColumnDoubleTable/NamedColumnDoubleTable.h \
    Qt/GuiDriver.hpp \
    Init/GuiInterface.h \
    Qt/mainwindow.hpp \
    GWR-Lib/gwr-lib.h \
    FowardList/FowardList.h \
    Init/GuiInterface.h

FORMS    += Qt/mainwindow.ui

LIBS += -lpthread
