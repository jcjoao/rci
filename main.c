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
    //Select Variables
    fd_set inputs, testfds;
    //struct timeval timeout;
    //int i,out_fds,n,errcode;
    int out_fds;
    FD_ZERO(&inputs); // Clear inputs
    FD_SET(0,&inputs); // Set standard input channel on
    //My variables
    char user_input[12];
    int net, id;
    char com[5];

    while(1)
    {
        testfds=inputs;
        
        out_fds=select(FD_SETSIZE,&testfds,(fd_set *)NULL,(fd_set *)NULL,(struct timeval *) NULL);
        
        if(out_fds==-1){
            perror("select");
                exit(1);
        }
        else{
            if(FD_ISSET(0,&testfds))
                {
                    fgets(user_input, 20, stdin);
                    sscanf(user_input,"%s %d %d\n",com,&net,&id);
                    printf("%s,%d,%d\n",com,net,id);
                }
        }
        
    }
    
    return 0;
}

int join_node(){
    //REG net id ip_da_maquina portaTCP
    return 0;
}
