#include "mainwindow.h"
#include <QApplication>

#include "GuiInterface.hpp"
#include "GuiDriver.h"

int Gui_Init(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}

/*static GuiDriver& GetReference(void)
{
	static GuiDriver gui;
	return gui;
}
*/
GuiDriver::GuiDriver(void)
{
	separator = '\t';
	fileName= "";
}

void GuiDriver::FileSelected(QString fileName)
{
	this->fileName= std::string(fileName.toLocal8Bit().data() );
}

void GuiDriver::SeparatorSelected(char separator)
{
	this->separator= separator;
}
