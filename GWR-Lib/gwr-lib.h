#ifndef GWR_LIB_H
#define GWR_LIB_H

#include "DoubleMatrixLib.h"
#include "FowardList.h"

//#define DEBUG_MATRIX_DIMENSIONS

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
	int *yVarColumn_independentLocalVariables;//pergunta se é usar as variáveis locais ou globais
	int *yVarColumn_independentGlobalVariables;
	int xVarColumn_dependentVariable;
	int x_dCoord;
	int y_dCoord;
	KernelType method;
	bool distanceInKM;
	bool returnDistancesMatrix;
	double *outMaxDistanceBetweenPoints;
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
	double cv1;
	double cv2;
};
typedef struct goldenDataIfAdpN GoldenDataIfAdpN;

struct GWRArguments_
{
	FowardList *communication;
	DoubleMatrix *data;
	int *yVarColumn_independentLocalVariables;//pergunta se é usar as variáveis locais ou globais
//	int *yVarColumn_independentGlobalVariables;
	int xVarColumn_dependentVariable;
	int x_dCoord;
	int y_dCoord;
	KernelType method;
	bool distanceInKm;
	double maxDistanceBetweenPoints;
//	bool returnDistancesMatrix;
	double h;
};
typedef struct GWRArguments_ GWRArguments;
/*
struct GoldenBetaData_
{
	
}*/


DoubleMatrix* LatPlusLon(DoubleMatrix *matrix, int latColumn, int lonColumn);//implemented
DoubleMatrix* DistanceToOrigin(DoubleMatrix* matrix, int column1, int column2);
double** DistanceBetweenAllPoints(DoubleMatrix* base, int yVarColumn, int xVarColumn, double* minDistOut, double *maxDistOut, bool returnOnlyMinAndMax);
//obs: o argumento e o tipo de retorno são void para permintir que essa função seja chamada no pthread_create,
//mas na verdade o argumento é um ponteiro para GoldenArguments e o tipo de retorno é um ponteiro de ponteiro para void
void* Golden(void* args);//vai retornar a matriz de distâncias se for pedido, caso contrário retorna NULL
double GWR_Determinant(DoubleMatrix *m);
void* GWR(void *args_);

#endif
