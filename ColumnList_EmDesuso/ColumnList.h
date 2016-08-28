#ifndef COLUMN_LIST_H
#define COLUMN_LIST_H

#define MAX_COLUMN_NAME_SIZE (50)
#define COLUMN_LIST_BAD_ALLOCATION (-2)
#define COLUMN_LIST_DO_NOT_EXIST (-3)
#define INVALID_COLUMN (-4)

#include"DoubleList.h"

struct columnList
{
	char name[MAX_COLUMN_NAME_SIZE+1];
	DoubleList *elements;
	struct columnList *next;
};
typedef struct columnList ColumnList;

ColumnList *NewColumnList(char *columnName);
int AddColumn(ColumnList *list, char *columnName);
int AppendDoubeOnColumnList(ColumnList *list, int desiredColumn, double num);
int SetDoubleElement(ColumnList *list, int desiredColumn, int desiredPosition, double num);
double GetDoubleElement(ColumnList *list, int desiredColumn, int desiredPosition);
void PrintColumnList(ColumnList *list, FILE *out, char *betweenLines, char *format);
void DeleteColumnList(ColumnList *list);

#endif
