#include "matrix.h"
#include <string.h>
#include <stdint.h>

Matrix* NewMatrix(int linesNumber, int columnsNumber, size_t elementSize)
{
	Matrix* ret= malloc(sizeof(Matrix));
	if(ret == NULL)
	{
		return NULL;
	}
	ret->isValid= true;
	ret->lines= linesNumber;
	ret->columns= columnsNumber;
	ret->singleElementSize= elementSize;
	ret->elements= malloc(linesNumber*columnsNumber*elementSize);
	if(ret->elements== NULL)
	{
		free(ret);
		return NULL;
	}
	return ret;
}

Matrix* NewMatrixAndInitializeElements(int lines, int columns, size_t elementSize, void * defaultElement)
{
	Matrix* ret= NewMatrix(lines, columns, elementSize);
	if(ret == NULL)
	{
		return NULL;
	}
	int cont, numberOfElements= lines*columns;
	void* aux= ret->elements;
	for(cont= 0; cont < numberOfElements; cont++)
	{
		memcpy(aux, defaultElement, elementSize);
		aux= ((uint8_t*)aux)+elementSize;
	}
	return ret;
}

void DeleteMatrix(Matrix* mat)
{
	mat->isValid= false;
	free(mat->elements);
	free(mat);
	mat= NULL;
}

Matrix* MatrixCopy(Matrix* origin)
{
	if(origin == NULL)
	{
		return NULL;
	}
	Matrix* copy= malloc(sizeof(Matrix));
	if(copy == NULL)
	{
		return NULL;
	}
	memcpy(copy, origin, sizeof(Matrix));
	int matrixMemorySize= (copy->columns)*(copy->lines)*(copy->singleElementSize);
	copy->elements= malloc(matrixMemorySize);
	if(copy->elements== NULL)
	{
		free(copy);
		return NULL;
	}
	memcpy(copy->elements, origin->elements, matrixMemorySize);
	return copy;
}

Matrix* MatrixIdentity(int oneDimensionSize, size_t elementSize, void* zero, void *one)
{
	Matrix* ret= NewMatrixAndInitializeElements(oneDimensionSize, oneDimensionSize, elementSize, zero);
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

bool MatrixHaveSameDimensionsAndSameElementSize(Matrix* m1, Matrix* m2)
{
	return ((m1->lines==m2->lines) && (m1-> columns== m2-> columns) && (m1->singleElementSize== m2->singleElementSize));
}

Matrix* MatrixAdd(Matrix* m1, Matrix* m2, bool resultInTheFirstMatrix, void (*ElementAddFunction)(void* destiny,void* firstOperator, void* secondOperator))
{
	if(!MatrixHaveSameDimensionsAndSameElementSize(m1, m2))
	{
		return NULL;
	}
	Matrix *ret;
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

Matrix* MatrixScalarMultiplication(void* scalar, Matrix *matrix, bool resultInTheSameMatrix, void(*ElementScalarMultFunction)(void *result,void* scalar, void* element))
{
	if(matrix == NULL)
	{
		return NULL;
	}
	Matrix* ret;
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

Matrix* MatrixTranspose(Matrix*, bool resultInTheSameMatrix)
{
	
}

