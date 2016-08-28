#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"ColumnList.h"

#define MAX_FILENAME_SIZE (50)
#ifdef _WIN32
	#define NEW_LINE "\r\n"
#else
	#define NEW_LINE "\n"
#endif

//#define DEBUG

int main(int argc, char **argv)
{
	char file_name[MAX_FILENAME_SIZE+1];
	if(argc == 1)
	{
		printf("Please insert input file: ");
		scanf("%50[^\r\n]", file_name);
	}
	else
	{
		strncpy(file_name, argv[1], 50);
	}
	file_name[50]= '\0';
	FILE *file= fopen(file_name, "r");
	if(NULL == file)
	{
		fprintf(stderr, "Could not open the file %s"NEW_LINE, file_name);
		return 0;
	}
	long int fileSize;
	fseek (file , 0 , SEEK_END);
	fileSize = ftell (file);
	rewind (file);
	char *buffer= malloc(fileSize* sizeof(char));
	if(NULL == buffer)
	{
		fprintf(stderr, "Could not allocate enough memory to read the file %s"NEW_LINE, file_name);
		fprintf(stderr, "Buffer size asked:  %ld"NEW_LINE, fileSize);
		return 0;
	}
	bool fileHaveComma= false;
	fread (buffer,1,fileSize,file);
	for(int cont =0; cont < fileSize; cont++)
	{
		if(',' == buffer[cont])
		{
			fileHaveComma= true;
			buffer[cont]= '.';
		}
	}
	if(fileHaveComma)
	{
		fclose(file);
		file= tmpfile();
		if(NULL == file)
		{
			fprintf(stderr, "Could not create temporary file"NEW_LINE);
			return 0;
		}
		fwrite(buffer, fileSize, 1, file);
		rewind (file);
		free(buffer);
	}
#ifdef DEBUG
printf("checkpoint: %s\t\t%d"NEW_LINE , __FILE__, __LINE__);
#endif
	
	char columnName[MAX_COLUMN_NAME_SIZE+1];
	columnName[MAX_COLUMN_NAME_SIZE]= '\0';
	ColumnList *table;
	fscanf(file, "%*[ ]");
	fscanf(file, "%50[^\n ]", columnName);
	table= NewColumnList(columnName);
	bool onfirstLine= true;
	int numberOfColumns=1;
	while(onfirstLine)
	{
		fscanf(file, "%*[ ]");
		onfirstLine= 1==( fscanf(file, "%50[^\n ]", columnName) );
		if(onfirstLine)
		{
			numberOfColumns++;
			if(SUCCESS != AddColumn(table, columnName))
			{
				fprintf(stderr, "Deu ruim"NEW_LINE);//melhorar aqui depois
			}
		}
		else
		{
			onfirstLine= false;
		}
	}
#ifdef DEBUG
printf("checkpoint: %s\t\t%d"NEW_LINE , __FILE__, __LINE__);
printf("Number of columns: %d"NEW_LINE, numberOfColumns);
#endif
	
	double aux;
	bool fileEnded= false;
	while(!fileEnded)
	{
#ifdef DEBUG
printf("checkpoint: %s\t\t%d"NEW_LINE , __FILE__, __LINE__);
#endif
		for(int cont = 0; cont < numberOfColumns; cont++)
		{
			int result= fscanf(file, " %lf", &aux);
			if(EOF != result)
			{
#ifdef DEBUG
printf("checkpoint: %s\t\t%d"NEW_LINE , __FILE__, __LINE__);
printf("number read: %lf"NEW_LINE, aux);
#endif
				AppendDoubeOnColumnList(table, cont, aux);
			}
			else
			{
				fileEnded= true;
			}
		}
	}
#ifdef DEBUG
printf("checkpoint: %s\t\t%d"NEW_LINE , __FILE__, __LINE__);
#endif
	
	fclose(file);
	PrintColumnList(table, stdout, NEW_LINE, "\t\t%.4lf");
	DeleteColumnList(table);
	return 0;
}
