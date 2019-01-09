#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "tree.h"


char *to_newick(Tree *t, Node *n){
    if (n->desc1 == NULL && n->desc1 == NULL){
        char *nodename = (char *)malloc(sizeof(char)*10);
        sprintf(nodename, "%i", n->id);
        return nodename;
    } else{
        char *d1name = to_newick(t, n->desc1);
        char *d2name = to_newick(t, n->desc2);
        
        char *nodename = (char *)malloc(sizeof(char)*(strlen(d1name)+strlen(d2name)+3));
        strcpy(nodename, "(");
        strcat(nodename, d1name);
        strcat(nodename, ",");
        strcat(nodename, d2name);
        strcat(nodename, ")");
        
        free(d1name);
        free(d2name);
        
        return nodename;
    }
    
}













