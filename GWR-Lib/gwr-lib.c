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

static double CrossValidation(DoubleMatrix *data, int oneOrTwo, bool distanceInKm, KernelType method, int forCount, int xCoord, int yCoord, DoubleMatrix *x, DoubleMatrix *y, DoubleMatrix *yhat, double h1, double maxDistanceBetweenPoints);

#define F1_PRONTO
#ifdef F1_PRONTO
static void func1(double min, double max, double *h0, double *h1, double *h2, double *h3,/* double *hv,*/ DoubleMatrix *yhat, GoldenArguments *args, DoubleMatrix *x, DoubleMatrix *y, double *outGolden, double *outXMin, int forCounter)//de acordo com o código SAS o outGolden e outXMin só são retornados(printados) caso o método não seja adaptive N, mas como em todos os casos essas variáveis existem, estou retornando-as
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
	double cv1= CrossValidation(args->data, 1, args->distanceInKM, args->method, forCounter, args->x_dCoord, args->y_dCoord, x, y, yhat, *h1, max);
	double cv2= CrossValidation(args->data, 2, args->distanceInKM, args->method, forCounter, args->x_dCoord, args->y_dCoord, x, y, yhat, *h1, max);

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
			cv1= CrossValidation(args->data, 1, args->distanceInKM, args->method, forCounter, args->x_dCoord, args->y_dCoord, x, y, yhat, *h1, max);
			cv2= CrossValidation(args->data, 2, args->distanceInKM, args->method, forCounter, args->x_dCoord, args->y_dCoord, x, y, yhat, *h1, max);
		}
		else
		{
			*h3= *h2;
			*h2= *h1;
			*h1= r*(*h2) + c*(*h0);
			cv1= CrossValidation(args->data, 1, args->distanceInKM, args->method, forCounter, args->x_dCoord, args->y_dCoord, x, y, yhat, *h1, max);
			cv2= CrossValidation(args->data, 2, args->distanceInKM, args->method, forCounter, args->x_dCoord, args->y_dCoord, x, y, yhat, *h1, max);
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
		outGolden[forCounter] = cv1;
		outXMin[forCounter]= *h1;
		if(ADAPTIVE_BSQ == args->method)
		{
			outXMin[forCounter]= floor(*h1);//a função floor existe em math.h
		}
	}
	else
	{
		outGolden[forCounter] = cv2;
		outXMin[forCounter]= *h2;
		if(ADAPTIVE_BSQ == args->method)
		{
			outXMin[forCounter]= floor(*h2);//a função floor existe em math.h
		}
	}
	if(ADAPTIVE_N == args->method)//tinha o caso de ser diferente mas só printava golden e xmin, que serão retornados
	{
//		*hv= outXMin[forCounter];
		GoldenDataIfAdpN *dataToReturn= malloc(sizeof(GoldenDataIfAdpN));
		ASSERT(dataToReturn);
		dataToReturn->xMin= outXMin[forCounter];
		FowardListAddElement(args->communication, dataToReturn);//supondo que esse apend está querendo retornar o xmin
	}
}
#endif

