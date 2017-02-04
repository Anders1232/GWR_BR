#include "gwr-lib.h"
#include <math.h>

#define APPROX_EARTH_RADIUS (6371)

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


double** DistanceBetweenAllPoints(DoubleMatrix* base, int yVarColumn, int xVarColumn, double* minDistOut, double *maxDistOut, bool returnOnlyMinAndMax)
{
	double** distances;
	if(!returnOnlyMinAndMax)
	{
		distances= (double**)malloc(base->lines * sizeof(double*));
		ASSERT(NULL != distances);
		int count, count2;
		for(count=0; count < base->lines; count++)
		{
			distances[count]= malloc( (count+1) *sizeof(double));
			ASSERT(NULL != distances[count]);
		}
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
	int count, count2;
	for(count=0; count < base->lines; count++)
	{
		for(count2= 0; count2<= count; count2++)
		{
			if(count2 == count)
			{
				if(!returnOnlyMinAndMax)
				{
					distances[count][count]= 0;
				}
			}
			else
			{
				temp= DistanceBetweenPoints(
							DoubleMatrixGetElement(base, count, xVarColumn),
							DoubleMatrixGetElement(base, count, yVarColumn),
							DoubleMatrixGetElement(base, count2, xVarColumn),
							DoubleMatrixGetElement(base, count2, yVarColumn)
							);
				if(!returnOnlyMinAndMax)
				{
					distances[count][count2]=temp;
				}
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
	if(!returnOnlyMinAndMax)
	{
		return distances;
	}
	else
	{
		return NULL;
	}
}

#define GOLDEN_PRONTO
#ifdef GOLDEN_PRONTO
DoubleMatrix* Golden(DoubleMatrix* base, int yVarColumn, int xVarColumn, int x_dCoord, int *y_dCoord, KernelType method, bool distanceInKM)
{
	DoubleMatrix *ret= NewDoubleMatrix(base->columns, base->lines);
	if(NULL == ret)
	{
		return NULL;
	}
	//calculando matriz de distâncias para apenas pegar a menor e a maior distância
	double minDist/*, medDist*/, maxDist;
	DistanceBetweenAllPoints(base, yVarColumn, xVarColumn, &minDist, &maxDist, true);
//	medDist=(maxDist + minDist)/2;

	double h0, h1, h2, h3;

	DoubleMatrix *x= NewDoubleMatrix(base->lines, 1);/**/
	DoubleMatrix *yhat= NewDoubleMatrix(base->lines, 1);
	if(ADAPTIVE_N == method)
	{
		double hv= 0;// como hv é uma matriz de 1x1, vou tratar como um double
//		yhat= NewDoubleMatrix(1, 1);	// o yhat é declarado na linha 47 como uma matrix coluna de n linhas e depois de declarado como uma matriz 1x1 de valor zero na linha 50 e depois não é mais usado
//aparentemente é usado nos CVs
		for(int i =1; i < base->lines; i++)
		{
			func1(minDist, maxDist, &h0, &h1, &h2, &h3);
		}
	}
	else
	{
		func1(minDist, maxDist, &h0, &h1, &h2, &h3);
	}
	//aqui tem o comando quit, o que ele deve fazer??
	return /* enfim chegamos no final da função, ver o que deve ser retornado*/;
}
#endif

#define F1_PRONTO
#ifdef F1_PRONTO
void func1(double min, double max, double *h0, double *h1, double *h2, double *h3)
{
	//dúvida: aparentenmente fazer isso em loop é inútil
	double ax, bx, cx, r, tol, c;
	ax= min;
	bx= (min+max)/2;
	cx= max;
	r = 0.61803399;
	tol = 0.001;
	c = 1-r;
	*h0= ax;
	*h3= cx;
	bx= c * (cx-ax);
	if(abs(cx-bx) > abs(bx-ax))
	{
		*h1= bx;
		*h2= bx-c*(bx-ax);
	}
	else
	{
		*h2=bx;
		*h1=bx-c*(bx-ax);
	}
	double cv1= CV1();
	double cv2= CV2();

	if(ADAPTIVE_N != method)
	{
		//Essa função deve retornar(não nesse momento, mas quando ela retornar) h1, cv1, h2, cv2
	}
	do
	{
		if(cv2< cv1)
		{
			h0= h1;
			h1= h2;
			h2= r*h1 + c*h3;
			cv1=cv2;
			cv2=CV2();
		}
		else
		{
			h3= h2;
			h2- h1;
			h1= r*h2 + c*h0;
			cv2=cv1;
			cv1= CV1();
		}
	}
	while(abs(h3-h0) > tol*(abs(h1)+abs(h2)));
	if(cv1 < cv2)
	{
		golden = cv1;
		xmin= h1;
		if(ADAPTIVE_BSQ == method)
		{
			xmin= floor(h1);//a função floor existe em math.h
		}
	}
	else
	{
		golden = cv2;
		xmin= h2;
		if(ADAPTIVE_BSQ == method)
		{
			xmin= floor(h2);//a função floor existe em math.h
		}
	}
	if(ADAPTIVE_N != method)
	{
		//no arquivo SAS está escrito "print golden xmin", deve-se colocar apenas um printf aqui??
		//Suponho que seja na verdade para que esse valor seja retornado quando essa função retornar
	}
	else if(ADAPTIVE_N == method)
	{
		hv= xmin;
		//aqui tem o comando "append from hv", perguntar o que isso faz
	}

}
#endif

#define CV_PRONTO
#ifdef CV_PRONTO

? cv(?, DoubleMatrix *data, bool distanceInKm, kernelType method)
{
	DoubleMatrix *d= NewDoubleMatrix(1, 3);
	if(ADAPTATIVE_N != METHOD)
	{
		int i, j;
		for(i=0; i < data->lines; i++)
		{//isolar em uma funçao 01
			for(j=0; j , data->lines; j++)
			{
				if(distanceInKm)
				{
					dif= abs(DoubleMatrixGetElement(data, i, 1) - DoubleMatrixGetElement(data, j, 1) );
					raio= raio=arcos(-1)/180;
					argument=
							sin(DoubleMatrixGetElement(data, i, 2)*raio)
							*sin(DoubleMatrixGetElement(data, j,2)*raio)
							+cos(DoubleMatrixGetElement(data, i,2)*raio)
							*cos(COORD[j,2]*raio)*cos(dif*raio);
					if(1 <= argument)
					{
						arco=0;
					}
					else//dúvida: onde realmente termina esse else? Estou supondo que na linha seguinte
					{
						//law of cosines
						arco =
								arcos(
									sin(DoubleMatrixGetElement(data, i,2)*raio)
									*sin(DoubleMatrixGetElement(data, j,2)*raio)
									+cos(DoubleMatrixGetElement(data, i,2)*raio)
									*cos(DoubleMatrixGetElement(data, j,2)*raio)
									*cos(dif*raio)
								);
					}
					dl= arco *APPROX_EARTH_RADIUS;
					if(0.001 >= dl)
					{
						dl=0;
					}
				}
				else
				{
					dl= sqrt(//isolar em uma função 01
								pow(
									DoubleMatrixGetElement(data, i,1)
									-DoubleMatrixGetElement(data, j,1)
									,2)
								+pow(
									DoubleMatrixGetElement(data,i,2)
									-DoubleMatrixGetElement(data,j,2)
									,2)
							);
				}
				if(
						(FIXED_G == method /* && d1 ^=0 */)
						|| ((FIXED_BSQ == method || ADAPTIVE_N == method) && d1 <= h1 * && d1 ^=0 */)
						||(ADAPTIVE_BSQ == method /* && d1 ^=0 */)
					)// perguntar o que é o "^="
				{
					cvAux2();
				}
				u= data->lines;
				DoubleMatrix* w= NewDoubleMatrix(u, 1);

			}
		}
	}
}

? cvAux2(bool distanceInKm)
{
	d[1]= i;
	d[2]= j;
	if(!distanceInKm)
	{
		d[3]= sqrt(//isolar em uma função 01
					pow(
						DoubleMatrixGetElement(data, i,1)
						-DoubleMatrixGetElement(data, j,1)
						,2)
					+pow(
						DoubleMatrixGetElement(data,i,2)
						-DoubleMatrixGetElement(data,j,2)
						,2);
				);
	}
	else
	{
		d[3]=arco*APPROX_EARTH_RADIUS;
	}
	//ver como fazer aqui, o código SAS aqui é "dist=dist//d;"
	// o "//" adiciona linha na tabela
}

#endif
