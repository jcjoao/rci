#ifndef __DJOIN__
#define __DJOIN__

#include "struct.h"

int djoin(node *app, char* id_to_connect);
void exitapp(int *fd,node *app);

#endif