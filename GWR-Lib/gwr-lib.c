#include "gwr-lib.h"
#include <math.h>
#include "string.h"

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
	double result= Pythagoras((x2-x1), (y2-y1));
	return (result>0)? result: -result;
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
		int count;
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

static double CrossValidation(DoubleMatrix *data, int oneOrTwo, bool distanceInKm, KernelType method, int i, int xCoord, int yCoord);//aqui temos um problema, estava supondo que era pra ignorar o wt, mas ele é necessariamente utilizado no cálculo do cv.

#define F1_PRONTO
#ifdef F1_PRONTO
static void func1(double min, double max, double *h0, double *h1, double *h2, double *h3, double *hv, DoubleMatrix *yhat, GoldenArguments *args, DoubleMatrix *x, DoubleMatrix *y, double *outGolden, double *outXMin)//de acordo com o código SAS o outGolden e outXMin só são retornados(printados) caso o método não seja adaptive N, mas como em todos os casos essas variáveis existem, estou retornando-as
{
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
	double cv1= CrossValidation();
	double cv2= CrossValidation();

	if(ADAPTIVE_N != args->method)
	{
		//Essa função deve retornar(não nesse momento, mas quando ela retornar) h1, cv1, h2, cv2
		GoldenDataIfNotAdpN *dataToReturn= malloc(sizeof(GoldenDataIfNotAdpN));//isolar numa função 3
		ASSERT(dataToReturn);
		dataToReturn->cv1= cv1;
		dataToReturn->cv2= cv2;
		dataToReturn-> h1= *h1;
		dataToReturn->h2= *h2;
		FowardListAddElement(args->communication, dataToReturn);
	}
	do
	{
		if(cv2< cv1)
		{
			*h0= *h1;
			*h1= *h2;
			*h2= r*(*h1) + c*(*h3);
			cv1= CrossValidation();
			cv2= CrossValidation();
		}
		else
		{
			*h3= *h2;
			*h2= *h1;
			*h1= r*(*h2) + c*(*h0);
			cv2= CrossValidation();
			cv1= CrossValidation();
		}
		if(ADAPTIVE_N != args->method)
		{
			GoldenDataIfNotAdpN *dataToReturn= malloc(sizeof(GoldenDataIfNotAdpN));//isolar numa função 3
			ASSERT(dataToReturn);
			dataToReturn->cv1= cv1;
			dataToReturn->cv2= cv2;
			dataToReturn-> h1= *h1;
			dataToReturn->h2= *h2;
			FowardListAddElement(args->communication, dataToReturn);
		}
	}
	while(abs(*h3-*h0) > tol*(abs(*h1)+abs(*h2)));
	if(cv1 < cv2)
	{
		*outGolden = cv1;
		*outXMin= *h1;
		if(ADAPTIVE_BSQ == args->method)
		{
			*outXMin= floor(*h1);//a função floor existe em math.h
		}
	}
	else
	{
		*outGolden = cv2;
		*outXMin= *h2;
		if(ADAPTIVE_BSQ == args->method)
		{
			*outXMin= floor(*h2);//a função floor existe em math.h
		}
	}
	if(ADAPTIVE_N == args->method)//tinha o caso de ser diferente mas só printava golden e xmin, que serão retornados
	{
		*hv= *outXMin;
		GoldenDataIfAdpN *dataToReturn= malloc(sizeof(GoldenDataIfAdpN));
		ASSERT(dataToReturn);
		dataToReturn->xMin= *outXMin;
		FowardListAddElement(args->communication, dataToReturn);//supondo que esse apend está querendo retornar o xmin
		//aqui tem o comando "append from hv", perguntar o que isso faz
	}
}
#endif

