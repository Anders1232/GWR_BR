#include "matrix.h"
#include <string.h>
#include <stdint.h>

DoubleMatrix* NewDoubleMatrix(int linesNumber, int columnsNumber);
{
	DoubleMatrix* ret= malloc(sizeof(Matrix));
	if(NULL == ret)
	{
		return NULL;
	}
	ret->lines= linesNumber;
	ret->columns= columnsNumber;
	ret->elements= malloc(linesNumber*columnsNumber*sizeof(double));
	if(NULL == ret->elements)
	{
		free(ret);
		return NULL;
	}
	return ret;
}

DoubleMatrix* NewDoubleMatrixAndInitializeElements(int lines, int columns, double defaultElement);
{
	DoubleMatrix* ret= NewDoubleMatrix(lines, columns);
	if(NULL == ret)
	{
		return NULL;
	}
	int cont, numberOfElements= lines*columns;
	double* aux= ret->elements;
	for(cont= 0; cont < numberOfElements; cont++)
	{
		aux[cont]=defaultElement;
	}
	return ret;
}

void DeleteMatrix(DoubleMatrix* mat)
{
	free(mat->elements);
	free(mat);
//	mat= NULL;
}

DoubleMatrix* MatrixCopy(DoubleMatrix* origin)
{
	if(NULL == origin)
	{
		return NULL;
	}
	DoubleMatrix* copy= malloc(sizeof(Matrix));
	if(NULL == copy)
	{
		return NULL;
	}
	memcpy(copy, origin, sizeof(Matrix));
	int matrixMemorySize= (copy->columns)*(copy->lines)*sizeof(double);
	copy->elements= malloc(matrixMemorySize);
	if(NULL == copy->elements)
	{
		free(copy);
		return NULL;
	}
	memcpy(copy->elements, origin->elements, matrixMemorySize);
	return copy;
}
//continuar daqui
DoubleMatrix* MatrixIdentity(int oneDimensionSize, size_t elementSize, void* zero, void *one)
{
	DoubleMatrix* ret= NewMatrixAndInitializeElements(oneDimensionSize, oneDimensionSize, elementSize, zero);
	int cont;
	int nextPositionOffset= (elementSize*oneDimensionSize)+elementSize;
	void* aux= ret->elements;
	for(cont=0; cont < oneDimensionSize; cont++)
	{
		memcpy(aux, one, elementSize);
		aux= ((uint8_t*)aux)+nextPositionOffset;
	}
	return ret;
}

bool MatrixHaveSameDimensionsAndSameElementSize(DoubleMatrix* m1, DoubleMatrix* m2)
{
	return ((m1->lines==m2->lines) && (m1-> columns== m2-> columns) && (m1->singleElementSize== m2->singleElementSize));
}

DoubleMatrix* MatrixAdd(DoubleMatrix* m1, DoubleMatrix* m2, bool resultInTheFirstMatrix, void (*ElementAddFunction)(void* destiny,void* firstOperator, void* secondOperator))
{
	if(!MatrixHaveSameDimensionsAndSameElementSize(m1, m2))
	{
		return NULL;
	}
	DoubleMatrix *ret;
	if(resultInTheFirstMatrix)
	{
		ret= m1;
	}
	else
	{
		ret= NewMatrix(m1->lines, m1->columns, m1-> singleElementSize);
		if(ret== NULL)
		{
			return NULL;
		}
	}
	void *aux1, *aux2, *aux3;
	aux1= m1->elements;
	aux2= m2->elements;
	aux3= ret->elements;
	int cont, numberOfElements= (m1->columns)*(m1->lines);
	int elementSize= m1->singleElementSize;
	for(cont=0; cont < numberOfElements; cont++)
	{
		(*ElementAddFunction)(aux3, aux1, aux2);
		aux1= ((uint8_t*)aux1)+elementSize;
		aux2= ((uint8_t*)aux2)+elementSize;
		aux3= ((uint8_t*)aux3)+elementSize;
	}
	return ret;
}

DoubleMatrix* MatrixScalarMultiplication(void* scalar, DoubleMatrix *matrix, bool resultInTheSameMatrix, void(*ElementScalarMultFunction)(void *result,void* scalar, void* element))
{
	if(matrix == NULL)
	{
		return NULL;
	}
	DoubleMatrix* ret;
	if(resultInTheSameMatrix)
	{
		ret= matrix;
	}
	else
	{
		ret= NewMatrix(matrix->lines, matrix->columns, matrix-> singleElementSize);
		if(ret== NULL)
		{
			return NULL;
		}
	}
	void *aux1, *aux2;
	aux1= matrix->elements;
	aux2= ret->elements;
	int cont, numberOfElements;
	numberOfElements= (matrix->columns)*(matrix->lines);
	int elementSize= matrix->singleElementSize;
	for(cont=0; cont < numberOfElements; cont++)
	{
		(*ElementScalarMultFunction)(aux2, scalar, aux1);
		aux1= ((uint8_t*)aux1)+elementSize;
		aux2= ((uint8_t*)aux2)+elementSize;
	}
	return ret;
}

DoubleMatrix* MatrixTranspose(DoubleMatrix*, bool resultInTheSameMatrix)
{
	
}

