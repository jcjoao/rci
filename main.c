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

int main(int argc, char *argv[ ])
{
    srand(time(NULL));
    struct node app;
    
    //My variables
    char user_input[12];
    char net[4], id[3];
    char com[6];
    char regIP[]="193.136.138.142"; //IP of "Tejo"
    char regUDP[]="59000"; //port of "tejo"
    char TCP[]="58000"; //our port that is going to be used on the TCP server
    char ip[]="194.210.157.117"; //our ip
    char id_to_connect[32]; //contact information of the node we want to connect to
    char bootid[3];  //id of the node we want to connect to
    char bootIP[16]; //IP of the node we want to connect to
    char bootTCP[6]; //TCP port of the node we want to connect to
    char recv[36]; //string to send messages
    char send[46]; //string to recieve messages
    int i;

    int fd_int[99]; //array with sockets from nodes connected to our TCP server
    int num_ints = 0;//number of nodes connected to our TCP server

    struct sockaddr addr; 
    socklen_t addrlen;
    int fdTCP=serverTCP(TCP,addr,addrlen); //initiate TCP server

    int fd_client; //socket to connect with another node's TCP server

    //Select Variables
    fd_set inputs, testfds;
    int out_fds;
    //struct timeval timeout;
    FD_ZERO(&inputs); // Clear inputs
    FD_SET(0,&inputs); // Set standard input channel on
    FD_SET(fdTCP,&inputs);
    FD_SET(fd_client,&inputs);

    while(1)
    {
        testfds=inputs;
        
        out_fds=select(FD_SETSIZE,&testfds,(fd_set *)NULL,(fd_set *)NULL,(struct timeval *) NULL);
        
        if(out_fds==-1){
            perror("select");
                exit(1);
        }
        else{
            if(FD_ISSET(0,&testfds)){
                fgets(user_input, 20, stdin);
                sscanf(user_input,"%s %s %s\n",com,net,id);
                    if(strlen(net)!=3 || strlen(id)!=2){
                        printf("Incorrect input!");
                        exit(0);
                    }
                sprintf(app.self,"%s %s %s",id,ip,TCP);
                if(strcmp(com,"join")==0){
                    join(net,id,regIP,regUDP,ip,TCP,id_to_connect);
                    if(strcmp(id_to_connect,"empty")==0){
                        strcpy(app.ext,app.self);
                        strcpy(app.bck,app.self);
                    }
                    else{
                        strcpy(app.ext,id_to_connect);
                        sscanf(id_to_connect,"%s %s %s\n",bootid,bootIP,bootTCP);
                        fd_client = clientTCP(bootIP, bootTCP);
                        //Send welcome message with contact
                        sprintf(send,"NEW %s",app.self);
                        messageTCP(fd_client,send);
                        responseTCP(fd_client,recv);
                        sscanf(recv,"EXTERN %s",app.bck); //Read recieved message
                    }
                }
                if(strcmp(com,"leave")==0){
                    leave(net,id,regIP,regUDP);
                }
            }
            if(FD_ISSET(fdTCP,&testfds)){
                printf("Novo Pedido Conexão\n");
                connectTCP(addr,addrlen,fdTCP,fd_int,&num_ints);
                responseTCP(fd_int[num_ints-1],recv);
                sscanf(recv,"NEW %s",app.intr[num_ints-1]);
                sprintf(send,"EXTERN %s",app.self);
                messageTCP(fd_int[num_ints-1],send);
                FD_SET(fd_int[num_ints-1],&inputs);
            }
            for (i = 0; i < num_ints; i++) {
                if(FD_ISSET(fd_int[num_ints],&testfds)){
                    printf("Mensagem TCP de um dos nós");
                }
            }
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

