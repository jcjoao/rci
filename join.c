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

#include "join.h"
#include "udptcp.h"

//Função que realiza a conexão TCP no join
int joinpt1(char* net,char* id,char* regIP,char* regUDP, char* id_to_connect){
    char list[150];
    char send[150];
    int ifrandom=0;
    sprintf(send,"NODES %s",net);
    //Receber lista de Nos
    clientUDP(regIP,regUDP, send, list);
    //Se der Timeout nao faz nada e retorna 0
    if(strcmp(list,"TIMEOUT")!=0){
        //Verificar se ja ha um id
        int aux=1;
            while(aux==1){
                //Se id ja existir, gera id novo random até gerar um que ainda não está na rede
                aux=verify_id(id,list);
                if(aux==1){
                    ifrandom=1;
                    snprintf(id, 3, "%02d", rand() % 100);
                }
            }
        //Se teve que gerar um id, por ja haver outro na rede, informa o utilizador
        if(ifrandom==1){
            printf("\x1b[32m[Info]\x1b[0m Já existe um nó com esse id na rede!\n");
            printf("\x1b[32m[Info]\x1b[0m Seu novo id: %s\n", id);
        }
        //Escolhe id para se conectar, fica na variavel id_to_connect
        choose_id(id,list,id_to_connect);
        return 1;
    }else{
    return 0;
    }
}

//Função que realiza o registo na rede de nós no join
int joinpt2(char* net,char* id,char* regIP,char* regUDP, char* ip,char* TCP){
    char recv[150];
    char send[150];

    //Cria mensagem REG
    sprintf(send,"REG %s %s %s %s\n",net,id,ip,TCP);
    //Envia a mensagem
    clientUDP(regIP,regUDP,send, recv);
    //Se correu bem imprime resposta positiva, caso contrario resposta negativa
    if(strcmp(recv,"OKREG")==0){
        printf("\x1b[32m[Info]\x1b[0m Nó entrou do Servidor de Nós com Sucesso!\n");
    }else{
        printf("\x1b[31m[Error]\x1b[0m Erro ao entrar do Servidor de Nós!\n");
    }
    return 0;
}

//Função para realizar a saída de um nó da rede
int leave(char* net,char* id,char* regIP,char* regUDP){
    char recv[150];
    char send[150];
    char list[150];

    //Receber lista de Nos
    sprintf(send,"NODES %s",net);
    clientUDP(regIP,regUDP, send, list);
    //Se nao tiver sido dado timeout
    if(strcmp(list,"TIMEOUT")!=0){
        //Verifica que o id está na lista
        int aux=verify_id(id,list);
        if(aux==0){
            printf("\x1b[31m[Error]\x1b[0m Nenhum nó com esse id encontrado!");
            return 0;
        }
        //Cria mensagem UNREG
        sprintf(send,"UNREG %s %s\n",net,id);
        //Envia a mensagem
        clientUDP(regIP,regUDP,send, recv);
        if(strcmp(recv,"OKUNREG")==0){
            printf("\x1b[32m[Info]\x1b[0m Nó saiu do Servidor de Nós com Sucesso!\n");
        }else{
            printf("\x1b[31m[Error]\x1b[0m Erro ao sair do Servidor de Nós!\n");
        }
        return 1;
    }
    return 0;
}

int verify_id(char* id,char* list){
    int i =0;
    int aux=0;
    int n = strlen(list);
    //Percorre a lista, enquanto aux = 0
    while(i!=n && aux==0){
        //Se estivermos na primeira linha, ou no inicio de uma linha (ultimo caracter \n)
        if(i==0||list[i-1]=='\n'){
            //Compara com o nosso id, se for igual, aux=1
            if(list[i]==id[0]&&list[i+1]==id[1]){
                aux=1;
            }
        }
        i++;
    }
    //aux=1 ja existe id, aux =0 ainda nao existe
    return aux;
}

int choose_id(char* id,char* list, char* id_to_connect){
    int num_lines=0;
    char *line;
    char lines[100][32];
    int choosen_id;
    strtok(list, "\n");

    line = strtok(NULL, "\n");

    //Conta numero de linhas que há
    while (line != NULL && num_lines < 100) {
        strncpy(lines[num_lines], line, 32);
        num_lines++;
        line = strtok(NULL, "\n");
    }

    //Escolhe uma dessas para se ligar, de forma aleatoria
    if (num_lines > 0) {
        choosen_id = rand() % num_lines;
        //printf("Random line: %s\n", lines[choosen_id]);
        strcpy(id_to_connect,lines[choosen_id]);
        printf("\x1b[32m[Info]\x1b[0m Conexão irá ser feita com o Nó: %s\n",id_to_connect); 
    }else{
        //Se a lista estiver vazia, id_to_connect vai ser ele proprio
        sprintf(id_to_connect,"%s 0 0\n",id);
        printf("\x1b[32m[Info]\x1b[0m Rede vazia, o seu Nó é o unico na rede!\n");
    }

    return 0;
}
