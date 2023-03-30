#ifndef __DJOIN__
#define __DJOIN__

#include "struct.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: djoin
 * Description: Faz djoin. Envia Mensagem de New, Recebe mensagem de Extern
 *              atualiza nó externo e de backup
 *              Retorna Socket da conexão
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int djoin(node *app, char* id_to_connect);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: read_input
 * Description: Fecha Sockets, isola nó
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void exitapp(int *fd,node *app);

#endif