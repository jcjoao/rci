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

typedef struct node
{
    int net;
    int id;
    int port;
    struct node *ext;
    struct node *bck;
    struct node *intr;
}node;


int main(void)
{
    printf("a");
    

    return 0;
}