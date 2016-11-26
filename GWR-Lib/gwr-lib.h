#ifndef GWR_LIB_H
#define GWR_LIB_H

#include "DoubleMatrixLib.h"

enum Method
{
	FIXED_G,
	FIXES_BSQ,
	ADAPTATIVE_N,
	ADAPTATIVE_BSQ
};

DoubleMatrix* LatPlusLon(DoubleMatrix *matrix, int latColumn, int lonColumn);//implemented
DoubleMatrix* DistanceToOrigin(DoubleMatrix* matrix, int column1, int column2);
/*DoubleMatrix* Golden(DoubleMatrix* base, int yVarColumn, int xVarColumn, ? dcoord, Method method, bool distanceInKM);
*/
#endif
