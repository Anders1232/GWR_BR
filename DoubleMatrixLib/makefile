flags= -Wall -pedantic -c

matrix.o: DoubleMatrixLib.c DoubleMatrixLib.h
	gcc DoubleMatrixLib.c $(flags)
clear:
	rm -r *.o
all:
	rm -r *.o
	gcc DoubleMatrixLib.c $(flags)
c90:
	gcc DoubleMatrixLib.c $(flags) -std=c90
c99:
	gcc DoubleMatrixLib.c $(flags) -std=c99
c11:
	gcc DoubleMatrixLib.c $(flags) -std=c11

