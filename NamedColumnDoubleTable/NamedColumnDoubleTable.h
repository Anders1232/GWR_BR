#ifndef NAMED_COLUMN_DOUBLE_TABLE_H
#define NAMED_COLUMN_DOUBLE_TABLE_H

#include"DoubleMatrixLib.h"

#ifdef _WIN32
	#define NEW_LINE "\r\n"
#else
	#define NEW_LINE "\n"
#endif

#define MAX_NAME_SIZE 50

struct namedColumnDoubleTable
{
	char **columnsName;
	DoubleMatrix *matrix;
};
typedef struct namedColumnDoubleTable NamedColumnDoubleTable;

NamedColumnDoubleTable *NewNamedColumnDoubleTable(int numberOfLines, int numberOfColumns);
NamedColumnDoubleTable *NewNamedColumnDoubleTableWithNoMatrix(int numberOfColumns);
void NamedColumnDoubleTable_SetColumnName(NamedColumnDoubleTable *table, int position, char const* columnName);
void NamedColumnDoubleTable_PrintColumnsNames(NamedColumnDoubleTable *table, FILE *output, char const *format);
void NamedColumnDoubleTable_PrintAll(NamedColumnDoubleTable *table, FILE *output,
									char const *columnsNameFormat,
									char const *doubleFormat,
									char const *betweenLinesFormat);
void DeleteNamedColumnDoubleTable(NamedColumnDoubleTable *table);
int NamedColumnDoubleTable_GetColumnIndex(NamedColumnDoubleTable *table, char const *columnName);

NamedColumnDoubleTable *NewNamedColumnDoubleTableFromFile(char const *fileName, char separator);
#endif

