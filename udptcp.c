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

int serverTCP(char* regTCP,struct sockaddr addr,socklen_t addrlen){
    struct addrinfo hints,*res;
    int fd,errcode; 

    if((fd=socket(AF_INET,SOCK_STREAM,0))==-1){
        printf("Error with socket");
        exit(1);}

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;//IPv4
    hints.ai_socktype=SOCK_STREAM;//TCP socket
    hints.ai_flags=AI_PASSIVE;

    if((errcode=getaddrinfo(NULL,regTCP,&hints,&res))!=0){
        printf("Error with errcode");
        /*error*/exit(1);}
    if(bind(fd,res->ai_addr,res->ai_addrlen)==-1){
        printf("Error with bind");
        /*error*/exit(1);}
    if(listen(fd,5)==-1){
        printf("Error with listen");
        /*error*/exit(1);}
    freeaddrinfo(res);

    return fd;
}

int clientUDP(char* regIP,char* regUDP, char* send, char* recv){
    int fd,errcode;
    struct addrinfo hints,*res;
    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;

    fd=socket(AF_INET,SOCK_DGRAM,0);//UDP socket
    if(fd==-1)/*error*/exit(1);

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;//IPv4
    hints.ai_socktype=SOCK_DGRAM;//UDP socket

    errcode=getaddrinfo("tejo.tecnico.ulisboa.pt",regUDP,&hints,&res);
    if(errcode!=0){
        printf("UDP Failed!");
        exit(1);}

    //Enviar Mensagem
    n=sendto(fd,send,128,0,res->ai_addr,res->ai_addrlen);
    if(n==-1)/*error*/exit(1);
    
    //Receber resposta
    addrlen=sizeof(addr);
    n=recvfrom(fd,recv,128,0,&addr,&addrlen);
    if(n==-1)/*error*/exit(1);

    recv[n] = '\0';
    printf("echo: %s\n", recv);


    freeaddrinfo(res);
    close(fd);
    return 0;
}

int clientTCP(char* bootIP, char* bootTCP){
    struct addrinfo hints,*res;
    int fd,n;

    fd=socket(AF_INET,SOCK_STREAM,0);//TCP socket
    if(fd==-1){printf("Error with Socket\n");exit(1);}//error
    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;//IPv4
    hints.ai_socktype=SOCK_STREAM;//TCP socket
    n=getaddrinfo(bootIP,bootTCP,&hints,&res);
    if(n!=0){printf("Error with getaddrinfo\n");exit(1);}
    n=connect(fd,res->ai_addr,res->ai_addrlen);
    if(n==-1){printf("Error with Connect\n");exit(1);}

    return fd;
}

int messageTCP(int fd, char* send){
    if (write(fd, send, 128) == -1) {exit(1);}
    return 0;
}

int responseTCP(int fd,char* recv){
    if (read(fd, recv, 128) == -1) {exit(1);}
    return 0;

}

int connectTCP(struct sockaddr addr,socklen_t addrlen,int fdTCP, int* fd_int, int* num_ints){
    //int newfd;

    addrlen=sizeof(addr);
    //if((newfd=accept(fdTCP,&addr,&addrlen))==-1)exit(1);
    //fd_int[*num_ints]=newfd;
    if((fd_int[*num_ints]=accept(fdTCP,&addr,&addrlen))==-1)exit(1);
    (*num_ints)++;

    //close(newfd);
    return 0;
}