#ifndef __JOIN__
#define __JOIN__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: joinpt1
 * Description: Vai ao servidor de nós, caso já exista nó com mesmo 
 *              id na rede escolhe outro aleatório e escolhe id para se ligar
 *
 * Parameter 1: rede a que nos queremos conectar
 * Parameter 2: id da nossa aplicação do nó
 * Parameter 3: endereço IP do servidor de nós fornecido pelo corpo docente
 * Parameter 4: porto UDP do servidor de nós fornecido pelo corpo docente
 * Parameter 5: string com informação do id, IP e porto do nó a que nos queremos conectar
 * Return: 0 caso dê timeout, e 1 caso a mensagem tenha sido corretamente recebida
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int joinpt1(char* net,char* id,char* regIP,char* regUDP, char* id_to_connect);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: joinpt2
 * Description: Registro no servidor de nós
 *
 * Parameter 1: rede a que nos queremos conectar
 * Parameter 2: id da nossa aplicação do nó
 * Parameter 3: endereço IP do servidor de nós fornecido pelo corpo docente
 * Parameter 4: porto UDP do servidor de nós fornecido pelo corpo docente
 * Parameter 5: IP da nossa aplicação de nó
 * Parameter 6: porta da nossa aplicação de nó
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int joinpt2(char* net,char* id,char* regIP,char* regUDP, char* ip,char* TCP);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: verify_id
 * Description: Verifica se o id já existe na rede
 *
 * Parameter 1: id da nossa aplicação do nó
 * Parameter 2: lista recebida do servidor de nós com contactos dos nós na rede
 * Return: 1 se o id já existir na rede, e 0 caso contrário
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int verify_id(char* id,char* list); 

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: choose_id
 * Description: Escolhe um nó da lista de nós
 *
 * Parameter 1: id da nossa aplicação do nó
 * Parameter 2: lista recebida do servidor de nós com contactos dos nós na rede
 * Parameter 3: string com informação do id, IP e porto do nó a que nos queremos conectar
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int choose_id(char* id,char* list, char* id_to_connect);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: leave
 * Description: Tira o registo do servidor de nós
 *
 * Parameter 1: rede de que nos queremos desconectar
 * Parameter 2: id da nossa aplicação do nó
 * Parameter 3: endereço IP do servidor de nós fornecido pelo corpo docente
 * Parameter 4: porto UDP do servidor de nós fornecido pelo corpo docente
 * Return: 1 se o nó tiver saído com sucesso, e 0 caso contrário
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int leave(char* net,char* id,char* regIP,char* regUDP);

#endif