#define GOLDEN_PRONTO
#ifdef GOLDEN_PRONTO
void** Golden(GoldenArguments* args)//vai retornar a matriz de distâncias se for pedido, caso contrário retorna NULL
{
	//calculando matriz de distâncias para apenas pegar a menor e a maior distância
	double minDist/*, medDist*/, maxDist;
	double **distances= DistanceBetweenAllPoints(args->data, args->x_dCoord, args->y_dCoord, &minDist, &maxDist, true);
//	medDist=(maxDist + minDist)/2;

	double h0, h1, h2, h3;
	DoubleMatrix *x= NewDoubleMatrixAndInitializeElements(args->data->lines, 1, 1.0);
	DoubleMatrixConcatenateColumn(x, args->data, args->xVarColumn_independentVariable);
	DoubleMatrix *y= NewColumnDoubleMatrixFromMatrix(args->data, args->yVarColumn_dependentLocalVariables[0]);
	int *yAux= (args->yVarColumn_dependentLocalVariables)+1;
	while(NULL != yAux)
	{
		DoubleMatrixConcatenateColumn(y, args->data, *yAux);
		yAux++;
	}
	DoubleMatrix *yhat;
	if(ADAPTIVE_N == args->method)
	{
		double hv;// como hv é uma matriz de 1x1, vou tratar como um double
//		yhat= NewDoubleMatrix(1, 1);	// o yhat é declarado na linha 47 como uma matrix coluna de n linhas e depois de declarado como uma matriz 1x1 de valor zero na linha 50 e depois não é mais usado
//aparentemente é usado nos CVs
		yhat= NewDoubleMatrix(1, 1);
		yhat->elements[0]=0;
		for(int i =1; i < args->data->lines; i++)
		{
			func1(minDist, maxDist, &h0, &h1, &h2, &h3, x, y, &hv);
		}
	}
	else
	{
		yhat= NewDoubleMatrix(args->data->lines, 1);
//		memset(yhat->elements, 0, sizeof(double)*yhat->lines);
		func1(minDist, maxDist, &h0, &h1, &h2, &h3);
	}
	//aqui tem o comando quit, o que ele deve fazer??
	DeleteDoubleMatrix(x);
	DeleteDoubleMatrix(y);
	DeleteDoubleMatrix(yhat);
	FowardListAddElement(args->communication, NULL);
	if(args->returnDistancesMatrix)
	{
		return distances;
	}
	else
	{
		free(distances);
		return NULL;
	}
}
#endif


#define CV_PRONTO
#ifdef CV_PRONTO

static double Sub(double a, double b)
{
	return a-b;
}

static double Div(double a, double b)
{
	return a/b;
}

static double Mult(double a, double b)
{
	return a*b;
}

