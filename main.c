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
#define regUDP "59000"
#define regIP "193.136.138.142"


int join(char* com, char* net,char* id,char* tejo_ip,char* portTEJO,char* ip, char* porta);

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
    //Select Variables
    fd_set inputs, testfds;
    //struct timeval timeout;
    //int i,out_fds,n,errcode;
    int out_fds;
    FD_ZERO(&inputs); // Clear inputs
    FD_SET(0,&inputs); // Set standard input channel on
    //My variables
    char user_input[12];
    char net[4], id[3];
    char com[6];
    char tejo_ip[]="193.136.138.142";
    char porta[]="69";
    char portTEJO[]="59000";
    char ip[]="193.136.138.142";

    while(1)
    {
        testfds=inputs;
        
        out_fds=select(FD_SETSIZE,&testfds,(fd_set *)NULL,(fd_set *)NULL,(struct timeval *) NULL);
        
        if(out_fds==-1){
            perror("select");
                exit(1);
        }
        else{
            if(FD_ISSET(0,&testfds))
                {
                    fgets(user_input, 20, stdin);
                    sscanf(user_input,"%s %s %s\n",com,net,id);
                    //printf("%s,%s,%s\n",com,net,id);
                    if(strlen(net)!=3 || strlen(id)!=2){
                        printf("Incorrect input!");
                        exit(0);
                    }
                    join(com,net,id,tejo_ip,portTEJO,ip, porta);
                }
        }
        
    }
    
    return 0;
}

int join(char* com, char* net,char* id,char* tejo_ip,char* portTEJO,char* ip, char* porta){
        //REG net id ip_da_maquina portaTCP
    struct addrinfo hints,*res;
    struct sockaddr addr;
    socklen_t addrlen;
    int fd,errcode;
    ssize_t n;
    char buffer[128+1];
    char buff[128];

    if(strcmp(com,"join")==0){
        sprintf(buff,"REG %s %s %s %s\n",net,id,ip,porta);
        printf("%s\n",buff);
    }
    else{
        if(strcmp(com,"leave")==0){
            sprintf(buff,"UNREG %s %s\n",net,id);
            printf("%s\n",buff);
        }
        else{
            exit(0);
        }
    }

    fd=socket(AF_INET,SOCK_DGRAM,0);//UDP socket
    if(fd==-1)/*error*/exit(1);

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;//IPv4
    hints.ai_socktype=SOCK_DGRAM;//UDP socket

    errcode=getaddrinfo("tejo.tecnico.ulisboa.pt",portTEJO,&hints,&res);
    if(errcode!=0)/*error*/exit(1);

    n=sendto(fd,buff,128,0,res->ai_addr,res->ai_addrlen);
    if(n==-1)/*error*/exit(1);
    
    //Receber resposta
    addrlen=sizeof(addr);
    n=recvfrom(fd,buffer,128,0,&addr,&addrlen);
    if(n==-1)/*error*/exit(1);

    buffer[n] = '\0';
    printf("echo: %s\n", buffer);
    close(fd);

    freeaddrinfo(res);
    
    return 0;
}
