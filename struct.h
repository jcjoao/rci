#ifndef NODE_H
#define NODE_H

typedef struct node {
    char self[32];
    char ext[32];
    char bck[32];
    int num_ints;
    char intr[][32];
} node;

#endif