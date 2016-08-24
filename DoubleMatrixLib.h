#ifndef DOUBLE_MATRIX_LIB_H
#define DOUBLE_MATRIX_LIB_H

#include<stdbool.h>
#include<stdlib.h>

struct doubleMatrix
{
	int lines;
	int columns;
	void *elements;
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
DoubleMatrix* NewDoubleMatrix(int lines, int columns);
DoubleMatrix* NewDoubleMatrixAndInitializeElements(int lines, int columns, double defaultElement);
void DeleteDoubleMatrix(DoubleMatrix*);
DoubleMatrix* DoubleMatrixCopy(DoubleMatrix*);
DoubleMatrix* DoubleMatrixIdentity(int oneDimensionSize);
DoubleMatrix* DoubleMatrixAdd(DoubleMatrix*, DoubleMatrix*, bool resultInTheFirstMatrix);
DoubleMatrix* DoubleMatrixScalarMultiplication(DoubleMatrix *matrix, double scalar, bool resultInTheSameMatrix);
DoubleMatrix* DoubleMatrixTranspose(DoubleMatrix*, bool resultInTheSameMatrix);
DoubleMatrix* DoubleMatrixMultiplication(DoubleMatrix*, DoubleMatrix*, bool resultInTheFirstMatrix);
DoubleMatrix* DoubleMatrixCompare(DoubleMatrix*, DoubleMatrix*, Comparison);
DoubleMatrix* DoubleMatrixElementBinaryOperation(DoubleMatrix* , DoubleMatrix*, bool resultInTheFirstMatrix, double(*ElementBinFunction)(double element1, double element2));
DoubleMatrix* DoubleMatrixElementUnaryOperation(DoubleMatrix*, bool resultInTheFirstMatrix, double(*ElementUnaFunction)(double element));
double DoubleMatrixMaxElement(DoubleMatrix*);
double DoubleMatrixMinElement(DoubleMatrix*);
double DoubleMatrixGetElement(DoubleMatrix *,int line, int column);
void DoubleMatrixSetElement(int line, int column, double Element);
DoubleMatrix* DoubleMatrixKroneckerProduct(DoubleMatrix*, DoubleMatrix*, bool resultInTheFirstMatrix);
DoubleMatrix* DoubleMatrixElementDivision(DoubleMatrix*, DoubleMatrix*, bool resultInTheFirstMatrix);
int DoubleMatrixColumnsNumber(DoubleMatrix*);
int DoubleMatrixLinesNumber(DoubleMatrix*);
bool DoubleMatrixHaveSameDimensionsAndSameElementSize(DoubleMatrix*, DoubleMatrix*);

#endif
