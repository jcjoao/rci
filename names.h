#ifndef __NAMES__
#define __NAMES__

#include "struct.h"
#include "udptcp.h"

typedef struct name
{
    char name[100];
    struct name *next;
}name;

name *create_node(char *name_str);
void add_node(name **list_head, char *name_str);
int remove_node(name **list_head, char *name_str);
void freelist(name **list_head);
void showlist(name *list_head);
void showtab(int *tab_exp);
void forwaring(int *fd, int *tab_exp, char* msg, int before);
int search_node(name **list_head, char *name_str);

#endif