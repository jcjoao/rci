#ifndef __JOIN__
#define __JOIN__

int join(char* com, char* net,char* id,char* regIP,char* regUDP,char* ip, char* regTCP, char* id_to_connect);
int verify_id(char* id,char* list); //returns 1 if id is in list, 0 if it inst
int choose_id(char* id,char* list, char* id_to_connect);

#endif