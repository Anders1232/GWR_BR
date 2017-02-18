#include "DoubleMatrixLib.h"
#include <string.h>
#include <stdint.h>
#include <math.h>

//#define DEBUG_DOUBLE_MATRIX

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


double DoubleMatrixGetElement(DoubleMatrix *matrix, int line, int column)
{
	int position = (matrix->columns)*line+column;
	return (matrix->elements)[position];
}

void DoubleMatrixSetElement(DoubleMatrix *matrix, int line, int column, double element)
{
	int position = (matrix->columns)*line+column;
	(matrix->elements)[position]= element;
}

DoubleMatrix* DoubleMatrixTranspose(DoubleMatrix *matrix, bool resultInTheSameMatrix)
{
	if(NULL == matrix)
	{
		return NULL;
	}
	
/*	DoubleMatrix* ret;
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
	}*/
	int lines, columns, counter;
	lines= matrix->lines;
	columns= matrix->columns;
	DoubleMatrix *temp= NewDoubleMatrix(columns, lines);
	counter= 0;
	int count1, count2;
	for(count1=0; count1 < columns; count1++)
	{
		for(count2=0; count2 < lines; count2++)
		{
			DoubleMatrixSetElement(
									temp,
									count1,
									count2,
									DoubleMatrixGetElement(
										matrix,
										count2,
										count1
									)
								);
		}
	}
	if(!resultInTheSameMatrix)
	{
		return temp;
	}
	else
	{
		matrix->lines= columns;
		matrix->columns= lines;
		free(matrix->elements);
		matrix->elements= temp->elements;
		free(temp);
		return matrix;
	}
}

void DoubleMatrixPrint(DoubleMatrix* matrix, FILE *output, const char *doubleFormat, const char *betweenLinesFormat)
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
#ifdef DEBUG_DOUBLE_MATRIX
			printf("verification: %p contains %lf\n", &(aux[counter-1]), aux[counter-1]);
#endif
		}
		fprintf(output, betweenLinesFormat);
	}
}

DoubleMatrix* DoubleMatrixElementBinaryOperation(DoubleMatrix* matrixA, DoubleMatrix*matrixB, bool resultInTheFirstMatrix, double(*ElementBinFunction)(double element1, double element2))
{
	DoubleMatrix* ret;
	if(!DoubleMatrixHaveSameDimensionsAndSameElementSize(matrixA, matrixB))
	{
		fprintf(stderr, "[ERROR]Operation not possible.\r\n");
		exit(1);
	}
	if(resultInTheFirstMatrix)
	{
		ret= matrixA;
	}
	else
	{
		ret= NewDoubleMatrix(matrixA->columns, matrixA->lines);
		if(NULL == ret)
		{
			return NULL;
		}
	}
	int numberOfElements= (matrixA->lines) * (matrixB->columns);
	double *p1= matrixA->elements;
	double *p2= matrixB->elements;
	double *p3= ret->elements;
	int count;
	for(count =0; count < numberOfElements; count++)
	{
		*p3= ElementBinFunction(*p1, *p2);
		p1++;
		p2++;
		p3++;
	}
	return ret;
}

void DoubleMatrixAddLine(DoubleMatrix *matrix)
{
	void* result= realloc(matrix->elements, ((matrix->lines)+1)*(matrix->columns)*sizeof(double));
	if(NULL == result)
	{
		fprintf(stderr, "[ERROR] Reallocation error!");
		exit(-1);
	}
	else
	{
		matrix->elements= result;
		matrix->lines= (matrix->lines)+1;
	}
}
void DoubleMatrixAddColumn(DoubleMatrix *matrix)
{
	void* result= realloc(matrix->elements, (matrix->lines)*((matrix->columns)+1)*sizeof(double));
	if(NULL == result)
	{
		fprintf(stderr, "[ERROR] Reallocation error!");
		exit(-1);
	}
	else
	{
		int count;
		int nColumnsBefore= matrix->columns;
		int nColumnsAfter= nColumnsBefore+1;
		double *v= matrix->elements;
		for(count= matrix->lines -1; count>0; count--)
		{
			memmove(&(v[nColumnsAfter*count]), &(v[nColumnsBefore*count]), nColumnsBefore*sizeof(double));
		}
	}
}
DoubleMatrix* NewLineDoubleMatrixFromMatrix(DoubleMatrix *origin, int lineFromOrigin)
{
	DoubleMatrix *ret;
	ret= NewDoubleMatrix(origin->columns, 1);
	if(NULL == ret)
	{
		return NULL;
	}
	memcpy(ret->elements, &(origin->elements[origin->columns * lineFromOrigin]), origin->columns * sizeof(double) );
	return ret;
}
DoubleMatrix* NewColumnDoubleMatrixFromMatrix(DoubleMatrix *origin, int columnFromOrigin)
{
	DoubleMatrix *ret;
	ret= NewDoubleMatrix(origin->columns, 1);
	if(NULL == ret)
	{
		return NULL;
	}
	int count;
	for(count=0; count < origin->lines; count++)
	{
		DoubleMatrixSetElement(ret, count, 0, DoubleMatrixGetElement(origin, count, columnFromOrigin));
	}
	return ret;
}

//#define DEBUG_DETERMINANT

