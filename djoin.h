#ifndef __DJOIN__
#define __DJOIN__

#include "struct.h"

int djoin(node *app, char* id_to_connect);
int exitapp(int *fd_int,int *num_ints, int *fd_client, node *app, int *ifclient);

#endif