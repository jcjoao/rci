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

#include "udptcp.h"
#include "join.h"
#include "djoin.h"
#include "struct.h"

//wsl hostname -i <-ip do wsl
//netcat [ip-address] [port] <-cliente TCP

int main(int argc, char *argv[])
{
    srand(time(NULL));
    node app;

    if (argc != 3) {
        printf("Wrong Arguments!\n");
        exit(1);
    }
    char *ip = argv[1]; //our ip
    char *TCP = argv[2]; //our port that is going to be used on the TCP server
    //char TCP[]="58000"; //our port that is going to be used on the TCP server
    //char ip[]="194.210.157.117"; //our ip

    //My variables
    char user_input[16];
    char net[4], id[3];
    char com[6];
    char regIP[]="193.136.138.142"; //IP of "Tejo"
    char regUDP[]="59000"; //port of "tejo"
    char id_to_connect[33]; //contact information of the node we want to connect to
    char bootid[3];  //id of the node we want to connect to
    char bootIP[16]; //IP of the node we want to connect to
    char bootTCP[6]; //TCP port of the node we want to connect to
    char recv[46]; //string to send messages
    char send[46]; //string to recieve messages
    int i=0;

    int fd_int[99]; //array with sockets from nodes connected to our TCP server
    int num_ints = 0;//number of nodes connected to our TCP server

    struct sockaddr addr; 
    socklen_t addrlen;
    int fdTCP=serverTCP(TCP,addr,addrlen); //initiate TCP server

    int fd_client; //socket to connect with another node's TCP server

    //Select Variables
    fd_set inputs;
    int out_fds;
    //struct timeval timeout;

    int ifclient = 0;

    while(1)
    {
        //Inicializar variaveis do select
        FD_ZERO(&inputs); // Clear inputs
        FD_SET(0,&inputs);
        FD_SET(fdTCP,&inputs);
        if(ifclient==1){FD_SET(fd_client,&inputs);}
        for (i = 0; i < num_ints; i++) {FD_SET(fd_int[i],&inputs);}
        
        out_fds=select(FD_SETSIZE,&inputs,(fd_set *)NULL,(fd_set *)NULL,(struct timeval *) NULL);
        
        if(out_fds==-1){
            perror("select");
                exit(1);
        }
        else{
            //Receber inputs no teclado
            if(FD_ISSET(0,&inputs)){
                FD_CLR(0,&inputs);
                fgets(user_input, 64, stdin);
                sscanf(user_input,"%s %s %s",com,net,id);
                if(strlen(net)!=3 || strlen(id)!=2){
                    printf("Incorrect input!");
                    exit(0);
                }
                sprintf(app.self,"%s %s %s",id,ip,TCP);

                if(strcmp(com,"join")==0){
                    joinpt1(net,id,regIP,regUDP,id_to_connect);
                    sscanf(id_to_connect,"%s %s %s\n",bootid,bootIP,bootTCP);
                    if(strcmp(id,bootid)==0){
                        strcpy(app.ext,app.self);
                        strcpy(app.bck,app.self);                      
                    }
                    else{
                        fd_client=djoin(&app,id_to_connect);
                    }
                    FD_SET(fd_client,&inputs);
                    ifclient=1;
                    joinpt2(net,id,regIP,regUDP,ip,TCP);
                }

                if(strcmp(com,"leave")==0){
                    leave(net,id,regIP,regUDP);
                    exitapp(fd_int, &num_ints, &fd_client,&app,&ifclient);
                }

                if(strcmp(com,"djoin")==0){
                    sscanf(user_input,"%s %s %s %s %s %s",com,net,id,bootid,bootIP,bootTCP);
                    sprintf(id_to_connect,"%s %s %s",bootid,bootIP,bootTCP);
                    
                    if(strcmp(id,bootid)==0){
                        strcpy(app.ext,app.self);
                        strcpy(app.bck,app.self);                      
                    }
                    else{
                        fd_client=djoin(&app,id_to_connect);
                    }
                    printf("app.ext: %s\n",app.ext);
                    printf("app.bck: %s\n",app.bck);
                    FD_SET(fd_client,&inputs);
                    ifclient=1;
                }
            }

            //Receber Mensagens dos Nos Internos
            for (i = 0; i < num_ints; i++) {
                if(FD_ISSET(fd_int[i],&inputs)){
                    FD_CLR(fd_int[i],&inputs);
                    printf("Mensagem TCP de um dos nós internos\n");
                    responseTCP(fd_int[i],recv);
                    if(strcmp(recv,"LEAVE")==0){
                        printf("Nó interno desconentou-se\n");
                        close(fd_int[i]);
                        num_ints--;
                    }
                }
            }

            //Receber Novos Pedidos de Conexao
            if(FD_ISSET(fdTCP,&inputs)){
                FD_CLR(fdTCP,&inputs);
                printf("Novo Pedido Conexão\n");
                connectTCP(addr,addrlen,fdTCP,fd_int,&num_ints);
                responseTCP(fd_int[num_ints-1],recv);
                sscanf(recv,"NEW %[^\n]",app.intr[num_ints-1]);
                if(strcmp(app.self,app.ext)==0){
                    strcpy(app.ext,app.intr[num_ints-1]);
                }
                sprintf(send,"EXTERN %s",app.ext);
                messageTCP(fd_int[num_ints-1],send);
                printf("Numero de internos:%d\n",num_ints);
                //printf("%s\n",recv);
                printf("app.ext: %s\n",app.ext);
                printf("app.bck: %s\n",app.bck);
                FD_SET(fd_int[num_ints-1],&inputs);
            }

            //Receber Mensagens de nós externos
            if(ifclient==1){
            if(FD_ISSET(fd_client,&inputs)){
                FD_CLR(fd_client,&inputs);
                printf("Mensagem do nó externo");
                responseTCP(fd_client,recv);
                
                if(strcmp(recv,"LEAVE")==0){
                    printf("Nó externo desconentou-se\n");
                    strcpy(app.ext,app.bck);
                    close(fd_client);
                    strcpy(id_to_connect,app.ext);
                    fd_client=djoin(&app,id_to_connect);
                    printf("Novo nó exterior: %s\n",app.ext);
                    printf("Novo nó de recuperação: %s\n",app.bck);
                    for (i = 0; i < num_ints; i++) {
                        sprintf(send,"EXTERN %s",app.ext);
                        messageTCP(fd_int[i],send);
                    }
                }else{
                    sscanf(recv,"%s",com);
                    if(strcmp(com,"EXTERN")==0){
                        printf("Mensagem com novo backup\n");
                        sscanf(recv,"EXTERN %[^\n]",app.bck);
                        printf("Novo nó de backup: %s\n",app.bck);
                    }
                }
            }}

        }
    }
    //close everything
    /*
    for (i = 0; i < num_ints; i++) {
        close(fd_int[i]);
    }
    close(fd_client);
    */
    close(fdTCP);
    return 0;
}

