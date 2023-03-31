#ifndef NODE_H
#define NODE_H

//Estrutura para guardar as informações referentes à nossa aplicação de nó
typedef struct node {
    char self[32]; //Informacoes do proprio
    char ext[32];  //Informacoes do seu vizinho externo
    char bck[32];  //Informacoes do Nó de backup
    int num_ints;  //Numero de internos
    char intr[100][32]; //Informacoes dos varios vizinhos internos
} node;

#endif