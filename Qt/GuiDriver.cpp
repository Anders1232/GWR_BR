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

static void PrintfDistancesFile(double** distances, int numberOfLines, FILE *output)
{
	for(int count=0; count < numberOfLines; count++)
	{
		for(int count2= 0; count2<= count-1; count2++)
		{
			fprintf(output, "%lf,", distances[count][count2]);
		}
		fprintf(output, "%lf", distances[count][count]);
		fprintf(output, NEW_LINE);
	}
}

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
					std::string outputDistanceBetweenPointsFileName,
					int modelType,
					KernelType kernelType,
					SelectionCriteria selectionCriteria,
					std::list<std::string> const &variables,
					std::string const &identifier,
					const std::string &dependent,//independent, n?
					const std::string &latitude,
					const std::string &longitude,
					std::string const &offset,
					const std::list<std::string> &localVariables,
					const std::list<std::string> &globalVariables,
					BandwidthSelectionMethod bandSelectionMethod,
					QTextEdit &textArea,
					bool distanceInKm
				)
{
	FILE *temp= fopen(outputFileName.c_str(), "w+");
	if(GOLDEN == bandSelectionMethod)
	{
		fprintf(temp, "GWR BR" NEW_LINE);
		fprintf(temp, "File: %s\t\t\tDelimiter: %c(%d)" NEW_LINE, fileName.c_str(), separator, separator);
		FILE *temp2=NULL;
		if("" != outputDistanceBetweenPointsFileName)
		{
			temp2= fopen(outputDistanceBetweenPointsFileName.c_str(), "w");
			if(NULL == temp2)
			{
				fprintf(stderr, "Failed assertion at %s|%s:%d\n", __FILE__, __func__, __LINE__);
			}
		}
		int dependentVariable, *indepedentLocalVariables, *independentGlobalVariables;
		dependentVariable= NamedColumnDoubleTable_GetColumnIndex(table, dependent.c_str());
		int latitudeColumn= NamedColumnDoubleTable_GetColumnIndex(table, latitude.c_str());
		int longitudecolumn= NamedColumnDoubleTable_GetColumnIndex(table, longitude.c_str());

		independentGlobalVariables= (int*)malloc(sizeof(int) * (globalVariables.size()+1) );

		indepedentLocalVariables= (int*)malloc(sizeof(int) * (localVariables.size()+1) );
		indepedentLocalVariables[localVariables.size()]= -1;
		auto i= localVariables.begin();
		for(unsigned int count=0; count < localVariables.size(); count++, i++)
		{
			indepedentLocalVariables[count]= NamedColumnDoubleTable_GetColumnIndex(table, i->c_str());
		}

		i= globalVariables.begin();
		for(unsigned int count=0; count < globalVariables.size(); count++, i++)
		{
			independentGlobalVariables[count]= NamedColumnDoubleTable_GetColumnIndex(table, i->c_str());
		}

		independentGlobalVariables= (int*)malloc(sizeof(int) * (globalVariables.size()+1) );
		independentGlobalVariables[globalVariables.size()]= -1;
		independentGlobalVariables[globalVariables.size()]= -1;
//		fprintf(temp ,"%s|%s:%d\t[DEBUG] I was here!\r\n", __FILE__, __func__, __LINE__);
		CalculateGolden(textArea, temp, temp2, kernelType, this->table->matrix, distanceInKm, dependentVariable, indepedentLocalVariables, independentGlobalVariables, latitudeColumn, longitudecolumn);
//		fprintf(temp ,"%s|%s:%d\t[DEBUG] I was here!\r\n", __FILE__, __func__, __LINE__);

		free(independentGlobalVariables);
		free(indepedentLocalVariables);
		if(NULL != temp2)
		{
		fclose(temp2);
		}
	}
	if(MODE_LAT_PLUS_LON == modelType)
	{
		int column1= NamedColumnDoubleTable_GetColumnIndex(table, latitude.c_str());
		int column2= NamedColumnDoubleTable_GetColumnIndex(table, longitude.c_str());
		DoubleMatrix *result= LatPlusLon(table->matrix, column1, column2);
		fprintf(temp, "GWR BR" NEW_LINE);
		fprintf(temp, "File: %s\t\t\tDelimiter: %c(%d)" NEW_LINE, fileName.c_str(), separator, separator);
//		fprintf(temp, "Operation: Latitude + Longitude" NEW_LINE);
		fprintf(temp, "Latitude column: %s" NEW_LINE, latitude.c_str());
		fprintf(temp, "Longitude column: %s" NEW_LINE, longitude.c_str());
		fprintf(temp, "Result:" NEW_LINE NEW_LINE);
//		DoubleMatrixPrint(result, temp, "\t%lf" NEW_LINE, NEW_LINE);
		QString ret= "";
		rewind(temp);
		char aux;
		while(EOF != (aux= getc(temp)))
		{
			ret+= aux;
		}
		if("" != outputDistanceBetweenPointsFileName)
		{
			CalculateDistanceBetweenPoints(fileName, separator, outputDistanceBetweenPointsFileName, latitude, longitude);
		}
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
		fprintf(temp, "GWR BR" NEW_LINE);
		fprintf(temp, "File: %s\t\t\tDelimiter: %c(%d)" NEW_LINE, fileName.c_str(), separator, separator);
		fprintf(temp, "Operation: Distance to origin from (Longitude, Latitude)" NEW_LINE);
		fprintf(temp, "Latitude column: %s" NEW_LINE, latitude.c_str());
		fprintf(temp, "Longitude column: %s" NEW_LINE, longitude.c_str());
		fprintf(temp, "Result:" NEW_LINE NEW_LINE);
		DoubleMatrixPrint(result, temp, "\t%lf", NEW_LINE);
		QString ret= "";
		if("" != outputDistanceBetweenPointsFileName)
		{
			CalculateDistanceBetweenPoints(fileName, separator, outputDistanceBetweenPointsFileName, latitude, longitude);
		}
	}
	else if(MODE_DISTANCE_BETWEEN_POITS == modelType)

	{
		return 	CalculateDistanceBetweenPoints(fileName, separator, outputDistanceBetweenPointsFileName, latitude, longitude);

	}
	fclose(temp);
	return QString("");

}

