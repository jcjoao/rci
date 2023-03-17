#ifndef __JOIN__
#define __JOIN__

int joinpt1(char* net,char* id,char* regIP,char* regUDP, char* id_to_connect);
int joinpt2(char* net,char* id,char* regIP,char* regUDP, char* ip,char* TCP);
int verify_id(char* id,char* list); //returns 1 if id is in list, 0 if it inst
int choose_id(char* id,char* list, char* id_to_connect);
int leave(char* net,char* id,char* regIP,char* regUDP);

#endif