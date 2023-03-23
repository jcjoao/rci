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
    app.num_ints=0;

    if (argc != 3) {
        printf("Wrong Arguments!\n");
        exit(1);
    }
    char *ip = argv[1]; //our ip
    char *TCP = argv[2]; //our port that is going to be used on the TCP server

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
    int j=0;
    int k=0;
    char idaux[3];

    int fd[100]; //array with sockets from nodes connected to our TCP server
    for(i=0;i<100;i++){fd[i]=-1;} //every element to -1

    struct sockaddr addr; 
    socklen_t addrlen;
    int fdTCP=serverTCP(TCP,addr,addrlen); //initiate TCP server
    int newfd;

    //Select Variables
    fd_set inputs;
    int out_fds;
    //struct timeval timeout;
    while(1)
    {
        //Inicializar variaveis do select
        FD_ZERO(&inputs);
        FD_SET(0,&inputs);
        FD_SET(fdTCP,&inputs);
        for (i = 0; i < 100; i++) {if(fd[i]!= -1 ){FD_SET(fd[i],&inputs);}}

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
                        fd[atoi(bootid)]=djoin(&app,id_to_connect);
                    }
                    joinpt2(net,id,regIP,regUDP,ip,TCP);
                }

                if(strcmp(com,"leave")==0){
                    leave(net,id,regIP,regUDP);
                    exitapp(fd,&app);
                }

                if(strcmp(com,"djoin")==0){
                    sscanf(user_input,"%s %s %s %s %s %s",com,net,id,bootid,bootIP,bootTCP);
                    sprintf(id_to_connect,"%s %s %s",bootid,bootIP,bootTCP);
                    if(strcmp(id,bootid)==0){
                        strcpy(app.ext,app.self);
                        strcpy(app.bck,app.self);                      
                    }
                    else{
                        fd[atoi(bootid)]=djoin(&app,id_to_connect);
                    }
                }
            }

            //Receber Novos Pedidos de Conexao
            if(FD_ISSET(fdTCP,&inputs)){
                printf("Novo Pedido Conexão\n");
                newfd = connectTCP(addr,addrlen,fdTCP);
                responseTCP(newfd,recv);
                sscanf(recv,"NEW %[^\n]",id_to_connect);
                //Nó estava sozinho
                if(strcmp(app.self,app.ext)==0){
                    strcpy(app.ext,id_to_connect);
                }else{
                    strcpy(app.intr[app.num_ints],id_to_connect);
                    app.num_ints++;
                }
                sprintf(send,"EXTERN %s",app.ext);
                messageTCP(newfd,send);
                printf("Numero de internos:%d\n",app.num_ints);
                //printf("%s\n",recv);
                sscanf(id_to_connect,"%s %s %s\n",bootid,bootIP,bootTCP);
                fd[atoi(bootid)]=newfd;
                printf("Vizinho Externo: %s\n",app.ext);
                printf("Nó de backup: %s\n",app.bck);
                FD_CLR(fdTCP,&inputs);
            }

            //Receber Novas Mensagens
            for (i = 0; i < 100; i++) {
            if(fd[i]!= -1 ){
            if(FD_ISSET(fd[i],&inputs)){
                printf("Mensagem TCP recebida, do nó com id:%d\n",i);
                responseTCP(fd[i],recv);
                if(strcmp(recv,"LEAVE")==0){
                    close(fd[i]);
                    fd[i]=-1;
                    //Ver se é externo
                    sscanf(app.ext,"%s",idaux);
                    if(atoi(idaux)==i){ //Mensagem de Vizinho externo
                        printf("Vizinho externo desconentou-se!\n");
                        if(strcmp(app.bck,app.self)!=0){//nao é ancora
                            strcpy(app.ext,app.bck); //Backup passa a ser o externo
                            sscanf(app.ext,"%s",idaux); //Buscar id do novo externo
                            fd[atoi(idaux)]=djoin(&app,app.ext);//Junta-se ao novo ext, recebe novo bck
                            //mandar mensagem a todos os internos com novo bck
                            for (j = 0; j < 100; j++){ //Percorrer array sockets
                            if(fd[j]!=-1 && j!=atoi(idaux)){ //os que nao forem -1, nem o externo (internos)
                                sprintf(send,"EXTERN %s",app.ext);
                                messageTCP(fd[j],send);
                            }}
                        }else{if(app.num_ints!=0){ //é ancora, escolhe novo ancora
                            strcpy(app.ext,app.intr[app.num_ints-1]);
                            if(fd[j]!=-1){ //os que nao forem -1, nem o externo (internos)
                                sprintf(send,"EXTERN %s",app.ext);
                                messageTCP(fd[j],send);
                            }
                            app.num_ints--;
                        }else{strcpy(app.ext,app.self);}
                        }
                    }else{
                        printf("Vizinho interno desconentou-se!\n");
                        for(j=0;j < app.num_ints;j++){ //percorrer vizinhos externos
                            sscanf(app.intr[j],"%s",idaux);
                            if(i==atoi(idaux)){        //quando encontra o interno que mandou msg
                                for(k=j;k<(app.num_ints-1);k++){
                                    strcpy(app.intr[k], app.intr[k+1]);
                                }
                                app.num_ints--;
                            }
                        }
                    }
                }
                sscanf(recv,"%s",com);
                if(strcmp(com,"EXTERN")==0){
                    printf("Mensagem com novo backup\n");
                    sscanf(recv,"EXTERN %[^\n]",app.bck);
                    printf("Novo nó de backup: %s\n",app.bck);
                }
                if(strcmp(com,"NEW")==0){
                    printf("Mensagem com novo Interno\n");
                    sscanf(recv,"NEW %[^\n]",app.intr[app.num_ints]);
                    app.num_ints++;
                }
            FD_CLR(fd[i],&inputs);               
            }}}

        }
    }

    close(fdTCP);
    return 0;
}

