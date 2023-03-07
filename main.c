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
#define regUDP "59000"
#define regIP "193.136.138.142"

int join_node();

typedef struct node
{
    int net;
    int id;
    int port;
    struct node *ext;
    struct node *bck;
    struct node *intr;
}node;


int main(int argc, char *argv[ ])
{
    char user_input[20];
    int net, id;
    char com[6];

    fgets(user_input, 12, stdin);
    sscanf(user_input,"%s %d %d",com,&net,&id);


    printf("\n%s, %d,%d,%d\n",com,net,id);
    

    return 0;
}

int join_node(){
    
    return 0;
}