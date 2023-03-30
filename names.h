#ifndef __NAMES__
#define __NAMES__

#include "struct.h"
#include "udptcp.h"

typedef struct name
{
    char name[100];
    struct name *next;
}name;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: create_node
 * Description: Criar novos nós na lista de nomes
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
name *create_node(char *name_str);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: add_node
 * Description: Função para adicionar nós da lista de nomes
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void add_node(name **list_head, char *name_str);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: remove_node
 * Description: Função para remover nós da lista de nomes
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int remove_node(name **list_head, char *name_str);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: freelist
 * Description: Função para remover nós da lista de nomes
 *              Retorna 1 em caso de ter conseguido remover, e retorna 0 caso contrário
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void freelist(name **list_head);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: showlist
 * Description: Função para mostrar lista de nomes
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void showlist(name *list_head);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: showtab
 * Description: Função mostrar tabela de expedicao
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void showtab(int *tab_exp);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: forwarding
 * Description: Função utilizada em mensagens QUERY NOCONTENT CONTENT
 *              Envia para o vizinho da tabela de expedicao que tem
 *              o destino, caso nao exista, envia a todos menos a quem
 *              lhe enviou
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void forwaring(int *fd, int *tab_exp, char* msg, int before);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: showlist
 * Description: Função para encontrar nós da lista de nomes
 *              Retorna 1 em caso de ter conseguido encontrar, e retorna 0 caso contrário
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int search_node(name **list_head, char *name_str);

#endif