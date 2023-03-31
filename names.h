#ifndef __NAMES__
#define __NAMES__

#include "struct.h"
#include "udptcp.h"

//Estrutura para os nós da lista de nomes
typedef struct name
{
    char name[100];
    struct name *next;
}name;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: create_node
 * Description: Criar novos nós na lista de nomes
 *
 * Parameter: string com a informação do nome name a adicionar
 * Return: pointer para o novo nó criado da lista de nomes
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
name *create_node(char *name_str);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: add_node
 * Description: Função para adicionar nós da lista de nomes
 * 
 * Parameter 1: pointer head da lista de nomes
 * Parameter 2: string com a informação do nome name a adicionar
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void add_node(name **list_head, char *name_str);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: remove_node
 * Description: Função para remover nós da lista de nomes
 * 
 * Parameter 1: pointer head da lista de nomes
 * Parameter 2: string com a informação do nome name a remover 
 * Return: 1 caso tenha conseguido remover, e 0 caso contrário
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int remove_node(name **list_head, char *name_str);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: freelist
 * Description: Função para libertar a memória alocada associada à lista de nomes
 * 
 * Parameter: pointer head da lista de nomes
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void freelist(name **list_head);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: showlist
 * Description: Função para mostrar lista de nomes
 * 
 * Parameter: pointer head da lista de nomes
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void showlist(name *list_head);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: showtab
 * Description: Função mostrar tabela de expedição
 * 
 * Parameter: pointer para o vetor da tabela de expedição
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
 * Parameter 1: socket da ligação associada ao nó a que queremos reencaminhar a mensagem
 * Parameter 2: pointer para o vetor da tabela de expedição
 * Parameter 3: string com informação da mensagem a ser reenviada
 * Parameter 4: id do nó que enviou a mensagem ao nosso
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void forwaring(int *fd, int *tab_exp, char* msg, int before);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: showlist
 * Description: Função para encontrar nós da lista de nomes
 * 
 * Parameter 1: pointer head da lista de nomes
 * Parameter 2: string com a informação do nome name a procurar
 * Return: 1 caso tenha conseguido encontrar, e 0 em caso contrário
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int search_node(name **list_head, char *name_str);

#endif