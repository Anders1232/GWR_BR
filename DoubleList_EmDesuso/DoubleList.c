#include "DoubleList.h"
#include<stdlib.h>
#include<math.h>

DoubleList * NewDoubleList(void)
{
	return NULL;
}

DoubleList * AppendDouble(DoubleList *list, double num)
{
	DoubleList *newElement= malloc(sizeof(DoubleList));
	DoubleList *listBegin= list;
	if(NULL == newElement)
	{
		return NULL;
	}
	newElement->n= num;
	newElement-> next= NULL;
	if(NULL == list)
	{
		listBegin= newElement;
	}
	else
	{
		while(NULL != list->next)
		{
			list=list->next;
		}
		list->next= newElement;
	}
	return listBegin;
}


int SetDouble(DoubleList *list, int desiredPosition, double num)
{
	int position=0;
	while(desiredPosition > position && NULL != list)
	{
		list= list->next;
		position++;
	}
	if(NULL == list)
	{
		return DOUBLE_LIST_INVALID_POSITION;
	}
	list->n= num;
	return SUCCESS;
}

double GetDouble(DoubleList *list, int desiredPosition)
{
	int position=0;
	while(desiredPosition > position && NULL != list)
	{
		list= list->next;
		position++;
	}
	if(NULL == list)
	{
		return nan("");
	}
	return list->n;
}

void PrintDoubleList(DoubleList *list, FILE *out, char *format)
{
	while(NULL != list)
	{
		fprintf(out, format, list->n);
		list= list->next;
	}
}

void DeleteDoubleList(DoubleList *list)
{
	if(NULL != list)
	{
		DeleteDoubleList(list->next);
		free(list);
	}
}

