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


//wsl hostname -i <-ip do wsl
//netcat [ip-address] [port] <-cliente TCP

typedef struct node
{
    int net;
    int id;
    int port;
    struct node *ext;
    struct node *bck;
    struct node *intr;
}node;

int main(int argc, char *argv[ ])
{
    srand(time(NULL));

    //My variables
    char user_input[12];
    char net[4], id[3];
    char com[6];
    char regIP[]="193.136.138.142";
    char regUDP[]="59000";
    char regTCP[]="58000";
    char ip[]="194.210.157.117";
    char id_to_connect[32];

    //servidor TCP
    
    int newfd;
    ssize_t n,nw;
    char *ptr,buffer[128];
    struct sockaddr addr; 
    socklen_t addrlen;
    int fdTCP=serverTCP(regTCP,addr,addrlen);

    //Select Variables
    fd_set inputs, testfds;
    int out_fds;
    //struct timeval timeout;
    FD_ZERO(&inputs); // Clear inputs
    FD_SET(0,&inputs); // Set standard input channel on
    FD_SET(fdTCP,&inputs);

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
                //printf("%s,%s,%s\n",com,net,id);
                if(strcmp(com,"join")==0||strcmp(com,"leave")==0){
                    if(strlen(net)!=3 || strlen(id)!=2){
                        printf("Incorrect input!");
                        exit(0);
                    }
                }
                join(com,net,id,regIP,regUDP,ip,regTCP,id_to_connect);
            }
            if(FD_ISSET(fdTCP,&testfds)){
                printf("recebeu mensagem tcp: ");
                addrlen=sizeof(addr);
                if((newfd=accept(fdTCP,&addr,&addrlen))==-1)exit(1);
                while((n=read(newfd,buffer,128))!=0){if(n==-1)exit(1);
                ptr=&buffer[0];
                printf("%s\n",buffer);
                while(n>0){if((nw=write(newfd,ptr,n))<=0)exit(1);
                n-=nw; ptr+=nw;}
                }
                close(newfd);
            }
        }
        
    }
    close(fdTCP);
    return 0;
}

