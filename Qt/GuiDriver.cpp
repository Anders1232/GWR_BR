#include"GuiDriver.hpp"
#include"QLineEdit"
#include"QRadioButton"
extern "C"
{
	#include "GuiInterface.h"
}
//C side functions implementations:

int Gui_Init(int argc, char *argv[])
{
	return InitQtGui(argc, argv);
}

//C++ side implementations:

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
	table= NULL;
}

void GuiDriver::FileSelected(QString fileName)
{
	this->fileName= std::string(fileName.toLocal8Bit().data() );
}

void GuiDriver::DelimiterSelected(char separator)
{
	this->separator= separator;
}

NamedColumnDoubleTable* GuiDriver::LoadFileButtonCliked(void)
{
	table= LoadFile(fileName.c_str(), separator);
	return table;
}


