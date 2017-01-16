#ifndef GWR_LIB_H
#define GWR_LIB_H

#include "DoubleMatrixLib.h"

enum KernelType
{
	FIXED_G,
	FIXES_BSQ,
	ADAPTIVE_N,
	ADAPTIVE_BSQ
};

enum SelectionCriteria
{
	AICc,
	AIC,
	BIC_MDL,
	CV
};

DoubleMatrix* LatPlusLon(DoubleMatrix *matrix, int latColumn, int lonColumn);//implemented
DoubleMatrix* DistanceToOrigin(DoubleMatrix* matrix, int column1, int column2);
double** DistanceBetweenAllPoints(DoubleMatrix* base, int yVarColumn, int xVarColumn, double* minDistOut, double *maxDistOut);
/*DoubleMatrix* Golden(DoubleMatrix* base, int yVarColumn, int xVarColumn, ? dcoord, KernelType method, bool distanceInKM);
*/
#endif
