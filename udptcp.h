#ifndef __UDPTCP__
#define __UDPTCP__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: serverTCP
 * Description: Inicia server TCP
 * 
 * Parameter 1: porta TCP do nosso nó como servidor
 * Return: socket da ligação TCP estabelecida
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int serverTCP(char* regTCP,struct sockaddr addr,socklen_t addrlen);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: clientUDP
 * Description: Inicia conexão UDP, envia  a mensagem que está na variável "send"
 *              e recebe mensagem para a variável "recv"
 *
 * Parameter 1: endereço IP do servidor de nós fornecido pelo corpo docente
 * Parameter 2: porto UDP do servidor de nós fornecido pelo corpo docente
 * Parameter 3: string com informação da mensagem que é enviada
 * Parameter 4: string com informação da mensagem que é recebida
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int clientUDP(char* regIP,char* regUDP, char* send, char* recv);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: clientTCP
 * Description: Conecta com um servidor TCP, retorna socket
 *
 * Parameter 1: endereço IP do nó a funcionar como servidor a que nos queremos ligar
 * Parameter 2: porto UDP do nó a funcionar como servidor a que nos queremos ligar
 * Return: socket da ligação TCP estabelecida
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int clientTCP(char* bootIP, char* bootTCP);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: messageTCP
 * Description: Envia mensagem TCP
 *
 * Parameter 1: socket da ligação associada ao nó a que se quer enviar mensagem
 * Parameter 2: string com informação da mensagem que queremos enviar
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int messageTCP(int fd, char* send);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: responseTCP
 * Description: Recebe mensagem TCP
 *
 * Parameter 1: socket do nó da mensagem recebida
 * Parameter 2: string com informação da mensagem recebida
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int responseTCP(int fd,char* recv);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: connectTCP
 * Description: Conecção server TCP com o cliente, retorna socket
 *
 * Parameter 3: socket servidor da nossa aplicação nó
 * Return: socket da nova ligação TCP estabelicida
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int connectTCP(struct sockaddr addr,socklen_t addrlen,int fdTCP);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Function name: newresponseTCP
 * Description: Função para receber mensagem TCP upgraded
 *              fica à espera até receber mensagem completa (caracter \n)
 *
 * Parameter 1: socket do nó da mensagem recebida
 * Parameter 2: string com informação da mensagem recebida
 * Return: -1 se a mensagem não for correta, 0 em caso contrário
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int newresponseTCP(int fd,char* recv);

#endif