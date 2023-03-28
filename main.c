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
#include "names.h"

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
    char com[12];
    char com2[32];
    char name[32];
    char regIP[]="193.136.138.142"; //IP of "Tejo"
    char regUDP[]="59000"; //port of "tejo"
    char id_to_connect[33]; //contact information of the node we want to connect to
    char bootid[3];  //id of the node we want to connect to
    char bootIP[16]; //IP of the node we want to connect to
    char bootTCP[6]; //TCP port of the node we want to connect to
    char recv[64]; //string to send messages
    char send[64]; //string to recieve messages
    int i=0;
    int j=0;
    int k=0;
    char idaux[3];
    int idauxint;
    char dest[3];
    char origin[3];

    int tab_exp[100]; //matriz para a tabela de expedição
    int fd[100]; //array with sockets from nodes connected to our TCP server
    for(i=0;i<100;i++){fd[i]=-1;tab_exp[i]=-1;} //Inicializar todos os elementos a -1

    struct sockaddr addr; 
    socklen_t addrlen;
    int fdTCP=serverTCP(TCP,addr,addrlen); //initiate TCP server
    int newfd;
    int flagjoin=-1; //0 if join, 1 if djoin, -1 if nothing

    struct name *name_head=NULL;

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
                sscanf(user_input,"%s %s",com, com2);

                if(strcmp(com,"join")==0){
                    sscanf(user_input,"%s %s %s",com,net,id);
                    if(strlen(net)!=3 || strlen(id)!=2){
                        printf("Incorrect input!\n");
                        exit(0);
                    }
                    sprintf(app.self,"%s %s %s",id,ip,TCP);
                    flagjoin=0;
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
                    if(flagjoin==0){
                        sscanf(app.self, "%s", id);
                        leave(net,id,regIP,regUDP);
                        exitapp(fd,&app);
                    }
                    if(flagjoin==1){exitapp(fd,&app);}
                    if(flagjoin==-1){printf("Atenção, é necessário dar join ou djoin antes do leave!\n");}
                    flagjoin=-1;
                }

                if(strcmp(com,"exit")==0){
                    if(flagjoin!=-1){
                        printf("Atenção,é necessário dar leave antes de sair!\n");
                    }else{
                        freelist(&name_head);
                        exit(0);
                    }
                }

                if(strcmp(com,"djoin")==0){
                    flagjoin=1;
                    sscanf(user_input,"%s %s %s %s %s %s",com,net,id,bootid,bootIP,bootTCP);
                    sprintf(app.self,"%s %s %s",id,ip,TCP);
                    sprintf(id_to_connect,"%s %s %s",bootid,bootIP,bootTCP);
                    if(strcmp(id,bootid)==0){
                        strcpy(app.ext,app.self);
                        strcpy(app.bck,app.self);                      
                    }
                    else{
                        fd[atoi(bootid)]=djoin(&app,id_to_connect);
                    }
                }
                if(strcmp(com,"create")==0){
                    add_node(&name_head,com2);
                }
                if(strcmp(com,"delete")==0){
                    remove_node(&name_head,com2);
                }
                if(((strcmp(com,"show")==0) && (strcmp(com2,"names")==0)) || (strcmp(com,"sn")==0)){
                    showlist(name_head);
                }
                if(((strcmp(com,"show")==0) && (strcmp(com2,"routing")==0)) || (strcmp(com,"sr")==0)){
                    showtab(tab_exp);
                }
                if(((strcmp(com,"show")==0) && (strcmp(com2,"topology")==0)) || (strcmp(com,"st")==0)){
                    printf("Vizinho Externo: %s\n",app.ext[0]!='\0' ? app.ext : "Não tem nó externo\n");
                    printf("Vizinho de Backup: %s\n",app.bck[0]!='\0' ? app.bck : "Não tem nó de recuperação\n");
                    if(app.num_ints!=0){
                        printf("Vizinhos Internos:\n");
                        for(i=0; i<app.num_ints; i++){
                            printf("%s\n", app.intr[i]);
                        }
                    }else{printf("Não tem nós internos\n");}
                }
                if(strcmp(com,"get")==0){
                    sscanf(user_input,"%s %s %s",com,dest,name);
                    sscanf(app.self,"%s",idaux);
                    sprintf(send, "QUERY %s %s %s\n", dest, idaux , name);
                    forwaring(fd,tab_exp,send,-1);
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
                sscanf(id_to_connect,"%s %s %s\n",bootid,bootIP,bootTCP);
                fd[atoi(bootid)]=newfd;
                FD_CLR(fd[atoi(bootid)],&inputs);
                FD_CLR(fdTCP,&inputs);
            }

            //Receber Novas Mensagens
            for (i = 0; i < 100; i++) {
            if(fd[i]!= -1 ){
            if(FD_ISSET(fd[i],&inputs)){
                printf("Mensagem TCP recebida, do nó com id:%d\n",i);
                responseTCP(fd[i],recv);
                printf("Mensagem:%s\n",recv);
                if(strcmp(recv,"LEAVE")==0){
                    close(fd[i]);
                    fd[i]=-1;
                    //Tirar X da tabela de expedição como vizinho e como destino
                    for (j = 0; j < 100; j++){if(tab_exp[j]==i){tab_exp[j]=-1;}}
                    tab_exp[i]=-1;
                    for (j = 0; j < 100; j++){
                        if(fd[j]!=-1){
                            sprintf(send,"WITHDRAW %d",i);
                            messageTCP(fd[j],send);
                        }
                    }
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
                            FD_CLR(fd[atoi(idaux)],&inputs);
                        }else{if(app.num_ints!=0){ //é ancora, escolhe novo ancora
                            strcpy(app.ext,app.intr[app.num_ints-1]);
                            sscanf(app.ext,"%s",idaux); //Buscar id do novo externo
                            for (j = 0; j < 100; j++){ //Percorrer array sockets
                            if(fd[j]!=-1){ //os que nao forem -1
                                sprintf(send,"EXTERN %s",app.ext);
                                messageTCP(fd[j],send);
                            }}
                            app.num_ints--;
                        }else{strcpy(app.ext,app.self);}//Ficou sozinho
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
                    sscanf(recv,"EXTERN %[^\n]",app.bck);
                    printf("Novo nó de backup: %s\n",app.bck);
                }
                if(strcmp(com,"WITHDRAW")==0){
                    sscanf(recv,"WITHDRAW %d",&idauxint);
                    tab_exp[idauxint]=-1;
                     for (j = 0; j < 100; j++){
                        //Enviar a todos os vizinhos (menos a quem nos envou) mensagem de WITHDDRAW
                        if(j!=i && fd[j]!=-1){
                            sprintf(send,"WITHDRAW %d",idauxint);
                            messageTCP(fd[j],send);
                        }
                    }
                }
                if(strcmp(com,"QUERY")==0 || strcmp(com,"CONTENT")==0 || strcmp(com,"NOCONTENT")==0){
                    sscanf(recv, "%*s %s %s %s", dest,origin,name);
                    tab_exp[atoi(origin)]=i; //Atualizar tabela de expedição
                    sscanf(app.self,"%s",idaux);
                    if(strcmp(dest,idaux)==0){ //É o destino
                    printf("Mensagem chegou ao destino!\n");
                        if(strcmp(com,"QUERY")==0){
                            //ver se tem, criar mensagem, forward
                            if(search_node(&name_head,name)==1){
                                //CONTENT
                                sprintf(send, "CONTENT %s %s %s\n", origin, dest, name);
                            }else{
                                //NOCONTENT
                                sprintf(send, "NOCONTENT %s %s %s\n", origin, dest, name);
                            }
                            forwaring(fd,tab_exp,send,i);
                        }
                        if(strcmp(com,"CONTENT")==0){
                            printf("O conteudo pedido existe!\n");
                        }
                        if(strcmp(com,"NOCONTENT")==0){
                            printf("O conteudo pedido não existe!\n");
                        }
                    }else{ //Não é o destino
                        forwaring(fd,tab_exp,recv,i);
                    }
                }
            FD_CLR(fd[i],&inputs);      
            }}}

        }
    }

    close(fdTCP);
    return 0;
}

