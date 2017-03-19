#ifndef GWR_LIB_H
#define GWR_LIB_H

#include "DoubleMatrixLib.h"
#include "FowardList.h"

enum kernelType
{
	FIXED_G,
	FIXED_BSQ,
	ADAPTIVE_N,
	ADAPTIVE_BSQ
};
typedef enum kernelType KernelType;

enum SelectionCriteria
{
	AICc,
	AIC,
	BIC_MDL,
	CV
};

struct goldenArguments
{
	FowardList *communication;
	DoubleMatrix *data;
	int *yVarColumn_dependentLocalVariables;//pergunta se é usar as variáveis locais ou globais
	int *yVarColumn_dependentGlobalVariables;
	int xVarColumn_independentVariable;
	int x_dCoord;
	int y_dCoord;
	KernelType method;
	bool distanceInKM;
	bool returnDistancesMatrix;
};
typedef struct goldenArguments GoldenArguments;

struct goldenDataIfNotAdpN//if method is not adaptive N
{
	double h1;
	double cv1;
	double h2;
	double cv2;
};
typedef struct goldenDataIfNotAdpN GoldenDataIfNotAdpN;

struct goldenDataIfAdpN//if method is adaptive N
{
	double xMin;
};
typedef struct goldenDataIfAdpN GoldenDataIfAdpN;

DoubleMatrix* LatPlusLon(DoubleMatrix *matrix, int latColumn, int lonColumn);//implemented
DoubleMatrix* DistanceToOrigin(DoubleMatrix* matrix, int column1, int column2);
double** DistanceBetweenAllPoints(DoubleMatrix* base, int yVarColumn, int xVarColumn, double* minDistOut, double *maxDistOut, bool returnOnlyMinAndMax);
void** Golden(GoldenArguments* args);//vai retornar a matriz de distâncias se for pedido, caso contrário retorna NULL

#endif
