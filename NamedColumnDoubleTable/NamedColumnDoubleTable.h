#ifndef NAMED_COLUMN_DOUBLE_TABLE_H
#define NAMED_COLUMN_DOUBLE_TABLE_H

#include"DoubleMatrixLib.h"

#define MAX_NAME_SIZE 50

struct namedColumnDoubleTable
{
	char **columnsName;
	DoubleMatrix *matrix;
};
typedef struct namedColumnDoubleTable NamedColumnDoubleTable;

NamedColumnDoubleTable *NewNamedColumnDoubleTable(int numberOfLines, int numberOfColumns);
NamedColumnDoubleTable *NewNamedColumnDoubleTableWithNoMatrix(int numberOfColumns);
void NamedColumnDoubleTable_SetColumnName(NamedColumnDoubleTable *table, int position, char* columnName);
void NamedColumnDoubleTable_PrintColumnsNames(NamedColumnDoubleTable *table, FILE *output, char *format);
void DeleteNamedColumnDoubleTable(NamedColumnDoubleTable *table);

NamedColumnDoubleTable *NewNamedColumnDoubleTableFromFile(char const *fileName, char separator);
#endif
