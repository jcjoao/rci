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

//wsl hostname -i <-ip do wsl
//netcat [ip-address] [port] <-cliente TCP

typedef struct node
{
    char self[32];
    char ext[32];
    char bck[32];
    char intr[][32];
}node;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    struct node app;

    if (argc != 3) {
        printf("Wrong Arguments!\n");
        exit(1);
    }
    char *ip = argv[1]; //our ip
    char *TCP = argv[2]; //our port that is going to be used on the TCP server
    //char TCP[]="58000"; //our port that is going to be used on the TCP server
    //char ip[]="194.210.157.117"; //our ip

    //My variables
    char user_input[12];
    char net[4], id[3];
    char com[6];
    char regIP[]="193.136.138.142"; //IP of "Tejo"
    char regUDP[]="59000"; //port of "tejo"
    char id_to_connect[32]; //contact information of the node we want to connect to
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
    //FD_ZERO(&inputs); // Clear inputs
    //FD_SET(0,&inputs); // Set standard input channel on
    //FD_SET(fdTCP,&inputs);
    //FD_SET(fd_client,&inputs);

    while(1)
    {
        FD_ZERO(&inputs); // Clear inputs
        FD_SET(0,&inputs);
        FD_SET(fdTCP,&inputs);
        //FD_SET(fd_client,&inputs);

        for (i = 0; i < num_ints; i++) {
            FD_SET(fd_int[i],&inputs);
        }
        
        out_fds=select(FD_SETSIZE,&inputs,(fd_set *)NULL,(fd_set *)NULL,(struct timeval *) NULL);
        
        if(out_fds==-1){
            perror("select");
                exit(1);
        }
        else{
            if(FD_ISSET(0,&inputs)){
                FD_CLR(0,&inputs);
                fgets(user_input, 20, stdin);
                sscanf(user_input,"%s %s %s\n",com,net,id);
                    if(strlen(net)!=3 || strlen(id)!=2){
                        printf("Incorrect input!");
                        exit(0);
                    }
                sprintf(app.self,"%s %s %s",id,ip,TCP);
                if(strcmp(com,"join")==0){
                    joinpt1(net,id,regIP,regUDP,id_to_connect);

                    if(strcmp(id_to_connect,"empty")==0){
                        strcpy(app.ext,app.self);
                        strcpy(app.bck,app.self);
                        printf("%s\n", app.bck);
                    }
                    else{
                        strcpy(app.ext,id_to_connect);
                        sscanf(id_to_connect,"%s %s %s\n",bootid,bootIP,bootTCP);
                        fd_client = clientTCP(bootIP, bootTCP);
                        //FD_SET(fd_client,&inputs);
                        //Send welcome message with contact
                        sprintf(send,"NEW %s",app.self);
                        messageTCP(fd_client,send);
                        responseTCP(fd_client,recv);
                        sscanf(recv,"EXTERN %s",app.bck); //Read recieved message
                        printf("%s\n",recv);
                    }

                    joinpt2(net,id,regIP,regUDP,ip,TCP);
                }
                if(strcmp(com,"leave")==0){
                    leave(net,id,regIP,regUDP);
                }
            }
            for (i = 0; i < num_ints; i++) {
                if(FD_ISSET(fd_int[num_ints],&inputs)){
                    FD_CLR(fd_int[num_ints],&inputs);
                    printf("Mensagem TCP de um dos nós");
                }
            }
            if(FD_ISSET(fdTCP,&inputs)){
                FD_CLR(fdTCP,&inputs);
                printf("Novo Pedido Conexão\n");
                connectTCP(addr,addrlen,fdTCP,fd_int,&num_ints);
                responseTCP(fd_int[num_ints-1],recv);
                sscanf(recv,"NEW %s",app.intr[num_ints-1]);
                sprintf(send,"EXTERN %s",app.self);
                messageTCP(fd_int[num_ints-1],send);
                printf("Numero de internos:%d\n",num_ints);
                printf("%s\n",recv);
                FD_SET(fd_int[num_ints-1],&inputs);
            }
            /*
            if(FD_ISSET(fd_client,&inputs)){
                FD_CLR(fd_client,&inputs);
                printf("Mensagem do nó externo");
            }
            */
        }
        
    }
    //close everything
    for (i = 0; i < num_ints; i++) {
        close(fd_int[i]);
    }
    close(fd_client);
    close(fdTCP);
    return 0;
}

