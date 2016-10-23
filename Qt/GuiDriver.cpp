#include"GuiDriver.hpp"
#include"QLineEdit"
#include"QRadioButton"
//#include<iostream>
extern "C"
{
	#include "GuiInterface.h"
	#include <stdio.h>
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

QString GuiDriver::GetPreview(int linesInPreview, int charPerLineInPreview)
{
//	this->fileName;
	FILE *temp= tmpfile();
	LoadPreview(temp, this->fileName.c_str(), linesInPreview, charPerLineInPreview);
	QString preview= "";
	rewind(temp);
	char aux;
	while(EOF != (aux= getc(temp)))
	{
//		char aux= getc(temp);
		preview+= aux;
	}
//	std::cout << preview.toLatin1().toStdString() << std::endl;
	fclose(temp);
	return(preview);
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


