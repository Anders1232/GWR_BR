#ifndef GWR_LIB_H
#define GWR_LIB_H

#include "DoubleMatrixLib.h"

DoubleMatrix* LatPlusLon(DoubleMatrix *matrix, int latColumn, int lonColumn);//implemented
DoubleMatrix* DistanceToOrigin(DoubleMatrix* matrix, int column1, int column2);


#endif
