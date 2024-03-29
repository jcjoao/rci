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
        printf("\x1b[31m[Error]\x1b[0m Error with socket");
        exit(1);}

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;//IPv4
    hints.ai_socktype=SOCK_STREAM;//TCP socket
    hints.ai_flags=AI_PASSIVE;

    if((errcode=getaddrinfo(NULL,regTCP,&hints,&res))!=0){
        printf("\x1b[31m[Error]\x1b[0m Error with errcode");
        /*error*/exit(1);}
    if(bind(fd,res->ai_addr,res->ai_addrlen)==-1){
        printf("\x1b[31m[Error]\x1b[0m Error with bind");
        /*error*/exit(1);}
    if(listen(fd,5)==-1){
        printf("\x1b[31m[Error]\x1b[0m Error with listen");
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

    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    fd=socket(AF_INET,SOCK_DGRAM,0);//UDP socket
    if(fd==-1)/*error*/exit(1);

    //Colocar timeout no socket
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0){
        printf("\x1b[31m[Error]\x1b[0m Error with setsockopt\n");
    }

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET;//IPv4
    hints.ai_socktype=SOCK_DGRAM;//UDP socket

    errcode=getaddrinfo(regIP,regUDP,&hints,&res);
    if(errcode!=0){
        printf("\x1b[31m[Error]\x1b[0m UDP Failed!");
        exit(1);}


    char buf[150];
    strncpy(buf, send, sizeof(buf));

    //Enviar Mensagem
    n=sendto(fd,buf,150,0,res->ai_addr,res->ai_addrlen);
    if(n==-1)/*error*/exit(1);
    
    //Receber resposta
    addrlen=sizeof(addr);
    memset(recv, 0, 150);
    n=recvfrom(fd,recv,150,0,&addr,&addrlen);
    if (n < 0) {
        if (errno == EWOULDBLOCK) {
            printf("\x1b[31m[Error]\x1b[0m Socket read timeout!\n");
            strcpy(recv,"TIMEOUT");
        }
        else{
            printf("\x1b[31m[Error]\x1b[0m Error with recvfrom!\n");
            exit(1);
        }
    }else{
        recv[n] = '\0';
    }

    //recv[n] = '\0';
    //printf(": %s\n", recv);


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
    if(n!=0){printf("\x1b[31m[Error]\x1b[0m Error with getaddrinfo\n");exit(1);}
    n=connect(fd,res->ai_addr,res->ai_addrlen);
    if(n==-1){printf("\x1b[31m[Error]\x1b[0m Error with Connect\n");exit(1);}

    freeaddrinfo(res);

    return fd;
}

int messageTCP(int fd, char* send){
    ssize_t nbytes,nleft,nwritten;
    char *ptr;
    ptr=send;
    nbytes = strlen(send);
    nleft = nbytes;
    while (nleft > 0) {
    nwritten = write(fd, ptr, nleft);
    if (nwritten <= 0) {exit(1);}
    nleft -= nwritten;
    ptr += nwritten;
    }
    //if (write(fd, send, 128) == -1) {exit(1);}
    return 0;
}

int responseTCP(int fd,char* recv){
    //Esta função não é utilizada por ser uma versão inferior da funcao "newresponse" encontrada a baixo
    //Manteve-se a funcao aqui pois pode ser util para casos mais gerais, pois recebe a mensagem completa
    //enquanto o newresponse fica à espera do caracter /n
    int socket_state;
    memset(recv, 0, 150);
    socket_state = read(fd, recv, 150);
    if (socket_state == -1) {exit(1);}
    //Caso esteja Vazio
    if(socket_state == 0){
        strcpy(recv,"LEAVE\n");
    }
    
    return 0;
}

int newresponseTCP(int fd,char* recv){
    //Versão avancada do responseTCP, recebe mensagens, mas só le ate encontrar um /n
    char c;
    int bytes_read = 0;
    int aux;
    memset(recv, 0, 150);
    while ( (aux=read(fd, &c, 1)) == 1) {
        //Quando encontra /n retorna
        if (c == '\n') {
            recv[bytes_read] = '\n';
            return bytes_read;
        } else {
            recv[bytes_read] = c;
            bytes_read++;
            //Se uma mensagem tem 150 caracteres em aparecer um /n algo está mal!
            if (bytes_read >= 150) {
                fprintf(stderr, "\x1b[31m[Error]\x1b[0m Mensagem Incorreta!\n");
                return -1;
            }
        }
    }
    if (aux < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("\x1b[31m[Error]\x1b[0m Socket read timeout!\n");
            return -1;
        } else {
            perror("\x1b[31m[Error]\x1b[0m Read Error\n");
        }
    }
    //Se nada é lido, é porque foi dado leave
    if(bytes_read==0){
        strcpy(recv,"LEAVE\n");
    }
    return 0;
}

int connectTCP(struct sockaddr addr,socklen_t addrlen,int fdTCP){
    int newfd;

    struct timeval timeout;
    timeout.tv_sec = 2; // 5 seconds
    timeout.tv_usec = 0;

    addrlen=sizeof(addr);
    if((newfd=accept(fdTCP,&addr,&addrlen))==-1)exit(1);
    if (setsockopt(newfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        printf("\x1b[31m[Error]\x1b[0m Error with setsockopt\n");
    }
    return newfd;
}