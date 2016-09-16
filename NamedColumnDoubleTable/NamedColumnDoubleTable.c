#include"NamedColumnDoubleTable.h"
#include<string.h>

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
	int cont;
	for(cont=0; cont < numberOfColumns; cont++)
	{
		ret->columnsName[cont]= NULL;
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

NamedColumnDoubleTable *NewNamedColumnDoubleTableFromFile(char *fileName, char separator)
{
	FILE *arq = fopen(fileName, "r");
	if(NULL == arq)
	{
		return NULL;
	}
	int numberOfLines=1;
	char aux;
	while(EOF != fscanf(arq, "%c", &aux))
	{
		if('\n'== aux)
		{
			numeroDeLinhas++;
			aux2= getc(arq);
			if(EOF == aux2)
			{//se sim, o arquivo termina com um \n, que não deve ser contado pro numero de linhas
				numeroDeLinhas--;
			}
			else
			{//senão desfaz essa última leitura e continua
				ungetc(aux2, arq);
			}
		}
	}
	//the number of lines -1 is the number of lines of the matrix(the first line have the name of columns)
}
