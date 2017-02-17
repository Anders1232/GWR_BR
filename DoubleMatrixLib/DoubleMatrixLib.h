#ifndef DOUBLE_MATRIX_LIB_H
#define DOUBLE_MATRIX_LIB_H

#include<stdbool.h>
#include<stdlib.h>
#include <stdio.h>

enum doubleMatrixType
{
	NORMAL_MATRIX,
	DIAGONAL,
	IDENTITY,
	TRIANGULAR_MATRIX
};

typedef enum doubleMatrixType DoubleMatrixType;

struct doubleMatrix
{
	DoubleMatrixType type;
	int lines;
	int columns;
	double *elements;
};
typedef struct doubleMatrix DoubleMatrix;

enum comparison
{
	GREATER,
	GREATER_OR_EQUAL,
	EQUAL,
	LESS_OR_EQUAL,
	LESS
};
typedef enum comparison Comparison;
/*
enum type
{
	INT16,
	INT32,
	INT64,
	UINT16,
	UINT32,
	UINT64,
	FLOAT,
	DOUBLE,
	LONG_DOUBLE
};
typedef enum type Type;
*/

DoubleMatrix* NewDoubleMatrix(int lines, int columns);	//implemented
DoubleMatrix* NewDoubleMatrixAndInitializeElements(int lines, int columns, double defaultElement);	//implemented
void DeleteDoubleMatrix(DoubleMatrix*);	//implemented
DoubleMatrix* DoubleMatrixCopy(DoubleMatrix*);	//implemented
DoubleMatrix* DoubleMatrixIdentity(int oneDimensionSize);	//implemented
DoubleMatrix* DoubleMatrix_Add(DoubleMatrix*, DoubleMatrix*, bool resultInTheFirstMatrix);	//implemented
DoubleMatrix* DoubleMatrixScalarMultiplication(DoubleMatrix *matrix, double scalar, bool resultInTheSameMatrix);	//implemented
DoubleMatrix* DoubleMatrixTranspose(DoubleMatrix*, bool resultInTheSameMatrix);	//implemented, need testing
DoubleMatrix* DoubleMatrixMultiplication(DoubleMatrix*, DoubleMatrix*);
DoubleMatrix* DoubleMatrixCompare(DoubleMatrix*, DoubleMatrix*, Comparison);
DoubleMatrix* DoubleMatrixElementBinaryOperation(DoubleMatrix* , DoubleMatrix*, bool resultInTheFirstMatrix, double(*ElementBinFunction)(double element1, double element2));
DoubleMatrix* DoubleMatrixElementUnaryOperation(DoubleMatrix*, bool resultInTheFirstMatrix, double(*ElementUnaFunction)(double element));
double DoubleMatrixMaxElement(DoubleMatrix*);
double DoubleMatrixMinElement(DoubleMatrix*);
double DoubleMatrixGetElement(DoubleMatrix *, int line, int column);	//implemented
void DoubleMatrixSetElement(DoubleMatrix *,int line, int column, double Element);	//implemented
DoubleMatrix* DoubleMatrixKroneckerProduct(DoubleMatrix*, DoubleMatrix*, bool resultInTheFirstMatrix);
DoubleMatrix* DoubleMatrixElementDivision(DoubleMatrix*, DoubleMatrix*, bool resultInTheFirstMatrix);
int DoubleMatrixColumnsNumber(DoubleMatrix*);
int DoubleMatrixLinesNumber(DoubleMatrix*);
bool DoubleMatrixHaveSameDimensionsAndSameElementSize(DoubleMatrix*, DoubleMatrix*);	//implemented
void DoubleMatrixPrint(DoubleMatrix* matrix, FILE *output, char const *doubleFormat, char const *betweenLinesFormat);//implemented
void DoubleMatrixAddLine(DoubleMatrix *matrix);
void DoubleMatrixAddColumn(DoubleMatrix *matrix);
DoubleMatrix* NewLineDoubleMatrixFromMatrix(DoubleMatrix *origin, int lineFromOrigin);
DoubleMatrix* NewColumnDoubleMatrixFromMatrix(DoubleMatrix *origin, int columnFromOrigin);
double DoubleMatrixDeterminant(DoubleMatrix *mat);
DoubleMatrix *DoubleMatrixInverse(DoubleMatrix *matrix);

#endif
