#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T13:35:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD/DoubleMatrixLib

TARGET = GWR_BR
TEMPLATE = app


SOURCES += Qt/main.cpp\
        Qt/mainwindow.cpp \
    DoubleMatrixLib/DoubleMatrixLib.c \
    NamedColumnDoubleTable/NamedColumnDoubleTable.c

HEADERS  += Qt/mainwindow.h \
    DoubleMatrixLib/DoubleMatrixLib.h \
    NamedColumnDoubleTable/NamedColumnDoubleTable.h

FORMS    += Qt/mainwindow.ui
