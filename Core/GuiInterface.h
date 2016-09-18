#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include "NamedColumnDoubleTable.h"

//GUI side-functions
int Gui_Init(int argc, char *argv[]);

//Core program logic side functions
NamedColumnDoubleTable* LoadFile(char const *fileName, char separator);


#endif
