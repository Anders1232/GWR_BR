#include "NamedColumnDoubleTable.h"

#define COLUMN_NAME_BUFFER_SIZE 100

NamedColumnDoubleTable* ReadSimpleFile(FILE *file, char separator)
{
	int columnsCounter;
	int linesCounter;
	columnsCounter=1;
	linesCounter=1;
	
	char aux;
	while(1 == fscanf(file, "%c", &aux))
	{
		if(separator == aux)
		{
			columnsCounter++;
		}
		else if('\n' == separator)
		{
			break;
		}
	}
	while(1 == fscanf(file, "%c", &aux))
	{
		if(feof(file))
		{
#ifdef DEBUG
printf("Essa if realmente e util! %s:\t\t%d"NEW_LINE , __FILE__, __LINE__);
#endif
		}
		else if(EOF == aux)
		{
#ifdef DEBUG
printf("Essa if realmente e util! %s:\t\t%d"NEW_LINE , __FILE__, __LINE__);
#endif
			break;
		}
		else if('\n' == aux)
		{
			linesCounter++;
		}
	}
	rewind(file);
	NamedColumnDoubleTable *ret= NewNamedColumnDoubleTable(linesCounter, columnsCounter);
	if(NULL == ret)
	{
		return NULL;
	}
	
	char columnName[COLUMN_NAME_BUFFER_SIZE+1];
	columnName[COLUMN_NAME_BUFFER_SIZE]= '\0';
	
	int columnCounter=0;
	int cont=0;
	do
	{//pensar em formar de otimizar isso aqui
		if(0 == fscanf(file, "%c", &aux) )
		{
#ifdef DEBUG
printf("Essa if realmente e util! %s:\t\t%d"NEW_LINE , __FILE__, __LINE__);
#endif
			break;
		}
		if(separator == aux || '\r' == aux || '\n' == aux)
		{
			columnName[cont]= '\0';
			//agora e enviar
			NamedColumnDoubleTable_SetColumnName(ret, columnCounter++, columnName);
			
		}
		else if(EOF == aux || feof(file))
		{
#ifdef DEBUG
printf("Essa if realmente e util! %s:\t\t%d"NEW_LINE , __FILE__, __LINE__);
#endif
			break;
		}
		else
		{
			columnName[cont++]= aux;
		}
	}
	while(columnCounter > linesCounter);
	
	double *numbers=ret->matrix->elements;
	int cont1, cont2;
	char format[5];
	format[0] = '%';
	format[1] = 'l';
	format[2] = 'f';
	format[3] = separator;
	format[4] = '\0';
	for(cont1=0; cont1 < linesCounter; cont1++)
	{
		for(cont2=0; cont2 < columnCounter; cont2++)
		{
			fscanf(file, "%c", &separator);
		}
	}
}
