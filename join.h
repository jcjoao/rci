#ifndef __JOIN__
#define __JOIN__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: joinpt1
 * Description: Vai ao servidor de nós,caso já exista nó com mesmo 
 *              id na rede escolhe outro aleatorio e escolhe id para se ligar, 
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int joinpt1(char* net,char* id,char* regIP,char* regUDP, char* id_to_connect);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: joinpt2
 * Description: Registro no servidor de Nós
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int joinpt2(char* net,char* id,char* regIP,char* regUDP, char* ip,char* TCP);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: verify_id
 * Description: Verifica se o id já existe na rede
 *              Retorna 1 caso já exista, 0 caso ainda não
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int verify_id(char* id,char* list); 

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: choose_id
 * Description: Escolhe um nó da lista de nós
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int choose_id(char* id,char* list, char* id_to_connect);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: leave
 * Description: Tira o registo do servidor de Nós
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int leave(char* net,char* id,char* regIP,char* regUDP);

#endif