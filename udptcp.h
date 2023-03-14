#ifndef __UDPTCP__
#define __UDPTCP__

int serverTCP(char* regTCP,struct sockaddr addr,socklen_t addrlen);
int clientUDP(char* regIP,char* regUDP, char* send, char* recv);
int clientTCP();
#endif