#ifndef __UDPTCP__
#define __UDPTCP__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: serverTCP
 * Description: Inicia server TCP
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int serverTCP(char* regTCP,struct sockaddr addr,socklen_t addrlen);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: clientUDP
 * Description: Inicia conxexao udp, envia  a mensagem que está na variavel "send"
 *              e recebe mensagem para a variavel "recv"
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int clientUDP(char* regIP,char* regUDP, char* send, char* recv);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: clientTCP
 * Description: Conecta com um servidor TCP, retorna socket
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int clientTCP(char* bootIP, char* bootTCP);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: messageTCP
 * Description: Envia Mensagem TCP
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int messageTCP(int fd, char* send);
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: responseTCP
 * Description: Recebe mensagem TCP
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int responseTCP(int fd,char* recv);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: connectTCP
 * Description: coneccao server TCP com o cliente, retorna socket
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int connectTCP(struct sockaddr addr,socklen_t addrlen,int fdTCP);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: newresponseTCP
 * Description: Funcao para receber mensagem TCP upgraded
 *              fica à espera até receber mensagem completa (caracter \n)
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int newresponseTCP(int fd,char* recv);

#endif