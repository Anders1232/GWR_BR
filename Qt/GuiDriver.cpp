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
	table= NULL;
//	modelType = MODE_LAT_PLUS_LON;
}

std::string GuiDriver::GetPreview(std::string fileName, int linesInPreview, int charPerLineInPreview)
{
//	this->fileName;
	FILE *temp= tmpfile();
	LoadPreview(temp, fileName.c_str(), linesInPreview, charPerLineInPreview);
	std::string preview= "";
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

NamedColumnDoubleTable* GuiDriver::LoadFileButtonCliked(std::string fileName, char separator)
{
	table= LoadFile(fileName.c_str(), separator);
	return table;
}

/*void GuiDriver::SelectModelType(int mode)
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
*/

QString GuiDriver::Calculate(
					std::string fileName,
					char separator,
					std::string outputFileName,
					int modelType,
					std::list<std::string> const &variables,
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
		int column1= NamedColumnDoubleTable_GetColumnIndex(table, latitude.c_str());
		int column2= NamedColumnDoubleTable_GetColumnIndex(table, longitude.c_str());
		DoubleMatrix *result= LatPlusLon(table->matrix, column1, column2);
		FILE *temp= fopen(outputFileName.c_str(), "w+");
		fprintf(temp, "GWR BR" NEW_LINE);
		fprintf(temp, "File: %s\t\t\tDelimiter: %c(%d)" NEW_LINE, fileName.c_str(), separator, separator);
		fprintf(temp, "Operation: Latitude + Longitude" NEW_LINE);
		fprintf(temp, "Latitude column: %s" NEW_LINE, latitude.c_str());
		fprintf(temp, "Longitude column: %s" NEW_LINE, longitude.c_str());
		fprintf(temp, "Result:" NEW_LINE NEW_LINE);
		DoubleMatrixPrint(result, temp, "\t%lf" NEW_LINE, NEW_LINE);
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
	else if(MODE_DISTANCE_TO_ORIGIN == modelType)
	{
		int column1= NamedColumnDoubleTable_GetColumnIndex(table, latitude.c_str());
		int column2= NamedColumnDoubleTable_GetColumnIndex(table, longitude.c_str());
		DoubleMatrix *result= DistanceToOrigin(table->matrix, column1, column2);
		FILE *temp= fopen(outputFileName.c_str(), "w+");;
		fprintf(temp, "GWR BR" NEW_LINE);
		fprintf(temp, "File: %s\t\t\tDelimiter: %c(%d)" NEW_LINE, fileName.c_str(), separator, separator);
		fprintf(temp, "Operation: Distance to origin from (Longitude, Latitude)" NEW_LINE);
		fprintf(temp, "Latitude column: %s" NEW_LINE, latitude.c_str());
		fprintf(temp, "Longitude column: %s" NEW_LINE, longitude.c_str());
		fprintf(temp, "Result:" NEW_LINE NEW_LINE);
		DoubleMatrixPrint(result, temp, "\t%lf", NEW_LINE);
		QString ret= "";
	}
	else if(MODE_DISTANCE_BETWEEN_POITS == modelType)
	{
		double min, med, max;
		int column1= NamedColumnDoubleTable_GetColumnIndex(table, latitude.c_str());
		int column2= NamedColumnDoubleTable_GetColumnIndex(table, longitude.c_str());
		double **result= DistanceBetweenAllPoints(table->matrix, column1, column2, &min, &max);
		med= min+max/2;
		FILE *temp= fopen(outputFileName.c_str(), "w+");;
		fprintf(temp, "GWR BR" NEW_LINE);
		fprintf(temp, "File: %s\t\t\tDelimiter: %c(%d)" NEW_LINE, fileName.c_str(), separator, separator);
		fprintf(temp, "Operation: BetweenPoints (Longitude, Latitude)" NEW_LINE);
		fprintf(temp, "Latitude column: %s" NEW_LINE, latitude.c_str());
		fprintf(temp, "Longitude column: %s" NEW_LINE, longitude.c_str());
		fprintf(temp, "Result:" NEW_LINE NEW_LINE);
		for(int count=0; count < table->matrix->lines; count++)
		{
			for(int count2= 0; count2<= count; count2++)
			{
				fprintf(temp, "\t%lf", result[count][count2]);
			}
			fprintf(temp, NEW_LINE);
		}
		QString ret= "";
		rewind(temp);
		char aux;
		while(EOF != (aux= getc(temp)))
		{
			ret+= aux;
		}
		fclose(temp);
		for(int count=0; count < table->matrix->lines; count++ )
		{
			free(result[count]);
		}
		free(result);
		return(ret);


	}
	return QString("");
}

