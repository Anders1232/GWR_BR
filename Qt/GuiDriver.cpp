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
	modelType = MODE_LAT_PLUS_LON;
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

void GuiDriver::SelectModelType(int mode)
{
	this->modelType= mode;
}

static double Mult(double a, double b)
{
	return a*b;
}

static double Add(double a, double b)
{
	return a+b;
}

QString GuiDriver::Calculate(std::list<std::string> const &variables,
					std::string const &identifier,
					const std::string &dependent,
					const std::string &latitude,
					const std::string &longitude,
					std::string const &offset,
					const std::list<std::string> &localVariables,
					const std::list<std::string> &globalVariables
				)
{
	if(MODE_LAT_PLUS_LON == modelType)
	{
//		NamedColumnDoubleTable *operator1= NewNamedColumnDoubleTableWithNoMatrix(3);
		int linesNumber= table->matrix->lines;
		DoubleMatrix *operator1= NewDoubleMatrix(linesNumber, 1);
		DoubleMatrix *operator2= NewDoubleMatrix(linesNumber, 1);
		int column1= NamedColumnDoubleTable_GetColumnIndex(table, latitude.c_str());
		int column2= NamedColumnDoubleTable_GetColumnIndex(table, longitude.c_str());
		for(int count =0; count < linesNumber; count++)
		{
			DoubleMatrixSetElement(operator1, 0, count, DoubleMatrixGetElement(table->matrix, column1, count));
			DoubleMatrixSetElement(operator2, 0, count, DoubleMatrixGetElement(table->matrix, column2, count));
		}
		DoubleMatrix *result = DoubleMatrixElementBinaryOperation(operator1, operator2, false, Add);

		FILE *temp= tmpfile();
		DoubleMatrixPrint(result, temp, "\t%lf", "\n");
		QString ret= "";
		rewind(temp);
		char aux;
		while(EOF != (aux= getc(temp)))
		{
			ret+= aux;
		}
		fclose(temp);
		return(ret);

	}
	else if (MODE_DEPENDENT_TIMES_OFFSET == modelType)
	{

	}
	return QString("");
}
