/*
MatrixLib, a general matrix libary
	Copyright (C) 2016  Francisco Anderson Bezerra Rodrigues

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DOUBLE_MATRIX_LIB_H
#define DOUBLE_MATRIX_LIB_H

#include<stdbool.h>
#include<stdlib.h>

struct doubleMatrix
{
	bool isValid;
	int lines;
	int columns;
	int singleElementSize;
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
DoubleMatrix* DoubleMatrixScalarMultiplication(double scalar, DoubleMatrix *matrix, bool resultInTheSameMatrix);
DoubleMatrix* DoubleMatrixTranspose(DoubleMatrix*, bool resultInTheSameMatrix);
DoubleMatrix* DoubleMatrixMultiplication(DoubleMatrix*, DoubleMatrix*, bool resultInTheFirstMatrix);
DoubleMatrix* DoubleMatrixCompare(DoubleMatrix*, DoubleMatrix*, Comparison);
DoubleMatrix* DoubleMatrixElementBinaryOperation(DoubleMatrix* , DoubleMatrix*, bool resultInTheFirstMatrix, double(*ElementBinFunction)(double element1, double element2));
DoubleMatrix* DoubleMatrixElementUnaryOperation(DoubleMatrix*, bool resultInTheFirstMatrix, double(*ElementUnaFunction)(double element));
double DoubleMatrixMaxElement(DoubleMatrix*);
double DoubleMatrixMinElement(DoubleMatrix*);
void* DoubleMatrixGetElement(int line, int column);
void DoubleMatrixSetElement(int line, int column, double Element);
DoubleMatrix* DoubleMatrixKroneckerProduct(DoubleMatrix*, DoubleMatrix*, bool resultInTheFirstMatrix);
DoubleMatrix* DoubleMatrixElementDivision(DoubleMatrix*, DoubleMatrix*, bool resultInTheFirstMatrix);
int DoubleMatrixColumnsNumber(DoubleMatrix*);
int DoubleMatrixLinesNumber(DoubleMatrix*);
bool DoubleMatrixHaveSameDimensionsAndSameElementSize(DoubleMatrix*, DoubleMatrix*);

#endif
