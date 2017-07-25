#include "gwr-lib.h"
#include <math.h>
#include "string.h"

#define APPROX_EARTH_RADIUS (6371)
#define WHERE printf("%s|%s:%d\r\n", __FILE__, __func__, __LINE__);

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
			cv1=cv2;
//			cv1= CrossValidation(args->data, 1, args->distanceInKM, args->method, forCounter, args->x_dCoord, args->y_dCoord, x, y, yhat, *h1, max);
			cv2= CrossValidation(args->data, 2, args->distanceInKM, args->method, forCounter, args->x_dCoord, args->y_dCoord, x, y, yhat, *h1, max);
		}
		else
		{
			*h3= *h2;
			*h2= *h1;
			*h1= r*(*h2) + c*(*h0);
			cv2=cv1;
//			cv1= CrossValidation(args->data, 1, args->distanceInKM, args->method, forCounter, args->x_dCoord, args->y_dCoord, x, y, yhat, *h1, max);
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
		dataToReturn->cv1= cv1;
		dataToReturn->cv2= cv2;
		FowardListAddElement(args->communication, dataToReturn);//supondo que esse apend está querendo retornar o xmin
	}
}

void *Golden(void *args_)//vai retornar a matriz de distâncias se for pedido, caso contrário retorna NULL
{
	GoldenArguments *args= args_;
	//calculando matriz de distâncias para apenas pegar a menor e a maior distância
	double minDist/*, medDist*/, maxDist;
	double **distances= DistanceBetweenAllPoints(args->data, args->x_dCoord, args->y_dCoord, &minDist, &maxDist, true);
	*(args->outMaxDistanceBetweenPoints)= maxDist;
//	medDist=(maxDist + minDist)/2;
	//x é em loop, y não é em loop.
	//o erro está na interface gráfica
	double h0, h1, h2, h3;
	DoubleMatrix *x= NewDoubleMatrixAndInitializeElements(args->data->lines, 1, 1.0);
	int *xAux= (args->yVarColumn_independentLocalVariables);//+1????
	while(-1 != *xAux)
	{
		printf("%s|%s:%d\t *xAux= %d\n", __FILE__, __func__, __LINE__, *xAux);
		DoubleMatrixConcatenateColumn(x, args->data, *xAux);
		xAux++;
	}
	WHERE;
	DoubleMatrixPrint(x, stdout, "\t%lf", "\n");
	printf("args->xVarColumn_independentVariable: %d\n", args->xVarColumn_dependentVariable);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("Golden.txt:36\tx criado, dimensões %dx%d\r\n", x->lines, x->columns);
#endif
	DoubleMatrix *y= NewColumnDoubleMatrixFromMatrix(args->data, args->xVarColumn_dependentVariable);
//	DoubleMatrixConcatenateColumn(y, args->data, args->xVarColumn_dependentVariable);
	WHERE;
	DoubleMatrixPrint(y, stdout, "\t%lf", "\n");
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

static void CvAux1(DoubleMatrix *data, DoubleMatrix *x, DoubleMatrix *y, DoubleMatrix *yhat,int xCOORD, int yCOORD, int oneOrTwo, int i, bool distanceInKm, KernelType method, double h1, double maxDistanceBetweenPoints, DoubleMatrix **outW)
{
	DoubleMatrix *d= NewDoubleMatrix(1, 3);
	DoubleMatrix *dist= NewDoubleMatrixAndInitializeElements(1, 3, 0.0);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:6\td e dist criados, dimensões %dx%d\r\n", d->lines, d->columns);
#endif
	double hn;
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
	int u= data->lines;//[DUVIDA] x1 e y1 começam como matrizes 1x1?
#ifdef DEBUG_MATRIX_DIMENSIONS
	fprintf(stdout, "%s|%s:%d\tu= %d\n", __FILE__, __func__, __LINE__, u);
	fprintf(stdout, "%s|%s:%d\tdata está %dx%d\n", __FILE__, __func__, __LINE__, data->lines, data->columns);
	fprintf(stdout, "%s|%s:%d\tdist está %dx%d\n", __FILE__, __func__, __LINE__, dist->lines, dist->columns);
#endif
	DoubleMatrix *w= NewDoubleMatrix(u, 1);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:44\tw criado, dimensões %dx%d\r\n", w->lines, w->columns);
#endif
	DoubleMatrix *x1= NewLineDoubleMatrixFromMatrix(x, i);//verificar qual a diferença dessas 2 matrizes
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:45\tx1 criado, dimensões i=%d %dx%d\r\n", i, x1->lines, x1->columns);
#endif
	DoubleMatrix *y1= NewLineDoubleMatrixFromMatrix(y, i);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:46\ty1 criado, dimensões %dx%d\r\n", y1->lines, y1->columns);
#endif
	if(FIXED_BSQ == method || ADAPTIVE_N == method)
	{
		DoubleMatrixSetElement(w, 0, 0,
								pow(1-pow(DoubleMatrixGetElement(dist, 0, 3-1) / h1, 2), 2));
	}
	else
	{
		DoubleMatrixSetElement(w, 0, 0,
								exp(pow(-(DoubleMatrixGetElement(dist,0,3-1)/ h1),2)));
	}
	for(int jj =2-1; jj < u; jj++)
	{
		if(FIXED_BSQ == method || ADAPTIVE_N == method)
		{
//			w[jj]= pow(1-pow(DoubleMatrixGetElement(dist, jj, 3) / h1, 2), 2);
			DoubleMatrixSetElement(
									w,
									jj,
									0,
									pow(1-pow(DoubleMatrixGetElement(dist, jj, 3-1) / h1, 2), 2)
								);
		}
		else
		{
//			w[jj]= exp(pow(-(DoubleMatrixGetElement(dist,jj,3)/ h1),2));
			DoubleMatrixSetElement(
									w,
									jj,
									0,
									exp(pow(-(DoubleMatrixGetElement(dist,jj,3-1)/ h1),2))
								);
		}
		DoubleMatrixConcatenateLine(x1, x, DoubleMatrixGetElement(dist, jj, 2-1));
		DoubleMatrixConcatenateLine(y1, y, DoubleMatrixGetElement(dist, jj, 2-1));
/*		printf("%s|%s:%d\t[DEBUG] I was here!\r\n", __FILE__, __func__, __LINE__);
		printf("jj = %d\n", jj);
*/	}
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
			temp= NULL;
		}
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:63\tdist ficou com as dimensões %dx%d\r\n", dist->lines, dist->columns);
#endif
		DeleteDoubleMatrix(w);
		w= NULL;
		w=NewDoubleMatrix(data->lines, 2);//verificar se precisa desalocar antes
		double hn= DoubleMatrixGetElement(dist, h1, 3);
#ifdef DEBUG_MATRIX_DIMENSIONS
		printf("%s|%s:%d\t%s\n", __FILE__, __func__, __LINE__, (NULL == w)? "W é NULL": "W é válido.");
#endif
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
	printf("%s|%s:%d\t%s\n", __FILE__, __func__, __LINE__, (NULL == w)? "W é NULL": "W é válido.");
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
	DoubleMatrix *aux = DoubleMatrixBinOpColumnsPerColumn(w, y, 0, false, Mult);//aux = W#y1
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:77\tw#x1 ficou com as dimensões %dx%d\r\n", aux->lines, aux->columns);
#endif

	DoubleMatrix *aux2;//aux2 = w#y1
	aux2= aux;

	aux= DoubleMatrixCopy(x1);
	DoubleMatrixTranspose(aux, true);//#aux = x1'
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:77\tx1' ficou com as dimensões %dx%d\r\n", aux->lines, aux->columns);
#endif
	DoubleMatrix *aux3= DoubleMatrixMultiplication(aux, aux2);//aux3=x1`*(w#y1)
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:77\tx1`*(w#x1#wt1) ficou com as dimensões %dx%d\r\n", aux3->lines, aux3->columns);
	printf("%s|%s:%d\t%s\n", __FILE__, __func__, __LINE__, (NULL == w)? "W é NULL": "W é válido.");
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
		DoubleMatrix *aux4= DoubleMatrixBinOpColumnsPerColumn(w, x, 0, false, Mult);//aux = W#x
		//inv(x1`*(w#y1))*x1`*(w#y1);
		b= DoubleMatrixInverse(aux3);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:79\tinv(x1`*(w#x1#wt1)) ficou com as dimensões %dx%d\r\n", b->lines, b->columns);
#endif
		DeleteDoubleMatrix(aux3);
		aux3= NULL;
		aux3= DoubleMatrixMultiplication(b, aux);//aux3=inv(x1`*(w#x1))*x1`
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:79\tinv(x1`*(w#x1#wt1))*x1` ficou com as dimensões %dx%d\r\n", aux3->lines, aux3->columns);
#endif
		DeleteDoubleMatrix(b);
		b= NULL;
		b= DoubleMatrixMultiplication(aux3, aux4);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:79\tb ficou com as dimensões %dx%d\r\n", b->lines, b->columns);
#endif
	}
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("%s|%s:%d\t%s\n", __FILE__, __func__, __LINE__, (NULL == w)? "W é NULL": "W é válido.");
#endif
	DeleteDoubleMatrix(aux3);
	aux3= NULL;
	DeleteDoubleMatrix(aux2);
	aux2= NULL;
	DeleteDoubleMatrix(aux);
	aux= NULL;
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
//		printf("Dimensões de yhat: %dx%d\n", yhat->lines, yhat->columns);
//		printf("Dimensões de temp2: %dx%d\n", temp2->lines, temp2->columns);
//		DoubleMatrixSetElement(yhat, i, count, DoubleMatrixGetElement(temp2, i, count));//temp2 está sendo uma matrix 1x1, vou tratar como double
		DoubleMatrixSetElement(yhat, i, count, DoubleMatrixGetElement(temp2, 0, 0));
	}
	DeleteDoubleMatrix(b);
	b= NULL;;
	DeleteDoubleMatrix(temp);
	temp= NULL;
	DeleteDoubleMatrix(temp2);
	temp2= NULL;
	DeleteDoubleMatrix(x1);
	x1= NULL;
	DeleteDoubleMatrix(y1);
	y1= NULL;
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("%s|%s:%d\t%s\n", __FILE__, __func__, __LINE__, (NULL == w)? "W é NULL": "W é válido.");
#endif
	*outW= w;
}

static double CrossValidation(DoubleMatrix *data, int oneOrTwo, bool distanceInKm, KernelType method, int forCount, int xCoord, int yCoord, DoubleMatrix *x, DoubleMatrix *y, DoubleMatrix *yhat, double h1, double maxDistanceBetweenPoints)
{
	DoubleMatrix *w= NULL;
	if(ADAPTIVE_N != method)
	{
		int i;
		for(i=0; i < data->lines; i++)
		{
			CvAux1(data, x, y, yhat, xCoord, yCoord, oneOrTwo, i, distanceInKm, method, h1, maxDistanceBetweenPoints, &w);
#ifdef DEBUG_MATRIX_DIMENSIONS
			printf("%s|%s:%d\t%s\n", __FILE__, __func__, __LINE__, (NULL == w)? "W é NULL": "W é válido.");
#endif
		}
		printf("%s|%s:%d\t%s\n", __FILE__, __func__, __LINE__, (NULL == w)? "W é NULL": "W é válido.");
	}
	else
	{
		CvAux1(data, x, y, yhat, xCoord, yCoord, oneOrTwo, forCount, distanceInKm, method, h1, maxDistanceBetweenPoints, &w);
//		cv1= ((y[i]-yhat)#wt1#w)`*(y[i]-yhat);
		printf("%s|%s:%d\t%s\n", __FILE__, __func__, __LINE__, (NULL == w)? "W é NULL": "W é válido.");
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
		printf("%s|%s:%d\t%s\n", __FILE__, __func__, __LINE__, (NULL == w)? "W é NULL": "W é válido.");
		fflush(stdout);
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

#define GWR_PRONTO
#ifdef GWR_PRONTO

void* GWR(void *args_)
{
	GWRArguments* args= args_;//contém o h
	DoubleMatrix *x= NewDoubleMatrixAndInitializeElements(args->data->lines, 1, 1.0);
	int *xAux= (args->yVarColumn_independentLocalVariables);//+1????
	int i =0;
	while(-1 != *xAux)
	{
		printf("%s|%s:%d\t *xAux= %d\n", __FILE__, __func__, __LINE__, *xAux);
		DoubleMatrixConcatenateColumn(x, args->data, *xAux);
		xAux++;
	}
	WHERE;
	DoubleMatrixPrint(x, stdout, "\t%lf", "\n");
	printf("args->xVarColumn_independentVariable: %d\n", args->xVarColumn_dependentVariable);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("Golden.txt:36\tx criado, dimensões %dx%d\r\n", x->lines, x->columns);
#endif
	int m1, m2;
	DoubleMatrix *y= NewColumnDoubleMatrixFromMatrix(args->data, args->xVarColumn_dependentVariable);
//	DoubleMatrixConcatenateColumn(y, args->data, args->xVarColumn_dependentVariable);
	WHERE;
	DoubleMatrixPrint(y, stdout, "\t%lf", "\n");
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("Golden.txt:37\ty criado, dimensões %dx%d\r\n", y->lines, y->columns);
#endif
	DoubleMatrix *yhat= NewDoubleMatrix(args->data->lines, 1);;
	double ni;//=ncol(unique(ci))
	double nh;//=ncol(unique(sh))
	//read all var {x,y} into POINTS??
	int m= args->data->lines;
	int n= y->lines;
	DoubleMatrix *fh= NewDoubleMatrixAndInitializeElements(n, 1, 0.);
	DoubleMatrix *bi= NewDoubleMatrixAndInitializeElements(x->columns*m, 4, 0.0);
	DoubleMatrix *BB= NewDoubleMatrixAndInitializeElements(x->columns*n, n, 0.);
	DoubleMatrix *rsqri= NewDoubleMatrixAndInitializeElements(m, 1, 0.);
	DoubleMatrix *sumwi= NewDoubleMatrixAndInitializeElements(m, 1, 0.);
	DoubleMatrix *varbi= NewDoubleMatrixAndInitializeElements(x->columns*m, 1, 0.);
	DoubleMatrix *varbigg= NewDoubleMatrixAndInitializeElements(x->columns*m, x->columns, 0.);
	DoubleMatrix *varbis= NewDoubleMatrixAndInitializeElements(x->columns*m, x->columns, 0.);
	DoubleMatrix *S= NewDoubleMatrixAndInitializeElements(m, 1, 0.);
	DoubleMatrix *S2= NewDoubleMatrixAndInitializeElements(m, 1, 0.);
	DoubleMatrix *biT= NewDoubleMatrixAndInitializeElements(m, x->columns+1, 0.);
/*	double med;
	{
		int totalElements= y->columns*y->lines;
		double sum=0;
		for(int count =0; count < totalElements; count++){
			sum+= y->elements[count];
		}
		med= sum/totalElements;
	}*/
//	DoubleMatrix *rikl= NewDoubleMatrixAndInitializeElements(n, /*comb(x->columns-1, 2)*/, 0.);
	DoubleMatrix *rikl= NewDoubleMatrixAndInitializeElements(n, BinomialCoefficient(x->columns-1, 2), 0.);
	DoubleMatrix *vif= NewDoubleMatrixAndInitializeElements(n, x->columns-1, 0.);
	
	//Aqui agora é o negócio de pegar distância entre pontos que nem no primerio dor do CvAux1, usando points no lugar de dcoord,
	//criar d e dist aqui
	DoubleMatrix *d= NewDoubleMatrix(1, 3);
	DoubleMatrix *dist= NewDoubleMatrixAndInitializeElements(1, 3, 0.0);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:6\td e dist criados, dimensões %dx%d\r\n", d->lines, d->columns);
#endif
	double hn;
	for(int j=0; j < args->data->lines; j++)
	{
		double arco, d1;
		if(args->distanceInKm)
		{//o COORD é uma tabela com (variável independente, variáveis dependentes)
			double dif= abs(DoubleMatrixGetElement(args->data, i, 1) - DoubleMatrixGetElement(args->data, j, 1) );
			double raio= raio=acos(-1)/180;
			double argument=
					sin(DoubleMatrixGetElement(args->data, i, args->y_dCoord)*raio)
					*sin(DoubleMatrixGetElement(args->data, j,args->y_dCoord)*raio)
					+cos(DoubleMatrixGetElement(args->data, i,args->y_dCoord)*raio)
					*cos(DoubleMatrixGetElement(args->data,j, args->y_dCoord)*raio)*cos(dif*raio);
			if(1 <= argument)
			{
				arco=0;
			}
			else//dúvida: onde realmente termina esse else? Estou supondo que na linha seguinte
			{
				//law of cosines
				arco =
						acos(
							sin(DoubleMatrixGetElement(args->data, i,args->y_dCoord)*raio)
							*sin(DoubleMatrixGetElement(args->data, j,args->y_dCoord)*raio)
							+cos(DoubleMatrixGetElement(args->data, i,args->y_dCoord)*raio)
							*cos(DoubleMatrixGetElement(args->data, j,args->y_dCoord)*raio)
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
							DoubleMatrixGetElement(args->data, i,args->x_dCoord)
							-DoubleMatrixGetElement(args->data, j,args->x_dCoord)
							,2)
						+pow(
							DoubleMatrixGetElement(args->data,i,args->y_dCoord)
							-DoubleMatrixGetElement(args->data,j,args->y_dCoord)
							,2)
					);
		}
		if(
				((FIXED_G == args->method  && d1 !=0))//aqui é a única diferença entre o cv1 e o cv2
				|| (FIXED_G == args->method && (d1<=args->maxDistanceBetweenPoints*1 && d1 !=0))
				|| ((FIXED_BSQ == args->method || ADAPTIVE_N == args->method) && d1 <= args->h && d1 !=0 )
				||(ADAPTIVE_BSQ == args->method && d1 !=0 )
			)
		{//foi feito um super if para tratar tantas condições
			DoubleMatrixSetElement(d, 0, 0, i);
			DoubleMatrixSetElement(d, 0, 1, j);
			if(!args->distanceInKm)
			{
				DoubleMatrixSetElement(d, 0, 2,
						sqrt(
							pow(
								DoubleMatrixGetElement(args->data, i,args->x_dCoord)
								-DoubleMatrixGetElement(args->data, j,args->x_dCoord)
								,2)
							+pow(
								DoubleMatrixGetElement(args->data,i,args->y_dCoord)
								-DoubleMatrixGetElement(args->data,j,args->y_dCoord)
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
	int u= args->data->lines;//[DUVIDA] x1 e y1 começam como matrizes 1x1?
#ifdef DEBUG_MATRIX_DIMENSIONS
	fprintf(stdout, "%s|%s:%d\tu= %d\n", __FILE__, __func__, __LINE__, u);
	fprintf(stdout, "%s|%s:%d\tdata está %dx%d\n", __FILE__, __func__, __LINE__, data->lines, data->columns);
	fprintf(stdout, "%s|%s:%d\tdist está %dx%d\n", __FILE__, __func__, __LINE__, dist->lines, dist->columns);
#endif
	DoubleMatrix *w= NewDoubleMatrix(u, 1);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:44\tw criado, dimensões %dx%d\r\n", w->lines, w->columns);
#endif
	DoubleMatrix *x1= NewLineDoubleMatrixFromMatrix(x, i);//verificar qual a diferença dessas 2 matrizes
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:45\tx1 criado, dimensões i=%d %dx%d\r\n", i, x1->lines, x1->columns);
#endif
	DoubleMatrix *y1= NewLineDoubleMatrixFromMatrix(y, i);
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:46\ty1 criado, dimensões %dx%d\r\n", y1->lines, y1->columns);
#endif
	if(FIXED_BSQ == args->method || ADAPTIVE_N == args->method)
	{
		DoubleMatrixSetElement(w, 0, 0,
								pow(1-pow(DoubleMatrixGetElement(dist, 0, 3-1) / args->h, 2), 2));
	}
	else
	{
		DoubleMatrixSetElement(w, 0, 0,
								exp(pow(-(DoubleMatrixGetElement(dist,0,3-1)/ args->h),2)));
	}
	for(int jj =2-1; jj < u; jj++)
	{
		if(FIXED_BSQ == args->method || ADAPTIVE_N == args->method)
		{
//			w[jj]= pow(1-pow(DoubleMatrixGetElement(dist, jj, 3) / h1, 2), 2);
			DoubleMatrixSetElement(
									w,
									jj,
									0,
									pow(1-pow(DoubleMatrixGetElement(dist, jj, 3-1) / args->h, 2), 2)
								);
		}
		else
		{
//			w[jj]= exp(pow(-(DoubleMatrixGetElement(dist,jj,3)/ h1),2));
			DoubleMatrixSetElement(
									w,
									jj,
									0,
									exp(pow(-(DoubleMatrixGetElement(dist,jj,3-1)/ args->h),2))
								);
		}
		DoubleMatrixConcatenateLine(x1, x, DoubleMatrixGetElement(dist, jj, 2-1));
		DoubleMatrixConcatenateLine(y1, y, DoubleMatrixGetElement(dist, jj, 2-1));
/*		printf("%s|%s:%d\t[DEBUG] I was here!\r\n", __FILE__, __func__, __LINE__);
		printf("jj = %d\n", jj);
*/	}
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:56\tx1, dimensões %dx%d\r\n", x1->lines, x1->columns);
	printf("cv1.txt:56\ty1, dimensões %dx%d\r\n", y1->lines, y1->columns);
#endif
	if(ADAPTIVE_BSQ == args->method)
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
			temp= NULL;
		}
#ifdef DEBUG_MATRIX_DIMENSIONS
	printf("cv1.txt:63\tdist ficou com as dimensões %dx%d\r\n", dist->lines, dist->columns);
#endif
		DeleteDoubleMatrix(w);
		w= NULL;
		w=NewDoubleMatrix(args->data->lines, 2);//verificar se precisa desalocar antes
		double hn= DoubleMatrixGetElement(dist, args->h, 3);
#ifdef DEBUG_MATRIX_DIMENSIONS
		printf("%s|%s:%d\t%s\n", __FILE__, __func__, __LINE__, (NULL == w)? "W é NULL": "W é válido.");
#endif
		for(int jj=2-1; jj < args->data->lines; jj++)
		{
			if(DoubleMatrixGetElement(dist, jj, 4) <= args->h)
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
		//linha 217 GWR cópia 1
		//aqui tem um end não sei do q, vou supor que não é do i
		//control+C control V do Golden
		//det(x1`*(w#x1#wt1))=0 then b=j(ncol(x),1,0);
	//	DoubleMatrix *aux= DoubleMatrixElementBinaryOperation(w, x1, false, Mult);//aux= w#x1
	#ifdef DEBUG_MATRIX_DIMENSIONS
		printf("cv1.txt:63\tw está as dimensões %dx%d\r\n", w->lines, w->columns);
		printf("cv1.txt:63\tx1 está com as dimensões %dx%d\r\n", x1->lines, x1->columns);
	#endif
//		DoubleMatrix *aux = DoubleMatrixBinOpColumnsPerColumn(w, x1, 0, false, Mult);
	#ifdef DEBUG_MATRIX_DIMENSIONS
		printf("cv1.txt:77\tw#x1 ficou com as dimensões %dx%d\r\n", aux->lines, aux->columns);
	#endif
//		DoubleMatrix *x1t= DoubleMatrixTranspose(x1, false);
		DoubleMatrix *x1t= DoubleMatrixTranspose(x1, false);
		DoubleMatrix *y1t= DoubleMatrixTranspose(y1, false);
		DoubleMatrix *x1t_w= DoubleMatrixBinOpColumnsPerColumn(x1, w, 0, false, Mult);
		DoubleMatrix *x1t_w_x1=DoubleMatrixBinOpColumnsPerColumn(x1t_w, x1, 0, false, Mult);
		DoubleMatrix *x1t_w_y1= DoubleMatrixBinOpColumnsPerColumn(x1t_w, y1, 0, false, Mult);
		DoubleMatrix *b;
//		DoubleMatrix *aux2;
//		aux2= aux;
	
//		aux= DoubleMatrixCopy(x1);
//		DoubleMatrixTranspose(aux, true);//#aux = x1'
	#ifdef DEBUG_MATRIX_DIMENSIONS
		printf("cv1.txt:77\tx1' ficou com as dimensões %dx%d\r\n", aux->lines, aux->columns);
	#endif
//		DoubleMatrix *aux3= DoubleMatrixMultiplication(aux, aux2);//aux3=x1`*(w#x1)
//		DoubleMatrix *aux4= DoubleMatrixElementBinaryOperation(w, y1, false, Mult);//aux3=w # y1
//		DoubleMatrix *aux4= Double
//		aux4= DoubleMatrixBinOpColumnsPerColumn(y1, w,0, false, Mult);//aux3=w # y1
	#ifdef DEBUG_MATRIX_DIMENSIONS
		printf("cv1.txt:77\tx1`*(w#x1#wt1) ficou com as dimensões %dx%d\r\n", aux3->lines, aux3->columns);
		printf("%s|%s:%d\t%s\n", __FILE__, __func__, __LINE__, (NULL == w)? "W é NULL": "W é válido.");
	#endif
		DoubleMatrix *C;
	//	fprintf(stderr, "%s|%s:%d\t aux2 is %dx%d\r\n", __FILE__, __func__, __LINE__, aux2->lines, aux2->columns);
	//	fprintf(stderr, "%s|%s:%d\t aux3 is %dx%d\r\n", __FILE__, __func__, __LINE__, aux3->lines, aux3->columns);
		if(0 == GWR_Determinant(x1t_w_x1))
		{
			b=NewDoubleMatrixAndInitializeElements(x->columns, 1, 0);
			C= NewDoubleMatrixAndInitializeElements(x->columns, 1, 0.);
		}
		else
		{
//			DoubleMatrix *aux4= DoubleMatrixBinOpColumnsPerColumn(w, x, 0, false, Mult);//aux = W#x
			//inv(x1`*(w#y1))*x1`*(w#y1);
//			DoubleMatrix *b= DoubleMatrixMultiplication(DoubleMatrixInverse(x1t_w_x1), x1t_w_y1);
			DoubleMatrix *_x1t_w_x1_t= DoubleMatrixTranspose(x1t_w_x1, false);
			DoubleMatrix *b= DoubleMatrixMultiplication(_x1t_w_x1_t, x1t_w_y1);
			//C=inv(x1`*(w#x1))*x1`#(w)`;
//			DoubleMatrix *w_x1= DoubleMatrixBinOpLinesPerLine(w, x1, 0, false, Mult);
			DoubleMatrix *wt/*t de transposto*/= DoubleMatrixTranspose(w, false);
//			DoubleMatrix *x1t__w_x1_= DoubleMatrixMultiplication(x1t, w_x1);
			DoubleMatrix *inv_x1t__w_x1__= DoubleMatrixInverse(x1t_w_x1);
			DoubleMatrix *w_y1= DoubleMatrixBinOpColumnsPerColumn(y1, w, 0, false, Mult);
			C= DoubleMatrixMultiplication(inv_x1t__w_x1__, x1t);
			DoubleMatrixBinOpLinesPerLine(C, wt, 0, true, Mult);
	#ifdef DEBUG_MATRIX_DIMENSIONS
		printf("cv1.txt:79\tinv(x1`*(w#x1#wt1)) ficou com as dimensões %dx%d\r\n", b->lines, b->columns);
	#endif
//			DeleteDoubleMatrix(aux3);
//			aux3= NULL;
//			aux3= DoubleMatrixMultiplication(b, aux);//aux3=inv(x1`*(w#x1))*x1`
	#ifdef DEBUG_MATRIX_DIMENSIONS
		printf("cv1.txt:79\tinv(x1`*(w#x1#wt1))*x1` ficou com as dimensões %dx%d\r\n", aux3->lines, aux3->columns);
	#endif
//			DeleteDoubleMatrix(b);
//			b= NULL;
//			b= DoubleMatrixMultiplication(aux3, aux4);
	#ifdef DEBUG_MATRIX_DIMENSIONS
		printf("cv1.txt:79\tb ficou com as dimensões %dx%d\r\n", b->lines, b->columns);
	#endif
		}
	#ifdef DEBUG_MATRIX_DIMENSIONS
		printf("%s|%s:%d\t%s\n", __FILE__, __func__, __LINE__, (NULL == w)? "W é NULL": "W é válido.");
	#endif
//		DeleteDoubleMatrix(aux3);
//		DeleteDoubleMatrix(aux2);
//		DeleteDoubleMatrix(aux);
//		aux3=NULL;
//		aux2=NULL;
//		aux=NULL;
	//fim Control C control+V Golden
		//linha 221 GWR cópia 1
//		aux= DoubleMatrixTranspose(C, false);
		DoubleMatrix *Ct= DoubleMatrixTranspose(C, false);
		DoubleMatrix *varb= DoubleMatrixMultiplication(C, Ct);
//		DeleteDoubleMatrix(aux);
//		aux=NULL;
		m1= (i-1)*x->columns+1;
		m2 = m1+x->columns-1;
//		for(int count =m1-1; count < m2;count++){
		for(int count =m1-1; count < varbi->lines;count++){
			DoubleMatrixSetElement(bi, count, 1-1, i);
			DoubleMatrixSetElement(bi, count, 2-1, b->elements[0]);
			DoubleMatrixSetElement(bi, count, 3-1, DoubleMatrixGetElement(args->data, i, args->x_dCoord));
			DoubleMatrixSetElement(bi, count, 4-1, DoubleMatrixGetElement(args->data, i, args->y_dCoord));
			DoubleMatrixSetElement(varbi, count, 1-1, DoubleMatrixGetElement(varb, count, count));
		}
//		for(int count =m1-1; count < varb->columns;count++){
//			DoubleMatrixSetElement(varbi, count, 1-1, DoubleMatrixGetElement(varb, count, count));
//		}
		DoubleMatrix *aux, *aux2, *aux3, *aux4;
		aux = NewLineDoubleMatrixFromMatrix(x, i);
		DoubleMatrix *teste= DoubleMatrixMultiplication(aux, b);
		if(teste->columns == teste->lines && teste->columns == 1){
			printf("%s|%s:%d\tok\n", __FILE__, __func__, __LINE__);
		}
		else
		{
			printf("%s|%s:%d\t[WARNING] esperado matriz 1x1\n", __FILE__, __func__, __LINE__);
		}
		DoubleMatrixSetElement(yhat, i, 1-1, teste->elements[0]);
//		yhat[i]= teste->elements[0];//atribuindo a um double uma Matriz...
		DeleteDoubleMatrix(aux);
		aux= NULL;
		DeleteDoubleMatrix(varb);//a variável varb está sendo calculada pra nada
		varb= NULL;
//	}//linha 232 GWR cópia 1
	DoubleMatrix *res= DoubleMatrixElementBinaryOperation(y, yhat, false, Sub);
	DoubleMatrix *stdbi= DoubleMatrixElementUnaryOperation(varbi, false, sqrt);
//	DoubleMatrix *aux=NULL, *aux2=NULL, *aux3=NULL;
	aux= NewColumnDoubleMatrixFromMatrix(bi, 2-1);
	DoubleMatrix *tstat = DoubleMatrixElementBinaryOperation(aux, stdbi, false, Div);//multiplicação pela inversa ou divisão element-wise?
	DeleteDoubleMatrix(aux);
	/***Global Estimates*linha 239 GWR copia 1*/
	aux= DoubleMatrixTranspose(x, false);
	aux2= DoubleMatrixMultiplication(aux, x);
	aux3= DoubleMatrixInverse(aux2);//aux3= inv(x' *x)
//	DoubleMatrix *aux4= DoubleMatrixCopy(aux3);
	DeleteDoubleMatrix(aux2);
	aux2= DoubleMatrixMultiplication(aux3, aux);//aux2= inv(x' *x)*x'
	DoubleMatrix *bg= DoubleMatrixMultiplication(aux2, y);
	DeleteDoubleMatrix(aux2);
	//linha 241 GWR copia 1
	aux2= DoubleMatrixMultiplication(x, bg);
	aux= DoubleMatrixElementBinaryOperation(y, aux2, false, Sub);//(y-x*bg)
	DoubleMatrix *s2g= DoubleMatrixTranspose(aux, false);//(y-x*bg)'
	for(int p=0; p < aux2->lines; p++)
	{
		for(int q=0; q < aux2->columns; q++)
		{
			DoubleMatrixSetElement(aux, p, q, DoubleMatrixGetElement(aux, p, q)/(n-bg->lines) );
		}
	}
	aux3= s2g;
	s2g= DoubleMatrixMultiplication(aux3, aux);
	DeleteDoubleMatrix(aux);
	DeleteDoubleMatrix(aux2);
	aux2= NULL;
	DeleteDoubleMatrix(aux3);
	aux3= NULL;
	aux= DoubleMatrixMultiplication(aux4, s2g);
	DoubleMatrix *varg= NewDoubleMatrix(1, aux4->columns);
	for(int count=0; count < varg->columns; count++){
		DoubleMatrixSetElement(varg, 1, count, DoubleMatrixGetElement(aux, count, count));
	}
	DeleteDoubleMatrix(aux);
	aux= NULL;
	DeleteDoubleMatrix(aux4);
	aux4= NULL;
	
	//linha 244 GWR cópia 1
	DoubleMatrix *stdg= DoubleMatrixElementUnaryOperation(varg, false, sqrt);
	DoubleMatrix *aux6= NewDoubleMatrixAndInitializeElements(bg->lines, bg->columns, stdg->elements[0]);
	DoubleMatrix *tg = DoubleMatrixElementBinaryOperation(bg, aux6, false, Div);
	//a linha 246 supõe a existência de uma matriz probt que não existe, pulando
	FILE *f= fopen("__res__.csv", "w");
	if(NULL == f)
	{
		printf("%s|%s:%d\t[ERROR]\r\n", __FILE__, __func__, __LINE__);
		exit(1);
	}
	fprintf(f, "\r\ny:\r\n");
	DoubleMatrixPrint(y, f, "\t%lf", "\r\n");
	fprintf(f, "\r\nyhat:\r\n");
	DoubleMatrixPrint(yhat, f, "\t%lf", "\r\n");
	fprintf(f, "\r\nres:\r\n");
	DoubleMatrixPrint(res, f, "\t%lf", "\r\n");
	fclose(f);
	
	f= fopen("__beta__.csv", "w");
	if(NULL == f)
	{
		printf("%s|%s:%d\t[ERROR]\r\n", __FILE__, __func__, __LINE__);
		exit(1);
	}
	fprintf(f, "\r\nid\t\tB\t\tx\t\ty\t\tstdbi\t\ttstat\r\n");
	DoubleMatrixPrint(bi, f, "\t%lf", "\r\n");
	fclose(f);

	//linha 252 GWR cópia 1
	return NULL;
}

#endif