static void CvAux1(DoubleMatrix *data, int xCOORD, int yCOORD, int oneOrTwo, int i, bool distanceInKm, KernelType method, double h1, double maxDistanceBetweenPoints)//[DUVIDA] o estranho é que o i só deveria existir no caso do adaptive n, mas ele é usando em todos os casos
{
	DoubleMatrix *d= NewDoubleMatrix(1, 3);
	DoubleMatrix *dist= NewDoubleMatrixAndInitializeElements(1, 3, 0.0);
	for(int j=0; j < data->lines; j++)
	{
		double arco, d1;
		if(distanceInKm)
		{//o COORD é uma tabela com (variável independente, variáveis dependentes)
			double dif= abs(DoubleMatrixGetElement(data, i, 1) - DoubleMatrixGetElement(data, j, 1) );
			double raio= raio=arcos(-1)/180;
			double argument=
					sin(DoubleMatrixGetElement(data, i, yCOORD)*raio)
					*sin(DoubleMatrixGetElement(data, j,yCOORD)*raio)
					+cos(DoubleMatrixGetElement(data, i,yCOORD)*raio)
					*cos(DoubleMatrixGetElement(data,j, yCOORD)*raio)*cos(dif*raio);
			if(1 <= argument)
			{
				arco=0;
			}
			else//dúvida: onde realmente termina esse else? Estou supondo que na linha seguinte
			{
				//law of cosines
				arco =
						arcos(
							sin(DoubleMatrixGetElement(data, i,yCOORD)*raio)
							*sin(DoubleMatrixGetElement(data, j,yCOORD)*raio)
							+cos(DoubleMatrixGetElement(data, i,yCOORD)*raio)
							*cos(DoubleMatrixGetElement(data, j,yCOORD)*raio)
							*cos(dif*raio)
						);
			}
			d1= arco *APPROX_EARTH_RADIUS;
			if(0.001 >= d1)
			{
				d1=0;
			}
		}
		else
		{
			d1= sqrt(//isolar em uma função 01
						pow(
							DoubleMatrixGetElement(data, i,xCOORD)
							-DoubleMatrixGetElement(data, j,xCOORD)
							,2)
						+pow(
							DoubleMatrixGetElement(data,i,yCOORD)
							-DoubleMatrixGetElement(data,j,yCOORD)
							,2)
					);
		}
		if(
				((FIXED_G == method  && d1 !=0) && oneOrTwo == 1)//aqui é a única diferença entre o cv1 e o cv2
				|| (FIXED_G == method && (d1<=MAXV*1 && d1 !=0) && oneOrTwo ==2)
				|| ((FIXED_BSQ == method || ADAPTIVE_N == method) && d1 <= h1 && d1 !=0 )
				||(ADAPTIVE_BSQ == method && d1 !=0 )
			)
		{//foi feito um super if para tratar tantas condições
			DoubleMatrixSetElement(d, 0, 0, i);
			DoubleMatrixSetElement(d, 0, 1, j);
			if(!distanceInKm)
			{
				DoubleMatrixSetElement(d, 0, 2,
						sqrt(
							pow(
								DoubleMatrixGetElement(data, i,xCOORD)
								-DoubleMatrixGetElement(data, j,xCOORD)
								,2)
							+pow(
								DoubleMatrixGetElement(data,i,yCOORD)
								-DoubleMatrixGetElement(data,j,yCOORD)
								,2)
						)
					);
			}
			else
			{
				DoubleMatrixSetElement(d, 0, 2, arco*APPROX_EARTH_RADIUS);
			}
			DoubleMatrixConcatenateLine(dist, d, 0);
		}
	}
	int u= dist->lines;//[DUVIDA] x1 e y1 começam como matrizes 1x1?
	DoubleMatrix* w= NewDoubleMatrix(u, 1);
	DoubleMatrix *x1= NewLineDoubleMatrixFromMatrix(data, i);//verificar qual a diferença dessas 2 matrizes
	DoubleMatrix *y1= NewLineDoubleMatrixFromMatrix(data, i);
	for(int jj =1; jj < u; jj++)
	{
		if(FIXED_BSQ == method || ADAPTIVE_N == method)
		{
//			w[jj]= pow(1-pow(DoubleMatrixGetElement(dist, jj, 3) / h1, 2), 2);
			DoubleMatrixSetElement(
									w,
									jj,
									0,
									pow(1-pow(DoubleMatrixGetElement(dist, jj, 3) / h1, 2), 2)
								);
		}
		else
		{
//			w[jj]= exp(pow(-(DoubleMatrixGetElement(dist,jj,3)/ h1),2));
			DoubleMatrixSetElement(
									w,
									jj,
									0,
									exp(pow(-(DoubleMatrixGetElement(dist,jj,3)/ h1),2))
								);
		}
		DoubleMatrixConcatenateLine(x1, x, DoubleMatrixGetElement(dist, jj, 2-1));
		DoubleMatrixConcatenateLine(y1, y, DoubleMatrixGetElement(dist, jj, 2-1));
	}
	if(ADAPTIVE_BSQ== method)
	{
		DoubleMatrix *x1= NewLineDoubleMatrixFromMatrix(x, i);
		DoubleMatrix *y1= NewLineDoubleMatrixFromMatrix(y, i);
		//algo é ordenado aqui
//		DoubleMatrixConcatenateColumn(dist, ?);//Ver que operação dos ":" faz, pois aqui está 1:nrow(dist)
		//se eu entendi certo o objeivo é contatenar a transposta de dist a dist
		for(int count=0; count < dist->lines; count++)
		{
			DoubleMatrix *temp= NewLineDoubleMatrixFromMatrix(dist, count);
			DoubleMatrixTranspose(temp, true);
			DoubleMatrixConcatenateColumn(dist, temp, 0);
			DeleteDoubleMatrix(temp);
		}
		w=NewDoubleMatrix(n, 2);//verificar se precisa desalocar antes
		double hn= DoubleMatrixGetElement(dist, h1, 3);
		for(int jj=2; jj < n; jj++)
		{
			if(DoubleMatrixGetElement(dist, jj, 4) <= h1)
			{
				DoubleMatrixSetElement(w, jj, 1, pow(1-pow((DoubleMatrixGetElement(dist, jj, 3)/hn), 2), 2) );
			}
			else
			{
				DoubleMatrixSetElement(w, jj, 1, 0);
			}
			DoubleMatrixSetElement(w, jj, 2, DoubleMatrixGetElement(dist, jj, 2));
		}
		//whot??position= w[loc(w[,1]>0),2];
		for(int count=0; count < w->lines; count++)
		{
			if(DoubleMatrixGetElement(w, count, 1) > 0)
			{
				position= DoubleMatrixGetElement(w, count, 2);
			}
		}
		//whot?  w={0}//w[position,1];
		//supondo que essa chaves atribua zero a todos os elementos de w, pergunta se é isso mesmo
		memset(w->elements, 0, w->lines*w->columns);
//		DoubleMatrixConcatenateLine(w, w, position, 1);
		//whot? 	w={0}//w[position,1];//está concatenando um elemento numa linha

		DoubleMatrixConcatenateLine(x1, x, position);
		DoubleMatrixConcatenateLine(y1, y, position);
		DeleteDoubleMatrix(x1);
		DeleteDoubleMatrix(y1);
	}
	DoubleMatrix *b;
	if(0 == DoubleMatrixDeterminant())
	{
		b=NewDoubleMatrixAndInitializeElements(x->columns, 1, 0);
	}
	else
	{
		b= //inv(x1`*(w#x1#wt1))*x1`*(w#y1#wt1);
	}
//	if(DoubleMatrixDeterminant()) //aqui faz uso do wt1, que a princípio era pra ser ignorado
	//aqui tem 	yhat[i]=x[i,]*b; aparentemente está sobrescrevendo uma coluna da matriz por outra de outra matriz resultado de uma multiplicação
	DoubleMatrix *temp= NewLineDoubleMatrixFromMatrix(x, i);
	DoubleMatrix *temp2 = DoubleMatrixMultiplication(temp, b);
	for(count =0; count < yhat->columns; count++)
	{
		DoubleMatrixSetElement(yhat, i, count, DoubleMatrixGetElement(temp2, i, count));
	}
	DeleteDoubleMatrix(b);
	DeleteDoubleMatrix(temp);
	DeleteDoubleMatrix(tem2);
	DeleteDoubleMatrix(x1);
	DeleteDoubleMatrix(y1);
}

