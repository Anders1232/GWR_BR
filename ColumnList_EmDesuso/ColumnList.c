#include"ColumnList.h"
#include<string.h>
#include<stdlib.h>

ColumnList *NewColumnList(char *columnName)
{
	ColumnList *ret= malloc(sizeof(ColumnList));
	if(NULL == ret)
	{
		return NULL;
	}
	ret->elements= NewDoubleList();
	ret->next= NULL;
	strncpy(ret->name, columnName, MAX_COLUMN_NAME_SIZE);
	ret->name[MAX_COLUMN_NAME_SIZE]= '\0';
	return ret;
}

int AddColumn(ColumnList *list, char *columnName)
{
	ColumnList *newColumn= NewColumnList(columnName);
	if(NULL == newColumn)
	{
		return COLUMN_LIST_BAD_ALLOCATION;
	}
	if(NULL == list)
	{
		return COLUMN_LIST_DO_NOT_EXIST;
	}
	while(NULL != list->next)
	{
		list=list->next;
	}
	list->next= newColumn;
	return SUCCESS;
}

int AppendDoubeOnColumnList(ColumnList *list, int desiredColumn, double num)
{
	if(NULL == list)
	{
		return COLUMN_LIST_DO_NOT_EXIST;
	}
	int columnPosition=0;
	while(NULL != list->next && desiredColumn > columnPosition)
	{
		list= list->next;
		columnPosition++;
	}
	if(columnPosition == desiredColumn)
	{
		list->elements= AppendDouble(list->elements, num);
	}
	else
	{
		return INVALID_COLUMN;
	}
	return SUCCESS;
}

int SetDoubleElement(ColumnList *list, int desiredColumn, int desiredPosition, double num)
{
	if(NULL == list)
	{
		return COLUMN_LIST_DO_NOT_EXIST;
	}
	int columnPosition=0;
	while(NULL != list->next && desiredColumn > columnPosition)
	{
		list= list->next;
		columnPosition++;
	}
	if(columnPosition == desiredColumn)
	{
		return SetDouble(list->elements, desiredPosition, num);
	}
	else
	{
		return INVALID_COLUMN;
	}
}

double GetDoubleElement(ColumnList *list, int desiredColumn, int desiredPosition)
{
	if(NULL == list)
	{
		return COLUMN_LIST_DO_NOT_EXIST;
	}
	int columnPosition=0;
	while(NULL != list->next && desiredColumn > columnPosition)
	{
		list= list->next;
		columnPosition++;
	}
	if(columnPosition == desiredColumn)
	{
		return GetDouble(list->elements, desiredPosition);
	}
	else
	{
		return INVALID_COLUMN;
	}
}

void PrintColumnList(ColumnList *list, FILE *out, char *betweenLines, char *format)
{
	while(NULL != list)
	{
		PrintDoubleList(list->elements, out, format);
		fprintf(out, betweenLines);
		list= list->next;
	}
}

void DeleteColumnList(ColumnList *list)
{
	if(NULL != list)
	{
		DeleteColumnList(list->next);
		DeleteDoubleList(list->elements);
		free(list);
	}
}

