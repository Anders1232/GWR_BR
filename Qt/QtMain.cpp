#include "mainwindow.h"
#include <QApplication>

#include "GuiInterface.hpp"

int Gui_Init(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}

class GuiDriver
{
	public:
		static void File();
};
