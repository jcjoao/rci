#ifndef __UDPTCP__
#define __UDPTCP__

int serverTCP(char* regTCP,struct sockaddr addr,socklen_t addrlen);
int clientUDP(char* regIP,char* regUDP, char* send, char* recv);
int clientTCP(char* bootIP, char* bootTCP);
int messageTCP(int fd, char* send);
int responseTCP(int fd,char* recv);
int connectTCP(struct sockaddr addr,socklen_t addrlen,int fdTCP);
int newresponseTCP(int fd,char* recv);

#endif