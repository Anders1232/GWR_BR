#include "mainwindow.h"
#include <QApplication>

#include "GuiInterface.h"

int Gui_Init(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