static double CrossValidation(DoubleMatrix *data, int oneOrTwo, bool distanceInKm, KernelType method, int i, int xCoord, int yCoord)//aqui temos um problema, estava supondo que era pra ignorar o wt, mas ele é necessariamente utilizado no cálculo do cv.
{
	if(ADAPTIVE_N != method)
	{
		int i;
		for(i=0; i < data->lines; i++)
		{
			CvAux1(data, xCoord, yCoord, oneOrTwo, i);
		}
	}
	else
	{
		CvAux1();
//		cv1= ((y[i]-yhat)#wt1#w)`*(y[i]-yhat);
	}
	if(ADAPTIVE_N != method)
	{
		DoubleMatrix *temp= DoubleMatrixCopy(y);
		DoubleMatrixElementBinaryOperation(temp, yhat, true, Sub);//temp = y[i] -yhat
		DoubleMatrix *temp2= DoubleMatrixElementBinaryOperation(temp, wt1, false, Mult);
		DoubleMatrixElementBinaryOperation(temp, w, true, Mult);
		DoubleMatrixTranspose(temp2, true);
		double cv= DoubleMatrixMultiplication(temp2, temp);
		DeleteDoubleMatrix(temp);
		DeleteDoubleMatrix(temp2);
		rerturn //((y-yhat)#wt1#w)`*(y-yhat);
	}
	else
	{
		//yhat[1]=x[i,]*b;

		//((y[i]-yhat)#wt1#w)`*(y[i]-yhat)
		DoubleMatrix *temp= NewLineDoubleMatrixFromMatrix(y, i);
		DoubleMatrixElementBinaryOperation(temp, yhat, true, Sub);//temp = y[i] -yhat
		DoubleMatrix *temp2= DoubleMatrixElementBinaryOperation(temp, wt1, false, Mult);
		DoubleMatrixElementBinaryOperation(temp, w, true, Mult);
		DoubleMatrixTranspose(temp2, true);
		double cv= DoubleMatrixMultiplication(temp2, temp);
		DeleteDoubleMatrix(temp);
		DeleteDoubleMatrix(temp2);
		return cv;
	}
}


#endif
