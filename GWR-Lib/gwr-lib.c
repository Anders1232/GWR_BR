#include "gwr-lib.h"
#include <math.h>

#define ASSERT(X)\
	if( !(X) )\
	{\
		printf("Failed Assertion at %s:%d", __FILE__, __LINE__);\
		exit(1);\
	}

static double Pythagoras(double x, double y)
{
	return sqrt(x*x+y*y);
}

static double DistanceBetweenPoints(double x1, double y1, double x2, double y2)
{
	return abs( Pythagoras((x2-x1), (y2-y1)) );
}

static double Add(double a, double b)
{
	return a+b;
}


DoubleMatrix* LatPlusLon(DoubleMatrix *matrix, int latColumn, int lonColumn)
{
	int linesNumber= matrix->lines;
	DoubleMatrix *latColumnMatrix= NewDoubleMatrix(linesNumber, 1);
	DoubleMatrix *lonColumnMatrix= NewDoubleMatrix(linesNumber, 1);
	int count;
	for(count =0; count < linesNumber; count++)
	{
		DoubleMatrixSetElement(latColumnMatrix, 0, count, DoubleMatrixGetElement(matrix, latColumn, count));
		DoubleMatrixSetElement(lonColumnMatrix, 0, count, DoubleMatrixGetElement(matrix, lonColumn, count));
	}
	DoubleMatrix *result = DoubleMatrixElementBinaryOperation(latColumnMatrix, lonColumnMatrix, false, Add);
//	DoubleMatrixPrint(result, temp, "\t%lf", NEW_LINE);
	DeleteDoubleMatrix(latColumnMatrix);
	DeleteDoubleMatrix(lonColumnMatrix);
	return result;
}

DoubleMatrix* DistanceToOrigin(DoubleMatrix *matrix, int latColumn, int lonColumn)
{
	int linesNumber= matrix->lines;
	DoubleMatrix *latColumnMatrix= NewDoubleMatrix(linesNumber, 1);
	DoubleMatrix *lonColumnMatrix= NewDoubleMatrix(linesNumber, 1);
	int count;
	for(count =0; count < linesNumber; count++)
	{
		DoubleMatrixSetElement(latColumnMatrix, 0, count, DoubleMatrixGetElement(matrix, latColumn, count));
		DoubleMatrixSetElement(lonColumnMatrix, 0, count, DoubleMatrixGetElement(matrix, lonColumn, count));
	}
	DoubleMatrix *result = DoubleMatrixElementBinaryOperation(latColumnMatrix, lonColumnMatrix, false, Pythagoras);
//	DoubleMatrixPrint(result, temp, "\t%lf", NEW_LINE);
	DeleteDoubleMatrix(latColumnMatrix);
	DeleteDoubleMatrix(lonColumnMatrix);
	return result;
}

unsigned long int BinomialCoefficient(unsigned long int n, unsigned long int k)
{
	unsigned long int i, res=0;
	for(i=1; i <= k; i++)
	{
		res+= (n+1-i)/i;
	}
	return res;
}


double** DistanceBetweenAllPoints(DoubleMatrix* base, int yVarColumn, int xVarColumn, double* minDistOut, double *maxDistOut)
{
	double **distances= malloc(base->lines * sizeof(double*));
	ASSERT(NULL != distances);
	int count, count2;
	for(count=0; count < base->lines; count++)
	{
		distances[count]= malloc( (count+1) *sizeof(double));
		ASSERT(NULL != distances[count]);
	}
	//otimizar isso aq
	//Calcular aq a distância mínima, média e máxima
	double temp= DistanceBetweenPoints(
				DoubleMatrixGetElement(base, 0, xVarColumn),
				DoubleMatrixGetElement(base, 0, yVarColumn),
				DoubleMatrixGetElement(base, 1, xVarColumn),
				DoubleMatrixGetElement(base, 1, yVarColumn)
				);
	if(NULL != minDistOut)
	{
		*minDistOut= temp;
	}
	if(NULL != maxDistOut)
	{
		*maxDistOut= temp;
	}
	for(count=0; count < base->lines; count++)
	{
		for(count2= 0; count2<= count; count2++)
		{
			if(count2 == count)
			{
				distances[count][count]= 0;
			}
			else
			{
				distances[count][count2]= DistanceBetweenPoints(
							DoubleMatrixGetElement(base, count, xVarColumn),
							DoubleMatrixGetElement(base, count, yVarColumn),
							DoubleMatrixGetElement(base, count2, xVarColumn),
							DoubleMatrixGetElement(base, count2, yVarColumn)
							);
				if(NULL != minDistOut)
				{
					if(temp < *minDistOut)
					{
						*minDistOut= temp;
					}

				}
				if(NULL != maxDistOut)
				{
					if(temp > *maxDistOut)
					{
						*maxDistOut= temp;
					}
				}
			}
		}
	}
	return distances;
}

#ifdef GOLDEN_PRONTO
DoubleMatrix* Golden(DoubleMatrix* base, int yVarColumn, int xVarColumn, int x_dCoord, int *y_dCoord, Method method, bool distanceInKM)
{
	DoubleMatrix *ret= NewDoubleMatrix(matrixA->columns, matrixA->lines);
	if(NULL == ret)
	{
		return NULL;
	}
	//calculando matriz de distâncias
	double minDist, medDist, maxDist;

	medDist=(maxDist + minDist)/2;



	DoubleMatrix *x= NewDoubleMatrix(base->lines, 1);/**/
	DoubleMatrix *yhat= NewDoubleMatrix(base->lines, 1);
	if(ADAPTIVE_N == method)
	{
//		hv= NewDoubleMatrix(1, 1);
		yhat= NewDoubleMatrix(1, 1);
		for(int i =1; i < base->lines; i++)
		{
//			func1();
		}
	}
	else
	{
//		func1();
	}
}
///*
? func1(double min, double mid, double max)
{
	double ax, bx, cx, r, tol, c, h0, h3;
	ax= min;
	bx= mid;
	cx= max;
	r = 0.61803399;
	tol = 0.001;
	c = 1-r;
	h0= ax;
	h3= cx;
	bx= c * (cx-ax);
	if(abs(cx-bx) > abs(bx-ax))
	{
		h1= bx;
		h2= bx - c (bx - ax);
	}
	else
	{
		h2=bx;
		h1=bx-c*(bx-ax);
	}
}
//*/
///*
? cv(?, DoubleMatrix *data, bool distanceInKm)
{
	DoubleMatrix *d= NewDoubleMatrix(1, 3);
	if(ADAPTATIVE_N != METHOD)
	{
		int i, j;
		for(i=0; i < base->lines; i++)
		{
			for(j=0; j , base->lines; j++)
			{
				if(distanceInKm)
				{
					dif= DistanceBetweenPoints(
								DoubleMatrixGetElement(data, i, ?),
								DoubleMatrixGetElement(data, ?, ?),
								DoubleMatrixGetElement(data, j, ?),
								DoubleMatrixGetElement(data, ?, ?),
					);
					double raio= raio=arcos(-1)/180;
					double argument=sin(COORD[i,2]*raio)*sin(COORD[j,2]*raio)+cos(COORD[i,2]*raio)*cos(COORD[j,2]*raio)*cos(dif*raio);
					if(1 <= argument)
					{
						double arco=0;
					}
				}
				else
				{
					//law of cosines
					double arco =
				}
			}
		}
	}
}

//*/

#endif
