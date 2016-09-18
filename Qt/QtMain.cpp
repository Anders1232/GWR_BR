#include "mainwindow.hpp"
#include <QApplication>

#include "GuiDriver.hpp"

int InitQtGui(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}

