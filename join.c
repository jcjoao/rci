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

int join(char* com, char* net,char* id,char* regIP,char* regUDP,char* ip, char* regTCP, char* id_to_connect){
    //REG net id ip_da_maquina portaTCP
    char recv[128];
    char send[128];
    char list[128];

    //Receber lista de Nos
    clientUDP(regIP,regUDP, "NODES 055", list);
    //Verificar se ja ha um id
    int aux;
    if(strcmp(com,"join")==0){
        aux=1;
        while(aux==1){
            aux=verify_id(id,list);
            if(aux==1){
            snprintf(id, sizeof(id), "%02d", rand() % 100);
            }
        }
        choose_id(id,list,id_to_connect);
        printf("%s\n",id_to_connect);
        sprintf(send,"REG %s %s %s %s\n",net,id,ip,regTCP);
        //printf("%s\n",send);
    }
    else{
        if(strcmp(com,"leave")==0){
            aux=verify_id(id,list);
            if(aux==0){
                printf("Error, no Node found");
                exit(0);
            }
            sprintf(send,"UNREG %s %s\n",net,id);
            //printf("%s\n",send);
        }
        else{
            exit(0);
        }
    }
    clientUDP(regIP,regUDP,send, recv);

    return 0;
}

int verify_id(char* id,char* list){
    int i =0;
    int aux=0;
    int n = strlen(list);
    while(i!=n && aux==0){
        if(i==0||list[i-1]=='\n'){
            if(list[i]==id[0]&&list[i+1]==id[1]){
                aux=1;
            }
        }
        i++;
    }
return aux;
}

int choose_id(char* id,char* list, char* id_to_connect){
    int num_lines=0;
    char *line;
    char lines[100][32];
    int choosen_id;
    strtok(list, "\n");

    line = strtok(NULL, "\n");

    while (line != NULL && num_lines < 100) {
        strncpy(lines[num_lines], line, 32);
        num_lines++;
        line = strtok(NULL, "\n");
    }

    if (num_lines > 0) {
        choosen_id = rand() % num_lines;
        //printf("Random line: %s\n", lines[choosen_id]);
        strcpy(id_to_connect,lines[choosen_id]);
    }else{
        strcpy(id_to_connect,"empty");
    }

    return 0;
}
