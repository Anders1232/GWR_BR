#include "DoubleMatrixLib.h"
#include <string.h>
#include <stdint.h>

DoubleMatrix* NewDoubleMatrix(int linesNumber, int columnsNumber)
{
	DoubleMatrix* ret= malloc(sizeof(DoubleMatrix));
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

DoubleMatrix* NewDoubleMatrixAndInitializeElements(int lines, int columns, double defaultElement)
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

void DeleteDoubleMatrix(DoubleMatrix* mat)
{
	free(mat->elements);
	free(mat);
//	mat= NULL;
}

DoubleMatrix* DoubleMatrixCopy(DoubleMatrix* origin)
{
	if(NULL == origin)
	{
		return NULL;
	}
	DoubleMatrix* copy= malloc(sizeof(DoubleMatrix));
	if(NULL == copy)
	{
		return NULL;
	}
	memcpy(copy, origin, sizeof(DoubleMatrix));
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
DoubleMatrix* DoubleMatrixIdentity(int oneDimensionSize)
{
	DoubleMatrix* ret= NewDoubleMatrixAndInitializeElements(oneDimensionSize, oneDimensionSize, 0.0);
	int cont;
	int nextPosition= oneDimensionSize+1;
	double* aux= ret->elements;
	for(cont=0; cont < oneDimensionSize; cont++)
	{
		aux[cont] = 1.0;
		cont+= nextPosition;
	}
	return ret;
}

bool DoubleMatrixHaveSameDimensionsAndSameElementSize(DoubleMatrix* m1, DoubleMatrix* m2)
{
	return ((m1->lines==m2->lines) && (m1-> columns== m2-> columns));
}

DoubleMatrix* DoubleMatrixAdd(DoubleMatrix* m1, DoubleMatrix* m2, bool resultInTheFirstMatrix)
{
	if(!DoubleMatrixHaveSameDimensionsAndSameElementSize(m1, m2))
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
		ret= NewDoubleMatrix(m1->lines, m1->columns);
		if(NULL == ret)
		{
			return NULL;
		}
	}
	double *aux1, *aux2, *aux3;
	aux1= m1->elements;
	aux2= m2->elements;
	aux3= ret->elements;
	int cont, numberOfElements= (m1->columns)*(m1->lines);
	for(cont=0; cont < numberOfElements; cont++)
	{
		aux3[cont]= aux1[cont] + aux2[cont];
	}
	return ret;
}

DoubleMatrix* DoubleMatrixScalarMultiplication(DoubleMatrix *matrix, double scalar, bool resultInTheSameMatrix)
{
	if(NULL == matrix)
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
		ret= NewDoubleMatrix(matrix->lines, matrix->columns);
		if(NULL == ret)
		{
			return NULL;
		}
	}
	double *aux1, *aux2;
	aux1= matrix->elements;
	aux2= ret->elements;
	int cont, numberOfElements;
	numberOfElements= (matrix->columns)*(matrix->lines);
	for(cont=0; cont < numberOfElements; cont++)
	{
		aux2[cont] = scalar * (aux1[cont]);
	}
	return ret;
}


double DoubleMatrixGetElement(DoubleMatrix *matrix,int line, int column)
{
	int position = (matrix->columns)*line+column;
	return (matrix->elements)[position];
}



DoubleMatrix* DoubleMatrixTranspose(DoubleMatrix *matrix, bool resultInTheSameMatrix)
{
	if(NULL == matrix)
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
		ret= NewDoubleMatrix(matrix->columns, matrix->lines);
		if(NULL == ret)
		{
			return NULL;
		}
	}
	int lines, columns, counter;
	lines= matrix->lines;
	columns= matrix->columns;
	double *temp= malloc(lines * columns * sizeof(double));
	counter= 0;
	int cont1, cont2;
	for(cont1=0; cont1 < columns; cont1++)
	{
		for(cont2=0; cont2 < lines; cont2++)
		{
#ifdef DEBUG
printf("Inserting element [%d][%d] in position %d\n", cont1, cont2, counter);
#endif
			temp[counter++]= DoubleMatrixGetElement(matrix, cont1, cont2);
		}
	}
	free(ret->elements);
	ret->elements= temp;
	return ret;
}

void DoubleMatrixPrint(DoubleMatrix* matrix, FILE *output, char *doubleFormat, char *betweenLinesFormat)
{
	int cont1, cont2, counter=0;
	int lines= matrix->lines;
	int columns= matrix->columns;
	double* aux= matrix->elements;
	for(cont1=0; cont1 < lines; cont1++)
	{
		for(cont2=0; cont2 < columns; cont2++)
		{
			fprintf(output, doubleFormat, aux[counter++]);
		}
		fprintf(output, betweenLinesFormat);
	}
}

