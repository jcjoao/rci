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

#include "menu.h"

//Função para dar print do menu inicial
void menu(){

    printf("\n");
    printf("|--------------------------------------------------------------------------------------|\n");
    printf("|---------------------\033[32mTransferência de Conteúdos em Redes Arbóreas\033[0m---------------------|\n");
    printf("|-------------------------------------\033[32mMENU INICIAL\033[0m-------------------------------------|\n");
    printf("|--------------------------------------------------------------------------------------|\n\n");
    printf("\033[34mFuncionalidade do programa:\n");
    printf("\033[0mAplicação para criação de um nó, pertencente a uma rede de nós no formato de uma árvore,\n");
    printf("interligados com sessões TCP, e com ligação por UDP a uma base de dados, para cosulta.\n\n");
    printf("\033[34mOpções de comandos:\n");
    printf("· \033[0mjoin net id\n");
    printf("\033[34m· \033[0mdjoin net id bootid bootIP bootTCP\n");
    printf("\033[34m· \033[0mcreate name\n");
    printf("\033[34m· \033[0mdelete name\n");
    printf("\033[34m· \033[0mget dest name\n"); 
    printf("\033[34m· \033[0mshow topology (st)\n");
    printf("\033[34m· \033[0mshow names (sn)\n");
    printf("\033[34m· \033[0mshow routing (sr)\n");
    printf("\033[34m· \033[0mclear routing (cr)\n");
    printf("\033[34m· \033[0mleave\n");
    printf("\033[34m· \033[0mexit\n");
    printf("\033[34m· \033[0mhelp\n\n");
    printf("Para mais informações, consultar a opção \033[34mhelp\033[0m.\n\n");

    return;
}

//Função para dar print das opções de ajuda ao utilizador
void helpp(){

    printf("\n\n");
    printf("|--------------------------------------------------------------------------------------|\n");
    printf("|-----------------------------------------\033[31mHELP\033[0m-----------------------------------------|\n");
    printf("|--------------------------------------------------------------------------------------|\n\n");
    printf("\033[34mOpções de comandos:\n\n");
    
    //Opção join net id   
    printf("· \033[0mjoin \033[4mnet\033[0m \033[4mid\033[0m:\n");
    printf("Entrada de um nó na rede \033[34mnet\033[0m com identificador \033[34mid\033[0m, sendo escolhido outro identificador\n");
    printf("se o indicado já estiver a ser usado por outro nó. Os valores de \033[34mnet\033[0m devem ter três\n");
    printf("dígitos, podendo variar entre 000 e 999, e os valores de \033[34mid\033[0m devem ter dois dígitos,\n");
    printf("podendo variar entre 00 e 99.\n\n");
    
    //Opção djoin net id bootid bootIP bootTCP 
    printf("\033[34m· \033[0mdjoin \033[4mnet\033[0m \033[4mid\033[0m \033[4mbootid\033[0m \033[4mbootIP\033[0m \033[4m bootTCP\033[0m:\n");
    printf("Entrada de um nó na rede \033[34mnet\033[0m com identificador \033[34mid\033[0m, único na rede, diferente dos já\n");
    printf("existentes, passando à aplicação o identificador e o contacto de um nó da rede, através\n");    
    printf("dos parâmetros \033[34mbootid\033[0m, \033[34mbootIP\033[0m e \033[34mbootTCP\033[0m, executando-se a ligação sem interrogar o\n");
    printf("servidor de nós. Se \033[34mbootid\033[0m for igual a \033[34mid\033[0m, é criada a rede só com um nó.\n\n"); 
   
    //Opção create name
    printf("\033[34m· \033[0mcreate \033[4mname\033[0m:\n");
    printf("Cria um conteúdo de nome \033[34mname\033[0m no nó, devendo \033[34mname\033[0m ser uma sequência alfanumérica com um\n");
    printf("máximo de 100 carateres.\n\n");
    
    //Opção delete name   
    printf("\033[34m· \033[0mdelete \033[4mname\033[0m:\n");
    printf("Apagado o conteúdo de nome \033[34mname\033[0m da lista de nomes do nó.\n\n");

    //Opção get dest name
    printf("\033[34m· \033[0mget \033[4mdest\033[0m \033[4mname\033[0m:\n");
    printf("Pesquisa do conteúdo com o nome \033[34mname\033[0m localizado no nó \033[34mdest\033[0m.\n\n");

    //Opção show topology (st)
    printf("\033[34m· \033[0mshow topology (st):\n");
    printf("Mostra os identificadores e os contactos dos vizinhos internos, do vizinho externo e\n");
    printf("do vizinho de recuperação, do nó em causa.\n\n");
    
    //Opção show names (sn)   
    printf("\033[34m· \033[0mshow names (sn):\n");
    printf("Mostra os nomes dos conteúdos presentes no nó.\n\n");
   
    //Opção show routing (sr)  
    printf("\033[34m· \033[0mshow routing (sr):\n");
    printf("Mostra a tabela de expedição do nó.\n\n");
   
    //Opção leave 
    printf("\033[34m· \033[0mleave:\n");
    printf("Saída do nó da rede.\n\n");
   
    //Opção exit
    printf("\033[34m· \033[0mexit:\n");
    printf("Fecho da aplicação.\n\n");
   
    //Opção help 
    printf("\033[34m· \033[0mhelp:\n");
    printf("Mostra mais detalhadamente todas as opções de comandos de entrada do programa.\n\n");

    return;
}

int all_digits(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
        i++;
    }
    return 1;
}