#define GOLDEN_PRONTO
#ifdef GOLDEN_PRONTO
void *Golden(void *args_)//vai retornar a matriz de distâncias se for pedido, caso contrário retorna NULL
{
	GoldenArguments *args= args_;
	//calculando matriz de distâncias para apenas pegar a menor e a maior distância
	double minDist/*, medDist*/, maxDist;
	double **distances= DistanceBetweenAllPoints(args->data, args->x_dCoord, args->y_dCoord, &minDist, &maxDist, true);
//	medDist=(maxDist + minDist)/2;

	double h0, h1, h2, h3;
	DoubleMatrix *x= NewDoubleMatrixAndInitializeElements(args->data->lines, 1, 1.0);
	DoubleMatrixConcatenateColumn(x, args->data, args->xVarColumn_independentVariable);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("Golden.txt:36\tx criado, dimensões %dx%d\r\n", x->lines, x->columns);
#endif
	DoubleMatrix *y= NewColumnDoubleMatrixFromMatrix(args->data, args->yVarColumn_dependentLocalVariables[0]);
	int *yAux= (args->yVarColumn_dependentLocalVariables)+1;
	while(-1 != *yAux)
	{
		printf("%s|%s:%d\t *yAux= %d\n", __FILE__, __func__, __LINE__, *yAux);
		DoubleMatrixConcatenateColumn(y, args->data, *yAux);
		yAux++;
	}
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("Golden.txt:37\ty criado, dimensões %dx%d\r\n", y->lines, y->columns);
#endif
	DoubleMatrix *yhat;
	if(ADAPTIVE_N == args->method)
	{
//		double hv;// como hv é uma matriz de 1x1, vou tratar como um double
//		yhat= NewDoubleMatrix(1, 1);	// o yhat é declarado na linha 47 como uma matrix coluna de n linhas e depois de declarado como uma matriz 1x1 de valor zero na linha 50 e depois não é mais usado
//aparentemente é usado nos CVs
		yhat= NewDoubleMatrix(1, 1);
		yhat->elements[0]=0;
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("Golden.txt:50\tyhat criado, dimensões %dx%d\r\n", yhat->lines, yhat->columns);
#endif
		double golden[args->data->lines];
		double xMin[args->data->lines];
		for(int i =1; i < args->data->lines; i++)
		{
			func1(minDist, maxDist, &h0, &h1, &h2, &h3/*, &hv*/, yhat, args, x, y, golden, xMin, i);
		}
	}
	else
	{
		yhat= NewDoubleMatrix(args->data->lines, 1);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("Golden.txt:37\tyhat criado, dimensões %dx%d\r\n", yhat->lines, yhat->columns);
#endif
//		memset(yhat->elements, 0, sizeof(double)*yhat->lines);
		double golden;
		double xMin;
		func1(minDist, maxDist, &h0, &h1, &h2, &h3,/* &hv,*/ yhat, args, x, y, &golden, &xMin, -1);
	}
	//aqui tem o comando quit, o que ele deve fazer??
	DeleteDoubleMatrix(x);
	DeleteDoubleMatrix(y);
	DeleteDoubleMatrix(yhat);
	FowardListAddElement(args->communication, NULL);
	if(args->returnDistancesMatrix)
	{
		pthread_exit(distances);
		return (void*)distances;
	}
	else
	{
		free(distances);
		pthread_exit(NULL);
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

static void CvAux1(DoubleMatrix *data, DoubleMatrix *x, DoubleMatrix *y, DoubleMatrix *yhat,int xCOORD, int yCOORD, int oneOrTwo, int i, bool distanceInKm, KernelType method, double h1, double maxDistanceBetweenPoints, DoubleMatrix *outW)
{
	DoubleMatrix *d= NewDoubleMatrix(1, 3);
	DoubleMatrix *dist= NewDoubleMatrixAndInitializeElements(1, 3, 0.0);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:6\td e dist criados, dimensões %dx%d\r\n", d->lines, d->columns);
#endif
	for(int j=0; j < data->lines; j++)
	{
		double arco, d1;
		if(distanceInKm)
		{//o COORD é uma tabela com (variável independente, variáveis dependentes)
			double dif= abs(DoubleMatrixGetElement(data, i, 1) - DoubleMatrixGetElement(data, j, 1) );
			double raio= raio=acos(-1)/180;
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
						acos(
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
				|| (FIXED_G == method && (d1<=maxDistanceBetweenPoints*1 && d1 !=0) && oneOrTwo ==2)
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
	fprintf(stderr, "%s|%s:%d\tu= %d\n", __FILE__, __func__, __LINE__, u);
	DoubleMatrix* w= NewDoubleMatrix(u, 1);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:44\tw criado, dimensões %dx%d\r\n", w->lines, w->columns);
#endif
	DoubleMatrix *x1= NewLineDoubleMatrixFromMatrix(data, i);//verificar qual a diferença dessas 2 matrizes
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:45\tx1 criado, dimensões %dx%d\r\n", x1->lines, x1->columns);
#endif
	DoubleMatrix *y1= NewLineDoubleMatrixFromMatrix(data, i);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:46\ty1 criado, dimensões %dx%d\r\n", y1->lines, y1->columns);
#endif
	for(int jj =2-1; jj < u; jj++)
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
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:56\tx1, dimensões %dx%d\r\n", x1->lines, x1->columns);
	printf("cv1.txt:56\ty1, dimensões %dx%d\r\n", y1->lines, y1->columns);
#endif
	if(ADAPTIVE_BSQ== method)
	{
//		DoubleMatrix *x1= NewLineDoubleMatrixFromMatrix(x, i);
//		DoubleMatrix *y1= NewLineDoubleMatrixFromMatrix(y, i);
		double position;
		//algo é ordenado aqui
//		DoubleMatrixConcatenateColumn(dist, ?);//Ver que operação dos ":" faz, pois aqui está 1:nrow(dist)
		//se eu entendi certo o objeivo é contatenar a transposta de dist a dist
		for(int count=0; count < dist->lines; count++)
		{
			DoubleMatrix *temp= NewLineDoubleMatrixFromMatrix(dist, count);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:62\tdist[i] criado, dimensões %dx%d\r\n", temp->lines, temp->columns);
#endif
			DoubleMatrixTranspose(temp, true);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:62\tdist[i]' criado, dimensões %dx%d\r\n", temp->lines, temp->columns);
#endif
			DoubleMatrixConcatenateColumn(dist, temp, 0);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:62\tdist= dist ||dist[i]' criado, dimensões %dx%d\r\n", temp->lines, temp->columns);
#endif
			DeleteDoubleMatrix(temp);
		}
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:63\tdist ficou com as dimensões %dx%d\r\n", dist->lines, dist->columns);
#endif
		DeleteDoubleMatrix(w);
		w=NewDoubleMatrix(data->lines, 2);//verificar se precisa desalocar antes
		double hn= DoubleMatrixGetElement(dist, h1, 3);
		for(int jj=2-1; jj < data->lines; jj++)
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
#ifdef MODO1
		memset(w->elements, 0, w->lines*w->columns*sizeof(double));
#else
		DoubleMatrix *temp= NewDoubleMatrix(1, 2);
		temp->elements[0]=0;
		temp->elements[1]= DoubleMatrixGetElement(w, position, 1);
		DoubleMatrixConcatenateLine(w, temp, 0);
#endif
		//		DoubleMatrixConcatenateLine(w, w, position, 1);
		//whot? 	w={0}//w[position,1];//está concatenando um elemento numa linha

		DoubleMatrixConcatenateLine(x1, x, position);
		DoubleMatrixConcatenateLine(y1, y, position);
//		DeleteDoubleMatrix(x1);
//		DeleteDoubleMatrix(y1);
	}
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:44\tw, dimensões %dx%d\r\n", w->lines, w->columns);
	printf("cv1.txt:75\tx1, dimensões %dx%d\r\n", x1->lines, x1->columns);
	printf("cv1.txt:75\ty1, dimensões %dx%d\r\n", y1->lines, y1->columns);
#endif
	//det(x1`*(w#x1#wt1))=0 then b=j(ncol(x),1,0);
//	DoubleMatrix *aux= DoubleMatrixElementBinaryOperation(w, x1, false, Mult);//aux= w#x1
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:63\tw está as dimensões %dx%d\r\n", w->lines, w->columns);
	printf("cv1.txt:63\tx1 está com as dimensões %dx%d\r\n", x1->lines, x1->columns);
#endif
	DoubleMatrix *aux = DoubleMatrixBinOpColumnsPerColumn(w, x1, 0, false, Mult);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:77\tw#x1 ficou com as dimensões %dx%d\r\n", aux->lines, aux->columns);
#endif

	DoubleMatrix *aux2;
	aux2= aux;

	aux= DoubleMatrixCopy(x1);
	DoubleMatrixTranspose(aux, true);//#aux = x1'
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:77\tx1' ficou com as dimensões %dx%d\r\n", aux->lines, aux->columns);
#endif
	DoubleMatrix *aux3= DoubleMatrixMultiplication(aux, aux2);//aux3=x1`*(w#x1#wt1)
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:77\tx1`*(w#x1#wt1) ficou com as dimensões %dx%d\r\n", aux3->lines, aux3->columns);
#endif
	DoubleMatrix *b;
//	fprintf(stderr, "%s|%s:%d\t aux2 is %dx%d\r\n", __FILE__, __func__, __LINE__, aux2->lines, aux2->columns);
//	fprintf(stderr, "%s|%s:%d\t aux3 is %dx%d\r\n", __FILE__, __func__, __LINE__, aux3->lines, aux3->columns);
	if(0 == GWR_Determinant(aux3))
	{
		b=NewDoubleMatrixAndInitializeElements(x->columns, 1, 0);
	}
	else
	{
		//inv(x1`*(w#x1#wt1))*x1`*(w#y1#wt1);
		b= DoubleMatrixInverse(aux3);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:79\tinv(x1`*(w#x1#wt1)) ficou com as dimensões %dx%d\r\n", b->lines, b->columns);
#endif
		DeleteDoubleMatrix(aux3);
		aux3= DoubleMatrixMultiplication(b, aux);//aux3=inv(x1`*(w#x1#wt1))*x1`
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:79\tinv(x1`*(w#x1#wt1))*x1` ficou com as dimensões %dx%d\r\n", aux3->lines, aux3->columns);
#endif
		DeleteDoubleMatrix(b);
		b= DoubleMatrixMultiplication(aux3, aux2);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:79\tb ficou com as dimensões %dx%d\r\n", b->lines, b->columns);
#endif
	}
	DeleteDoubleMatrix(aux3);
	DeleteDoubleMatrix(aux2);
	DeleteDoubleMatrix(aux);
//	if(GWR_Determinant()) //aqui faz uso do wt1, que a princípio era pra ser ignorado
	//aqui tem 	yhat[i]=x[i,]*b; aparentemente está sobrescrevendo uma coluna da matriz por outra de outra matriz resultado de uma multiplicação
	DoubleMatrix *temp= NewLineDoubleMatrixFromMatrix(x, i);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:81\tx[i] ficou com as dimensões %dx%d\r\n", temp->lines, temp->columns);
#endif
	DoubleMatrix *temp2 = DoubleMatrixMultiplication(temp, b);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:81\tx[i]*b ficou com as dimensões %dx%d\r\n", temp2->lines, temp2->columns);
#endif
	for(int count =0; count < yhat->columns; count++)
	{
		DoubleMatrixSetElement(yhat, i, count, DoubleMatrixGetElement(temp2, i, count));
	}
	DeleteDoubleMatrix(b);
	DeleteDoubleMatrix(temp);
	DeleteDoubleMatrix(temp2);
	DeleteDoubleMatrix(x1);
	DeleteDoubleMatrix(y1);
	outW=w;
}

static double CrossValidation(DoubleMatrix *data, int oneOrTwo, bool distanceInKm, KernelType method, int forCount, int xCoord, int yCoord, DoubleMatrix *x, DoubleMatrix *y, DoubleMatrix *yhat, double h1, double maxDistanceBetweenPoints)
{
	DoubleMatrix *w= NULL;
	if(ADAPTIVE_N != method)
	{
		int i;
		for(i=0; i < data->lines; i++)
		{
			CvAux1(data, x, y, yhat, xCoord, yCoord, oneOrTwo, i, distanceInKm, method, h1, maxDistanceBetweenPoints, w);
		}
	}
	else
	{
		CvAux1(data, x, y, yhat, xCoord, yCoord, oneOrTwo, forCount, distanceInKm, method, h1, maxDistanceBetweenPoints, w);
//		cv1= ((y[i]-yhat)#wt1#w)`*(y[i]-yhat);
	}
	DoubleMatrix *cv;
	if(ADAPTIVE_N != method)
	{
		DoubleMatrix *temp= DoubleMatrixCopy(y);
		DoubleMatrixElementBinaryOperation(temp, yhat, true, Sub);//temp = y[i] -yhat
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:83\ty-yhat criado, dimensões %dx%d\r\n", temp->lines, temp->columns);
#endif
		DoubleMatrix *temp2= DoubleMatrixCopy(temp);
		DoubleMatrixElementBinaryOperation(temp, w, true, Mult);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:83\ty-yhat#w criado, dimensões %dx%d\r\n", temp->lines, temp->columns);
#endif
		DoubleMatrixTranspose(temp2, true);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:83\t(y-yhat#w)' criado, dimensões %dx%d\r\n", temp2->lines, temp2->columns);
#endif
		cv= DoubleMatrixMultiplication(temp2, temp);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:83\t(y-yhat#w)'*(y-yhat) criado, dimensões %dx%d\r\n", cv->lines, cv->columns);
#endif
		DeleteDoubleMatrix(temp);
		DeleteDoubleMatrix(temp2);
	}
	else
	{
		//yhat[1]=x[i,]*b;

		//((y[i]-yhat)#wt1#w)`*(y[i]-yhat)
		DoubleMatrix *temp= NewLineDoubleMatrixFromMatrix(y, forCount);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:87\ty[i] criado, dimensões %dx%d\r\n", temp->lines, temp->columns);
#endif
		DoubleMatrixElementBinaryOperation(temp, yhat, true, Sub);//temp = y[i] -yhat
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:87\ty[i]-yhat criado, dimensões %dx%d\r\n", temp->lines, temp->columns);
#endif
		DoubleMatrix *temp2= DoubleMatrixCopy(temp);
		DoubleMatrixElementBinaryOperation(temp, w, true, Mult);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:87\t(y[i]-yhat)#w criado, dimensões %dx%d\r\n", temp->lines, temp->columns);
#endif
		DoubleMatrixTranspose(temp, true);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:87\t( (y[i]-yhat)#w)' criado, dimensões %dx%d\r\n", temp->lines, temp->columns);
#endif
		cv= DoubleMatrixMultiplication(temp, temp2);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:87\t( (y[i]-yhat)#w)'*(y[i]-yhat) criado, dimensões %dx%d\r\n", cv->lines, cv->columns);
#endif
		DeleteDoubleMatrix(temp);
		DeleteDoubleMatrix(temp2);
	}
	double result= cv->elements[0];
	DeleteDoubleMatrix(cv);
	DeleteDoubleMatrix(w);
	return result;//((y-yhat)#wt1#w)`*(y-yhat);
}

double GWR_Determinant(DoubleMatrix *m)
{
	DoubleMatrix *temp = DoubleMatrixTranspose(m, false);
	DoubleMatrix *temp2= DoubleMatrixMultiplication(temp, m);
	double result= DoubleMatrixDeterminant(temp2);
	DeleteDoubleMatrix(temp);
	DeleteDoubleMatrix(temp2);
	return result;
}


#endif
