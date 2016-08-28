#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

#include<stdio.h>

#define DOUBLE_LIST_INVALID_POSITION (-1)
#define SUCCESS (0)

struct doubleList
{
	double n;
	struct doubleList *next;
};
typedef struct doubleList DoubleList;

DoubleList * NewDoubleList(void);
DoubleList * AppendDouble(DoubleList *list, double num);
int SetDouble(DoubleList *list, int desiredPosition, double num);
double GetDouble(DoubleList *list, int desiredPosition);
void PrintDoubleList(DoubleList *list, FILE *out, char *format);
void DeleteDoubleList(DoubleList *list);

#endif
