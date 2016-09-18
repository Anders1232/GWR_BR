#include "GuiDriver.hpp"
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

void GuiDriver::SeparatorSelected(char separator)
{
	this->separator= separator;
}

void GuiDriver::LoadFileButtonCliked(void)
{
	table= LoadFile(fileName.c_str(), separator);
}


