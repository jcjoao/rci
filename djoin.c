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
    char bootid[3];  //id of the node we want to connect to
    char bootIP[16]; //IP of the node we want to connect to
    char bootTCP[6]; //TCP port of the node we want to connect to
    char recv[64]; //string to send messages
    char send[64]; //string to recieve messages

    sscanf(id_to_connect,"%s %s %s\n",bootid,bootIP,bootTCP);

    strcpy(app->ext,id_to_connect);
    fd_client = clientTCP(bootIP, bootTCP);
    //Send welcome message with contact
    sprintf(send,"NEW %s",app->self);
    messageTCP(fd_client,send);
    responseTCP(fd_client,recv);
    sscanf(recv,"EXTERN %[^\n]",app->bck); //Read recieved message
    printf("Mensagem recebida: %s\n",recv);

    return fd_client;
}

int exitapp(int *fd_int,int *num_ints, int *fd_client, node *app, int *ifclient){
    int i;
    for (i = 0; i < *num_ints; i++) {
        strcpy(app->intr[i],"\0");
        close(fd_int[i]);
    }
    *num_ints=0;
    strcpy(app->bck, "\0");
    strcpy(app->ext, "\0");
    if(*ifclient==1){close(*fd_client);}
    *ifclient=0;
}