double DoubleMatrixDeterminant(DoubleMatrix *mat)
{
	if(mat->lines != mat->columns)
	{
		fprintf(stderr, "%s:%d\tFunction:%s\t\tError: Not a sqared matrix.\n", __FILE__, __LINE__, __FUNCTION__);
	}
	double result=0, temp;
	int count, count2, aux;
	for(count=0; count < mat->lines; count++)
	{
		temp=1;
		for(count2=0; count2 < mat->lines; count2++)//diagonal principal
		{
			temp*= DoubleMatrixGetElement(mat, count2, (count+count2)%(mat->lines) );
#ifdef DEBUG_DETERMINANT
			printf("[incremental]temp *= mat[%d][%d]\n", count2, (count+count2)%(mat->lines));
#endif
		}
#ifdef DEBUG_DETERMINANT
		printf("temp = %lf\n", temp);
#endif
		result+=temp;
		temp=1;
		for(count2=0; count2 < mat->lines; count2++)//diagonal secundária
		{
			aux= (count-count2)%(mat->lines);
			if(0 > aux)
			{
				aux+= mat->lines;
			}
			temp*= DoubleMatrixGetElement(mat, count2, aux);
#ifdef DEBUG_DETERMINANT
			printf("[decremental]temp *= mat[%d][%d](%lf)\n", count2, aux, DoubleMatrixGetElement(mat, count2, aux));
#endif
		}
#ifdef DEBUG_DETERMINANT
		printf("temp = %lf\n", temp);
#endif
		result-=temp;
	}
	return result;
}

DoubleMatrix *DoubleMatrixInverse(DoubleMatrix *matrix)
{
	if(matrix->columns != matrix->lines)
	{
		fprintf(stderr, "%s:%d\t\tCannot inverve non squared matrix!\n", __FILE__, __LINE__);
	}
	DoubleMatrix *ret= NewDoubleMatrix(matrix->columns, matrix->columns);
	if(NULL == ret)
	{
		fprintf(stderr, "%s:%s:%d\t\t Error allocating matrix.\n", __FILE__, __func__, __LINE__);
		return NULL;
	}
	int i, j, n=matrix->columns, k;
	double ratio, a;
	for(i = 0; i < n; i++)
	{
		for(j = n; j < 2*n; j++)
		{
			if(i==(j-n))
			{
				DoubleMatrixSetElement(ret, i, j, 1.0);
			}
			else
			{
				DoubleMatrixSetElement(ret, i, j, 0.0);
			}
		}
	}
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			if(i!=j)
			{
				ratio = DoubleMatrixGetElement(ret, j, i)/DoubleMatrixGetElement(ret, i, i);
				for(k = 0; k < 2*n; k++)
				{
//					matrix[j][k] -= ratio * matrix[i][k];
					DoubleMatrixSetElement(ret, j, k, DoubleMatrixGetElement(ret, j, k)-ratio*DoubleMatrixGetElement(ret, i, k));
				}
			}
		}
	}
	for(i = 0; i < n; i++)
	{
//		a = matrix[i][i];
		a= DoubleMatrixGetElement(ret, i, i);
		for(j = 0; j < 2*n; j++)
		{
//			matrix[i][j] /= a;
			DoubleMatrixSetElement(ret, i, j, DoubleMatrixGetElement(ret, i, j)/a);
		}
	}
	return ret;
}

//#define DEBUG_MATRIX_MULT

DoubleMatrix* DoubleMatrixMultiplication(DoubleMatrix *a, DoubleMatrix *b)
{
	DoubleMatrix* ret;
	if(a->columns != b->lines)
	{
		fprintf(stderr, "%s:%s:%d\t\tMatrices not compatible for multiplication.\n", __func__, __FILE__, __LINE__);
		return NULL;
	}
#ifdef DEBUG_MATRIX_MULT
	printf("'a'' is %dx%d and b is %dx%d\n", a->lines, a->columns, b->lines, b->columns);
	printf("ret is %dx%d\n", a->lines, b->columns);
#endif
	ret= NewDoubleMatrix(a->lines, b->columns);
	if(NULL == ret)
	{
#ifdef DEBUG_MATRIX_MULT
	printf("Chegou aqui! \t%s:%d\n", __FILE__, __LINE__);
#endif
		return NULL;
	}
#ifdef DEBUG_MATRIX_MULT
	printf("Chegou aqui! \t%s:%d\n", __FILE__, __LINE__);
#endif
	int count1, count2, count3;
	for(count1=0; count1 < ret->lines; count1++)
	{
		for(count2=0; count2 < ret->columns; count2++)
		{
			double sum=0;
			for(count3=0; count3 < a->columns; count3++)
			{
#ifdef DEBUG_MATRIX_MULT
				printf("a[%d][%d]*b[%d][%d]\n", count1, count3, count3, count2);
#endif
				sum+= DoubleMatrixGetElement(a, count1, count3)*DoubleMatrixGetElement(b, count3, count2);
			}
#ifdef DEBUG_MATRIX_MULT
			printf("ret[%d][%d]= %lf\n", count1, count2, sum);
#endif
			DoubleMatrixSetElement(ret, count1, count2, sum);
		}
	}
	return ret;
}
