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
#include "menu.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    node app;
    app.num_ints=0;

    //menu();

    if (argc != 3) {
        printf("\x1b[31m[Error]\x1b[0m Argumentos Inválidos\n");
        exit(1);
    }
    char *ip = argv[1]; //our ip
    char *TCP = argv[2]; //our port that is going to be used on the TCP server

    if (atoi(TCP) < 1024 || atoi(TCP) > 65535) {
        printf("\x1b[31m[Error]\x1b[0m Número de porto Inválido!\n");
        exit(0);
    }

    //My variables
    char user_input[150];
    char net[4], id[3];
    char com[12];
    char com2[100];
    char name[100];
    char regIP[]="193.136.138.142"; //IP of "Tejo"
    char regUDP[]="59000"; //port of "tejo"
    char id_to_connect[33]; //contact information of the node we want to connect to
    char bootid[3];  //id of the node we want to connect to
    char bootIP[16]; //IP of the node we want to connect to
    char bootTCP[6]; //TCP port of the node we want to connect to
    char recv[150]; //string to send messages
    char send[150]; //string to recieve messages
    int i=0;
    int j=0;
    int k=0;
    char idaux[3];
    int idauxint;
    char dest[3];
    char origin[3];
    int checkinput;

    //char *msg, *saveptr;

    int tab_exp[100]; //matriz para a tabela de expedição
    int fd[100]; //array with sockets from nodes connected to our TCP server
    for(i=0;i<100;i++){fd[i]=-1;tab_exp[i]=-1;} //Inicializar todos os elementos a -1

    struct sockaddr addr; 
    socklen_t addrlen = 0;
    int fdTCP=serverTCP(TCP,addr,addrlen); //initiate TCP server
    int newfd;
    int flagjoin=-1; //0 if join, 1 if djoin, -1 if nothing

    struct name *name_head=NULL;

    //Ignorar Sigpipe
    struct sigaction act;
    memset(&act,0,sizeof act);
    act.sa_handler=SIG_IGN;
    if(sigaction(SIGPIPE,&act,NULL)==-1)/*error*/exit(1);


    //Select Variables
    fd_set inputs;
    int out_fds;
    //struct timeval timeout;
    while(1)
    {
        printf("\n-------------\n");
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
                fgets(user_input, 150, stdin);
                sscanf(user_input,"%s %s",com, com2);
                checkinput=0;

                if(strcmp(com,"join")==0){
                    checkinput=1;
                    if(flagjoin==-1){
                    sscanf(user_input,"%s %s %s",com,net,id);
                    if(strlen(net)!=3 || strlen(id)!=2){
                        printf("\x1b[33m[Warning]\x1b[0m Argumentos Incorretos!\n");
                    }else{
                        flagjoin=0;
                        joinpt1(net,id,regIP,regUDP,id_to_connect);
                        sprintf(app.self,"%s %s %s",id,ip,TCP);
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
                    }else{
                        printf("\x1b[33m[Warning]\x1b[0m Fazer leave antes de dar join outra vez!\n");
                    }
                }

                if(strcmp(com,"leave")==0){
                    checkinput=1;
                    if(flagjoin==0){
                        sscanf(app.self, "%s", id);
                        leave(net,id,regIP,regUDP);
                        exitapp(fd,&app);
                    }
                    if(flagjoin==1){exitapp(fd,&app);}
                    if(flagjoin==-1){printf("\x1b[33m[Warning]\x1b[0m É necessário dar join ou djoin antes do leave!\n");}
                    flagjoin=-1;
                }

                if(strcmp(com,"exit")==0){
                    checkinput=1;
                    if(flagjoin!=-1){
                        printf("\x1b[33m[Warning]\x1b[0m É necessário dar leave antes de sair!\n");
                    }else{
                        freelist(&name_head);
                        exit(0);
                    }
                }

                if(strcmp(com,"djoin")==0){
                    checkinput=1;
                    if(flagjoin==-1){
                    sscanf(user_input,"%s %s %s %s %s %s",com,net,id,bootid,bootIP,bootTCP);
                    if(strlen(net)!=3 || strlen(id)!=2 || strlen(bootid)!=2 || atoi(bootTCP) < 1024 || atoi(bootTCP) > 65535){
                        printf("\x1b[33m[Warning]\x1b[0m Argumentos Incorretos!\n");
                    }else{
                        flagjoin=1;
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
                    }else{
                        printf("\x1b[33m[Warning]\x1b[0m Fazer leave antes de dar djoin outra vez!\n");
                    }
                }
                if(strcmp(com,"create")==0){
                    checkinput=1;
                    add_node(&name_head,com2);
                }
                if(strcmp(com,"delete")==0){
                    checkinput=1;
                    remove_node(&name_head,com2);
                }
                if(((strcmp(com,"show")==0) && (strcmp(com2,"names")==0)) || (strcmp(com,"sn")==0)){
                    checkinput=1;
                    showlist(name_head);
                }
                if(((strcmp(com,"show")==0) && (strcmp(com2,"routing")==0)) || (strcmp(com,"sr")==0)){
                    checkinput=1;
                    showtab(tab_exp);
                }
                if(((strcmp(com,"show")==0) && (strcmp(com2,"topology")==0)) || (strcmp(com,"st")==0)){
                    checkinput=1;
                    printf("\x1b[32m[Info]\x1b[0m Vizinho Externo: %s\n",app.ext[0]!='\0' ? app.ext : "Não tem nó externo\n");
                    printf("\x1b[32m[Info]\x1b[0m Vizinho de Backup: %s\n",app.bck[0]!='\0' ? app.bck : "Não tem nó de recuperação\n");
                    if(app.num_ints!=0){
                        printf("\x1b[32m[Info]\x1b[0m Vizinhos Internos:\n");
                        for(i=0; i<app.num_ints; i++){
                            printf("%s\n", app.intr[i]);
                        }
                    }else{printf("\x1b[32m[Info]\x1b[0m Não tem nós internos\n");}
                }
                if(strcmp(com,"get")==0){
                    checkinput=1;
                    sscanf(user_input,"%s %s %s",com,dest,name);
                    if(strlen(dest)!=2){
                        printf("\x1b[33m[Warning]\x1b[0m Argumentos Incorretos!\n");
                    }else{
                        sscanf(app.self,"%s",idaux);
                        sprintf(send, "QUERY %s %s %s\n", dest, idaux , name);
                        forwaring(fd,tab_exp,send,-1);
                    }
                }
                if(((strcmp(com,"clear")==0) && (strcmp(com2,"routing")==0)) || (strcmp(com,"cr")==0)){
                    checkinput=1;
                    for(i=0;i<100;i++){tab_exp[i]=-1;}
                    printf("\x1b[32m[Info]\x1b[0m Tabela de Expedição Limpa!\n");
                }
                if(strcmp(com,"help")==0){
                    checkinput=1;
	                helpp(); 
                    printf("help\n");
                }
                if(checkinput==0){
                    printf("\x1b[33m[Warning]\x1b[0m Input invalido, porfavor introduzir um dos comandos!\n");
                }
            }

            //Receber Novos Pedidos de Conexao
            if(FD_ISSET(fdTCP,&inputs)){
                printf("\x1b[32m[Info]\x1b[0m Novo Pedido Conexão\n");
                newfd = connectTCP(addr,addrlen,fdTCP);
                responseTCP(newfd,recv);
                printf("\x1b[32m[Info]\x1b[0m Mensagem Recebida: %s",recv);
                sscanf(recv,"NEW %[^\n]",id_to_connect);
                //Nó estava sozinho
                if(strcmp(app.self,app.ext)==0){
                    printf("\x1b[32m[Info]\x1b[0m Novo Vizinho Externo: %s\n",app.ext);
                    strcpy(app.ext,id_to_connect);
                }else{
                    strcpy(app.intr[app.num_ints],id_to_connect);
                    printf("\x1b[32m[Info]\x1b[0m Novo Vizinho Interno: %s\n",app.intr[app.num_ints]);
                    app.num_ints++;
                }
                sprintf(send,"EXTERN %s\n",app.ext);
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
                checkinput=0;
                printf("\x1b[32m[Info]\x1b[0m Mensagem TCP recebida, do nó com id:%d\n",i);
                if(newresponseTCP(fd[i],recv)!=-1){
                printf("\x1b[32m[Info]\x1b[0m Mensagem:%s\n",recv);
                sscanf(recv,"%s",com);
                if(strcmp(com,"LEAVE")==0){
                    checkinput=1;
                    printf("\x1b[32m[Info]\x1b[0m Nó %d saiu da rede!\n",i);
                    close(fd[i]);
                    fd[i]=-1;
                    //Tirar X da tabela de expedição como vizinho e como destino
                    for (j = 0; j < 100; j++){if(tab_exp[j]==i){tab_exp[j]=-1;}}
                    tab_exp[i]=-1;
                    for (j = 0; j < 100; j++){
                        if(fd[j]!=-1){
                            sprintf(send,"WITHDRAW %d\n",i);
                            messageTCP(fd[j],send);
                        }
                    }
                    //Ver se é externo
                    sscanf(app.ext,"%s",idaux);
                    if(atoi(idaux)==i){ //Mensagem de Vizinho externo
                        //printf("Vizinho externo desconentou-se!\n");
                        if(strcmp(app.bck,app.self)!=0){//nao é ancora
                            strcpy(app.ext,app.bck); //Backup passa a ser o externo
                            printf("\x1b[32m[Info]\x1b[0m Novo Nó externo: %s\n",app.ext);
                            sscanf(app.ext,"%s",idaux); //Buscar id do novo externo
                            fd[atoi(idaux)]=djoin(&app,app.ext);//Junta-se ao novo ext, recebe novo bck
                            //mandar mensagem a todos os internos com novo bck
                            for (j = 0; j < 100; j++){ //Percorrer array sockets
                            if(fd[j]!=-1 && j!=atoi(idaux)){ //os que nao forem -1, nem o externo (internos)
                                sprintf(send,"EXTERN %s\n",app.ext);
                                messageTCP(fd[j],send);
                            }}
                            FD_CLR(fd[atoi(idaux)],&inputs);
                        }else{if(app.num_ints!=0){ //é ancora, escolhe novo ancora
                            strcpy(app.ext,app.intr[app.num_ints-1]);
                            printf("\x1b[32m[Info]\x1b[0m Novo Nó externo: %s\n",app.ext);
                            sscanf(app.ext,"%s",idaux); //Buscar id do novo externo
                            for (j = 0; j < 100; j++){ //Percorrer array sockets
                            if(fd[j]!=-1){ //os que nao forem -1
                                sprintf(send,"EXTERN %s\n",app.ext);
                                messageTCP(fd[j],send);
                            }}
                            app.num_ints--;
                        }else{
                            strcpy(app.ext,app.self);
                            printf("\x1b[32m[Info]\x1b[0m Nó ficou sozinho\n");
                            }//Ficou sozinho
                        }
                    }else{
                        //printf("Vizinho interno desconentou-se!\n");
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
                //msg = strtok_r(recv, "\n", &saveptr);
                //while (msg != NULL) {
                    //sscanf(msg, "%s", com);
                    if(strcmp(com,"EXTERN")==0){
                        checkinput=1;
                        sscanf(recv,"EXTERN %[^\n]",app.bck);
                        printf("\x1b[32m[Info]\x1b[0m Novo nó de backup: %s\n",app.bck);
                    }
                    if(strcmp(com,"WITHDRAW")==0){
                        checkinput=1;
                        sscanf(recv,"WITHDRAW %d",&idauxint);
                        tab_exp[idauxint]=-1;
                        for (j = 0; j < 100; j++){
                            //Enviar a todos os vizinhos (menos a quem nos envou) mensagem de WITHDDRAW
                            if(j!=i && fd[j]!=-1){
                                sprintf(send,"WITHDRAW %d\n",idauxint);
                                messageTCP(fd[j],send);
                            }
                        }
                    }
                    if(strcmp(com,"QUERY")==0 || strcmp(com,"CONTENT")==0 || strcmp(com,"NOCONTENT")==0){
                        checkinput=1;
                        sscanf(recv, "%*s %s %s %s", dest,origin,name);
                        tab_exp[atoi(origin)]=i; //Atualizar tabela de expedição
                        sscanf(app.self,"%s",idaux);
                        if(strcmp(dest,idaux)==0){ //É o destino
                        //printf("Mensagem chegou ao destino!\n");
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
                                printf("\x1b[32m[Info]\x1b[0m O conteudo pedido existe!\n");
                            }
                            if(strcmp(com,"NOCONTENT")==0){
                                printf("\x1b[32m[Info]\x1b[0m O conteudo pedido \x1b[31mNÃO\x1b[0m existe!\n");
                            }
                        }else{ //Não é o destino
                            //Se o que lhe enviou a mensagem é o mesmo que está na tabela de expedição
                            //como vizinho para o destino, descartar mensagem
                            if(tab_exp[atoi(dest)]!=i){
                                forwaring(fd,tab_exp,recv,i);
                            }
                        }
                    }
                    //msg = strtok_r(NULL, "\n", &saveptr);
                //}
                }
                if(checkinput==0){
                    printf("\x1b[33m[Warning]\x1b[0m Mensagem recebida com formato errado\n");
                }
            FD_CLR(fd[i],&inputs); 
            }}}

        }
    }

    close(fdTCP);
    return 0;
}

