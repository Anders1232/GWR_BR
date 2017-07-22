#ifdef __cplusplus
extern "C"
{
#endif

#include "GuiInterface.h"
#include "NamedColumnDoubleTable.h"
#include "locale.h"

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "C");
#ifdef _WIN32
	freopen ("ErrorLog.txt","w",stderr);
	freopen ("Log.txt","w",stdout);
#endif
	return Gui_Init(argc, argv);
}

NamedColumnDoubleTable* LoadFile(char const *fileName, char separator)
{
	return NewNamedColumnDoubleTableFromFile(fileName, separator);
}

void LoadPreview(FILE *preview, char const *fileName, int linesInPreview, int charPerLineInPreview)
{
	FILE *file= fopen(fileName, "r");
	int count1, count2;
	char temp;
	if(NULL == file)
	{
		fprintf(stderr, "%s:%d\t\t[ERROR] Error opening file", __FILE__, __LINE__);
		exit(1);
	}
	for(count1=0; count1 < linesInPreview; count1++)
	{
		for(count2=0; count2 < charPerLineInPreview; count2++)
		{
			temp= getc(file);
			if(EOF == temp || '\n' == temp)
			{
				break;
			}
			putc(temp, preview);
		}
		while(EOF != temp && '\n' != temp)
		{
			temp= getc(file);
		}
		putc('\n', preview);
	}
	fclose(file);
}

#ifdef __cplusplus
}
#endif
