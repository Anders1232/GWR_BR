#include "gwr-lib.h"
#include <math.h>

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
#ifdef GOLDEN_PRONTO
DoubleMatrix* Golden(DoubleMatrix* base, int yVarColumn, int xVarColumn, ? dcoord, Method method, bool distanceInKM)
{
	DoubleMatrix *ret= NewDoubleMatrix(matrixA->columns, matrixA->lines);
	if(NULL == ret)
	{
		return NULL;
	}

	//otimizar isso aq
	//Calcular aq a distância mínima, média e máxima
	int count, count2, double minDist, medDist, maxDist;
	double temp= DistanceBetweenPoints(
				DoubleMatrixGetElement(base, 0, xVarColumn),
				DoubleMatrixGetElement(base, 0, yVarColumn),
				DoubleMatrixGetElement(base, 1, xVarColumn),
				DoubleMatrixGetElement(base, 1, yVarColumn),
				);
	minDist= maxDist = temp;
	for(count=0; count < base->lines-1; count++)
	{
		for(count2= count+1; count2< base->lines; count2++)
		{
			temp= DistanceBetweenPoints(
							DoubleMatrixGetElement(base, count, xVarColumn),
							DoubleMatrixGetElement(base, count, yVarColumn),
							DoubleMatrixGetElement(base, count2, xVarColumn),
							DoubleMatrixGetElement(base, count2, yVarColumn),
							);
			if(temp > maxDist)
			{
				maxDist= temp;
			}
			if(temp < minDist)
			{
				minDist= temp;
			}
		}
	}
	medDist=(maxDist + minDist)/2;



	/*
	DoubleMatrix* wt;
	%IF &WEIGHT NE %THEN %DO;		De onde veio esse weight?
	{
		wt= weight;
	}
	else
	{
		wt= NewDoubleMatrix(base->lines, 1);
	}
	*/
	DoubleMatrix *x= NewDoubleMatrix(base->lines, 1);/**/
	DoubleMatrix *yhat= NewDoubleMatrix(base->lines, 1);
	if(ADAPTATIVE_N == method)
	{
		hv= NewDoubleMatrix(1, 1);
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
/*
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
	if(abs(cx=bx) > abs(bx-ax))
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
*/
/*
? cv1(?)
{
	if(ADAPTATIVE_N != METHOD)
	{
		int i;
		for(i=1; i < base->lines; i++)
		{
			/* bla*/
		}
	}
}

*/

#endif
