#include "GuiDriver.hpp"
#include "GuiInterface.h"

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

}


