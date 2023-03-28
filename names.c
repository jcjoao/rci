#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include "names.h"
#include "struct.h"
#include "udptcp.h"

//Função para criar novos nós para a lista de nome
name *create_node(char *name_str) {

    struct name *new_node = malloc(sizeof(name));

    //Verificação da correta alocação de memória
    if (new_node == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    strncpy(new_node->name, name_str, sizeof(new_node->name));
    new_node->next = NULL;

    return new_node;
}


//Função para adicionar nós da lista de nomes
void add_node(name **list_head, char *name_str) {

    struct name *aux = *list_head;

    if (*list_head == NULL) {

        *list_head = create_node(name_str);

        return;
    }

    while (aux->next != NULL) {
        if(strcmp(aux->name,name_str)==0){
            printf("O nome já se encontra adicionado.\n");
            return;
        }
        aux = aux->next;
    }

    if(strcmp(aux->name,name_str)==0){
        printf("O nome já se encontra adicionado.\n");
        return;
    }

    aux->next = create_node(name_str);

    return;
}

//Função para remover nós da lista de nomes
//Retorna 1 em caso de ter conseguido remover, e retorna 0 caso contrário
int remove_node(name **list_head, char *name_str) {
    
    struct name *prev = *list_head, *aux = NULL;
                
    if (*list_head == NULL) {
        printf("O nome não está presente no nó.\n");
        return 0;
    }

    aux = prev->next;

    if (strcmp((*list_head)->name, name_str) == 0) {

        *list_head = aux;
        free(prev);
        return 1;
    }

    while (aux != NULL) {
        if (strcmp(aux->name, name_str) == 0) {
            prev->next = aux->next;
            free(aux);
            return 1;
        }
        prev = aux;
        aux = aux->next;
    }

    printf("O nome não está presente no nó.\n");
    return 0;
}

//Função para libertar toda a memória alocada para a criação da lista de nomes
void freelist(name **list_head){
    
    struct name *aux = *list_head, *delete=NULL;

    while(aux != NULL){

        delete=aux;
        aux=aux->next;
        free(delete);
    }

    *list_head=NULL;
     
    return;
}

//Função para dar print de todos os conteúdos da lista de nomes
void showlist(name *list_head){

    struct name *aux = list_head;

    if(aux==NULL){
        printf("Não existe qualquer conteúdo adicionado a este nó.\n"); 
        return;
    }

    printf("Tem-se neste nó os seguintes conteúdos:\n");   

    while(aux != NULL){

        printf("%s\n",aux->name);

        aux=aux->next;
    }

    return;
}

//Função para dar print na tabela de expedição
void showtab(int *tab_exp){

    int aux=0, i=0;

    for (int i = 0; i < 100; i++) {

        if(tab_exp[i]!=-1){
            if(aux==0){
                printf("Tabela de expedição:\n");
            }
            printf("Destino: %d, Próximo nó: %d\n", i, tab_exp[i]);
            aux=1;
        }
    }

    if(aux==0){printf("A tabela de expedição do nó encontra-se vazia.\n");}

    return;
}

//Função para encontrar nós da lista de nomes
//Retorna 1 em caso de ter conseguido encontrar, e retorna 0 caso contrário
int search_node(name **list_head, char *name_str){
    
    struct name *aux = NULL;
                
    if (*list_head == NULL) {return 0;}

    aux = (*list_head)->next;

    if (strcmp((*list_head)->name, name_str) == 0) {return 1;}

    while (aux != NULL) {
        if (strcmp(aux->name, name_str) == 0) {

            return 1;
        }
        aux = aux->next;
    }
    return 0;
}

void forwaring(int *fd, int *tab_exp, char* msg, int before){
    int j;
    int dest;
    sscanf(msg, "%*s %d %*d %*s", &dest);

    if(tab_exp[dest]!=-1 && fd[tab_exp[dest]]!=-1){ //Ver se tem na tabela de expedição o destino
        messageTCP(fd[tab_exp[dest]],msg);         //Se tiver manda Query apenas para esse vizinho
    }else{ //Se não, manda para todos menos de quem recebeu
        for (j = 0; j < 100; j++){
            if(fd[j]!=-1 && j!=before){
                messageTCP(fd[j],msg);
            }
        }
    }
}
