#ifndef __DJOIN__
#define __DJOIN__

#include "struct.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: djoin
 * Description: Faz djoin. Envia Mensagem de New, Recebe mensagem de Extern
 *              atualiza nó externo e de backup
 *
 * Parameter 1: estrutura com informações da nossa aplicação de nó
 * Parameter 2: string com informação do id, IP e porto do nó a que nos queremos conectar
 * Return: socket da conexão TCP estabelecida
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int djoin(node *app, char* id_to_connect);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: read_input
 * Description: Fecha Sockets, isola nó
 *
 * Parameter 1: socket da ligação TCP que queremos fechar
 * Parameter 2: estrutura com informações da nossa aplicação de nó
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void exitapp(int *fd,node *app);

#endif