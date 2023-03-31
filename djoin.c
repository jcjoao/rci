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

#include "djoin.h"
#include "udptcp.h"
#include "struct.h"


int djoin(node *app, char* id_to_connect){
    int fd_client;
    char bootid[3];  //id do nó a que nos queremos conectar
    char bootIP[16]; //IP do nó a que nos queremos conectar
    char bootTCP[6]; //TCP do nó a que nos queremos conectar
    char recv[150]; //string para receber mensagens
    char send[150]; //string para enviar mensagens

    sscanf(id_to_connect,"%s %s %s\n",bootid,bootIP,bootTCP);

    strcpy(app->ext,id_to_connect);
    //conectar com o Nó
    fd_client = clientTCP(bootIP, bootTCP);
    //Mandar welcome message com o nosso contacto
    sprintf(send,"NEW %s\n",app->self);
    messageTCP(fd_client,send);
    //Receber mensagem de EXTERN com o nó de backup e sei contacto
    newresponseTCP(fd_client,recv);
    sscanf(recv,"EXTERN %[^\n]",app->bck); //Read recieved message
    //printf("\x1b[32m[Info]\x1b[0m Mensagem recebida: %s\n",recv);
    printf("\x1b[32m[Info]\x1b[0m Novo Nó de backup: %s\n", app->bck);
    return fd_client;
}

void exitapp(int *fd,node *app){
    int i;
    //Fechar todos os sockets ativos
    for (i = 0; i < 100; i++) {
        if(fd[i]!=-1){
            if(close(fd[i])==-1){
                printf("\x1b[31m[Error]\x1b[0m Não foi possivel fechar o socket\n");
            }
            fd[i]=-1;
        }
    }
    //Colocar numero de internos a 0, atualizar bck e ext para ele proprio
    //Se sai fica sozinho na rede
    app->num_ints=0;
    strcpy(app->bck, app->self);
    strcpy(app->ext, app->self);
    return;
}
