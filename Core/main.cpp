#ifdef __cplusplus
extern "C"
{
#endif

#include "GuiInterface.h"

int main(int argc, char *argv[])
{
	return Gui_Init(argc, argv);
}

NamedColumnDoubleTable* LoadFile(char const *fileName, char separator)
{
	return NewNamedColumnDoubleTableFromFile(fileName, separator);
}


#ifdef __cplusplus
}
#endif
