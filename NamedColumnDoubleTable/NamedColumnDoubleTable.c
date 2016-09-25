#include"NamedColumnDoubleTable.h"
#include<string.h>

#ifdef _WIN32
	#define NEW_LINE "\r\n"
#else
	#define NEW_LINE "\n"
#endif

//#define DEBUG_NAMED_COLUMN

NamedColumnDoubleTable *NewNamedColumnDoubleTableWithNoMatrix(int numberOfColumns)
{
	NamedColumnDoubleTable *ret= malloc(sizeof(NamedColumnDoubleTable));
	if(NULL == ret)
	{
		return NULL;
	}
	
	ret->columnsName= malloc(numberOfColumns * sizeof(char *));
	if(NULL == ret->columnsName)
	{
		free(ret);
		return NULL;
	}
	int count;
	for(count=0; count < numberOfColumns; count++)
	{
		ret->columnsName[count]= NULL;
	}
	ret->matrix= NULL;
	return ret;
}

NamedColumnDoubleTable *NewNamedColumnDoubleTable(int numberOfLines, int numberOfColumns)
{
	if(0 > numberOfLines || 0 >= numberOfColumns)
	{
		fprintf(stderr, "[WARNING] Allocating NamedColumnDoubleTable with invalid sizes\n");
		return NULL;
	}
	NamedColumnDoubleTable *ret= NewNamedColumnDoubleTableWithNoMatrix(numberOfColumns);
	if(NULL == ret)
	{
		return NULL;
	}
	ret->matrix= NewDoubleMatrix(numberOfLines, numberOfColumns);
	if(NULL == ret->matrix)
	{
		free(ret->columnsName);
		free(ret);
		return NULL;
	}
	return ret;
}

void NamedColumnDoubleTable_SetColumnName(NamedColumnDoubleTable *table, int position, char* columnName)
{
	if(table->matrix->columns > position)
	{
		if(NULL != table->columnsName[position])
		{
			free(table->columnsName[position]);
		}
		table->columnsName[position] = malloc( strlen(columnName) * sizeof(char) + 1);
		if(NULL == table->columnsName[position])
		{
			fprintf(stdout, "[ERROR] Could not alloc enough memory to column name\n");
			exit(1);
		}
		strcpy(table->columnsName[position], columnName);
	}
	else
	{
		fprintf(stdout, "[ERROR] Tried to set the column name '%s' in the position %d, but the table only have %d columns\n", columnName, position, table->matrix->columns);
		exit(1);
	}
}

void NamedColumnDoubleTable_PrintColumnsNames(NamedColumnDoubleTable *table, FILE *output, char *format)
{
	int cont, numberOfColumns= table->matrix->columns;
	for(cont=0; cont < numberOfColumns; cont++)
	{
		fprintf(output, format, table->columnsName[cont]);
	}
}

void DeleteNamedColumnDoubleTable(NamedColumnDoubleTable *table)
{
	int cont;
	for(cont =0; cont < table->matrix->columns; cont++)
	{
		if(NULL != table->columnsName[cont])
		{
			free(table->columnsName[cont]);
		}
	}
	free(table->columnsName);
	DeleteDoubleMatrix(table->matrix);
}

NamedColumnDoubleTable *NewNamedColumnDoubleTableFromFile(char const *fileName, char separator)
{
	FILE *file = fopen(fileName, "r");
	if(NULL == file)
	{
		return NULL;
	}
	int numberOfLines=1;
	char aux, aux2;
	while(EOF != fscanf(file, "%c", &aux))
	{
		if('\n'== aux)
		{
			numberOfLines++;
			aux2= getc(file);
			if(EOF == aux2)
			{//se sim, o arquivo termina com um \n, que não deve ser contado pro numero de linhas
				numberOfLines--;
			}
			else
			{//senão desfaz essa última leitura e continua
				ungetc(aux2, file);
			}
		}
	}
	//the number of lines -1 is the number of lines of the matrix(the first line have the name of columns)
	rewind(file);
	int numberOfColumns=1;
	do
	{
		fscanf(file, "%c", &aux);
		if(aux == separator)
		{
			numberOfColumns++;
		}
	}
	while('\n' != aux);
	rewind(file);
	NamedColumnDoubleTable *returnValue= NewNamedColumnDoubleTable(numberOfLines-1, numberOfColumns);
	if(NULL == returnValue)
	{
		return NULL;
	}
	//reading the columns names
	char columnName[MAX_NAME_SIZE+1];
	columnName[MAX_NAME_SIZE]= '\0';
	int count, count2;
	for(count =0; count < numberOfColumns; count++)
	{
		for(count2 =0; count2 < MAX_NAME_SIZE; count2++)
		{
			fscanf(file, "%c", &aux);
			if(aux != separator && '\n' != aux)
			{
				columnName[count2]= aux;
			}
			else
			{
				columnName[count2]= '\0';
				break;
			}
		}
		while(aux != separator && '\n' != aux)
		{
			fscanf(file, "%c", &aux);
		}
		NamedColumnDoubleTable_SetColumnName(returnValue, count, columnName);
	}
	//reading the matrix
	double *elements= returnValue->matrix->elements;
	double temp;
	for(count =0; count < numberOfLines-1; count++)
	{
		for(count2=0; count2 < numberOfColumns; count2++)
		{
			if(0 == fscanf(file, "%lf", &temp) )
			{
				fprintf(stderr, "File format invalid!\n");
				exit(-1);
			}
#ifdef DEBUG_NAMED_COLUMN
			printf("Number read: %lf\n", temp);
#endif
			*elements= temp;
#ifdef DEBUG_NAMED_COLUMN
			printf("Number read verification: %p contains %lf\n", elements, *(elements));
#endif
			elements++;
			aux = getc(file);
			if(aux != separator && '\n' != aux && EOF != aux)
			{
				fprintf(stderr, "%s: %d\t\tWhat?? Expected: %x, Found: %x"NEW_LINE, __FILE__, __LINE__, separator, aux);
				ungetc(aux, file);
			}
		}
		fscanf(file, "%*[\n]");
	}
	//modificação temporária só para mostrar que leu o arquivo
//	NamedColumnDoubleTable_PrintAll(returnValue, stdout, "%s\t\t", "%lf\t\t", NEW_LINE);
	//fim da modificação temporária
	return returnValue;
}

void NamedColumnDoubleTable_PrintAll(NamedColumnDoubleTable *table, FILE *output, char *columnsNameFormat, char *doubleFormat, char *betweenLinesFormat)
{
	NamedColumnDoubleTable_PrintColumnsNames(table, output, columnsNameFormat);
	fprintf(output, betweenLinesFormat);
	DoubleMatrixPrint(table->matrix, output, doubleFormat, betweenLinesFormat);
}