QString GuiDriver::CalculateDistanceBetweenPoints
(
		std::string fileName,
		char separator,
		std::string outputFileName,
		const std::string &latitude,
		const std::string &longitude
)
{
	double min, max;
	int column1= NamedColumnDoubleTable_GetColumnIndex(table, latitude.c_str());
	int column2= NamedColumnDoubleTable_GetColumnIndex(table, longitude.c_str());
	double **result= DistanceBetweenAllPoints(table->matrix, column1, column2, &min, &max, false);
//	med= min+max/2;
	FILE *temp= fopen(outputFileName.c_str(), "w+");;
	fprintf(temp, "GWR BR" NEW_LINE);
	fprintf(temp, "File: %s\t\t\tDelimiter: %c(%d)" NEW_LINE, fileName.c_str(), separator, separator);
	fprintf(temp, "Operation: BetweenPoints (Longitude, Latitude)" NEW_LINE);
	fprintf(temp, "Latitude column: %s" NEW_LINE, latitude.c_str());
	fprintf(temp, "Longitude column: %s" NEW_LINE, longitude.c_str());
	fprintf(temp, "The shortest distance: %lf" NEW_LINE, min);
	fprintf(temp, "The largest distance: %lf" NEW_LINE, max);
	fprintf(temp, "Result:" NEW_LINE NEW_LINE);
	PrintfDistancesFile(result, table->matrix->lines, temp);
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

void GuiDriver::CalculateGolden(QTextEdit &textArea,
		FILE* outputFile,
		FILE *outputDistancesBetweenPoints,
		KernelType kernelType,
		DoubleMatrix *data,
		bool distanceInKm,
		int dependentVariable,
		int *independentLocalVariables,
		int *independentGlobalVariables,
		int latitude,
		int longitude
)
{
	double maxDisBetPoints;
	FowardList *fw= NewFowardList();
	GoldenArguments args;
	args.communication=fw;
	args.data=data;
	args.distanceInKM= distanceInKm;
	args.method= kernelType;
	args.returnDistancesMatrix= (outputDistancesBetweenPoints != NULL);
	args.xVarColumn_dependentVariable= dependentVariable;
	args.yVarColumn_independentGlobalVariables= independentGlobalVariables;
	args.yVarColumn_independentLocalVariables= independentLocalVariables;
	args.x_dCoord= latitude;
	args.y_dCoord= longitude;
	args.outMaxDistanceBetweenPoints= &maxDisBetPoints;
	double h;
	pthread_t thread;
	pthread_create(&thread, NULL, Golden, &args);
	if(ADAPTIVE_N == kernelType)
	{
		printf("%s|%s:%d\t[DEBUG] I was here!\r\n", __FILE__, __func__, __LINE__);
		fprintf(outputFile, "xMin\t\tcv1\t\tcv2\n");
		GoldenDataIfAdpN* response;
		while(NULL !=(response= (GoldenDataIfAdpN*)FowardListGetElement(fw) ) )
		{
			QString temp= "\nxMin:\t";
			temp+= QString::fromStdString( std::to_string(response->xMin) );
			temp+= "\t\t";
			temp+= QString::fromStdString( std::to_string(response->cv1) ) ;
			temp+= "\t\t";
			temp+= QString::fromStdString( std::to_string(response->cv2) );
			temp+= "\n";
			textArea.append(temp);
			fprintf(outputFile, temp.toStdString().c_str());
			fflush(outputFile);
		}
	}
	else
	{
		GoldenDataIfNotAdpN* response;
//		if(NULL != outputFile)
//		{
			fprintf(outputFile, "\th1\t\th2\t\tcv1\t\tcv2\n\n");
//		}
		textArea.append(QString("\th1\t\th2\t\tcv1\t\tcv2\n\n"));
		while(NULL !=(response= (GoldenDataIfNotAdpN*)FowardListGetElement(fw) ) )
		{
			QString temp= "\t";
			temp+= QString::number(response->h1, 'f');
			temp+= "\t\t ";
			temp+= QString::number(response->h2, 'f');
			temp+= "\t\t ";
			temp+= QString::number(response->cv1, 'f');
			temp+= "\t\t ";
			temp+= QString::number(response->cv2, 'f');
			temp+= "\n";
			textArea.append(temp);
			h= response->h1;
//			if(NULL != outputFile)
//			{
			fprintf(outputFile, temp.toStdString().c_str());
//			}
			fflush(outputFile);
		}
	}
	double** distances=NULL;
	pthread_join(thread, (void**)distances);
	if(distances != NULL)
	{
		distances= (double**)*distances;
		if(NULL != outputDistancesBetweenPoints)
		{
			PrintfDistancesFile(distances, data->lines, outputDistancesBetweenPoints);
		}
	}
	GWRArguments gwrArgs;
	gwrArgs.data= data;
	gwrArgs.yVarColumn_independentLocalVariables= independentLocalVariables;
	gwrArgs.xVarColumn_dependentVariable= dependentVariable;
	gwrArgs.x_dCoord= latitude;
	gwrArgs.y_dCoord= longitude;
	gwrArgs.method= kernelType;
	gwrArgs.distanceInKm= distanceInKm;
	gwrArgs.maxDistanceBetweenPoints= maxDisBetPoints;
	gwrArgs.h= h;
	GWR(&gwrArgs);
}


