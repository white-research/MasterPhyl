#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

void add_taxon_randomly(Tree *t, int id, int max_taxa){
    printf("  Adding tip %i randomly\n", id);
    Node *new_node = malloc(sizeof(Node));
    new_node->id = id;
    new_node->desc1 = NULL;
    new_node->desc2 = NULL;
    if (t->ntaxa == 0){
        Node *root_node = malloc(sizeof(Node));
        root_node->anc = NULL;
        root_node->id = max_taxa;
        root_node->desc1 = new_node;
        root_node->desc2 = NULL;
        new_node->anc = root_node;
        t->ntaxa = 1;
        t->maxid = max_taxa;
        t->root_node = root_node;
    }
    else {
        if (t->ntaxa == 1){
            printf("\nAbout to add root's descendent\n");
            printf("\nRoot:%i\n", t->root_node->id);
            t->root_node->desc2 = new_node;
            printf("\nAbout to add root as node's ancestor\n");
            new_node->anc = t->root_node;
            printf("\nAbout to increment total taxa\n");
            t->ntaxa++;
            printf("\nDone.\n");
        }
        else{
            t->max_id++;
            Node *old_desc1 = t->root_node->desc1;
            Node *new_internal_node = malloc(sizeof(Node));
            new_internal_node->id = t->max_id;
            new_internal_node->anc = t->root_node;
            t->root_node->desc1 = new_internal_node;
            old_desc1->anc = new_internal_node;
            new_internal_node->desc1 = new_node;
            new_internal_node->desc2 = old_desc1;
            t->ntaxa++;
            t->max_id++;
            printf("total taxa in tree: %i\n", t->ntaxa);
        }
    }
}


Tree *make_random_tree(int ntaxa){
    int included[ntaxa];
    for (int idx=0; idx<ntaxa; idx++){
        included[idx] = 1;
    }
    
    Tree *newt = malloc(sizeof(Tree));
    newt->ntaxa=0;
    for(int taxa_left=ntaxa; taxa_left > 0; taxa_left--){
        int r = rand() % taxa_left;
        int counter = -1, idx = 0;
        while (counter < r) {
            counter += included[idx];
            idx++;
            printf("%i:", idx);
            printf("%i ", included[idx]);
        }
        included[idx-1] = 0;
        printf("\nIncluded\n");
        for (int idx=0; idx<ntaxa; idx++){
            printf("%i ", included[idx]);
        }
        printf("\nrandom int:%i - taxa left: %i\n", r, taxa_left);
        add_taxon_randomly(newt, idx-1, ntaxa);
    }
    return newt;
}



int get_children(int node, int phyl[], int num_vertices, int children[2])
{
    int start, i, j = -1;
    start = node*num_vertices;

    for ( i = 0; i < num_vertices; ++i)
    {
        if (phyl[start+i] == -1)
        {
            if (j>2)
            {
                printf("1: This is not a binary tree\n");
                abort();
            }
            j++;
            children[j] = i;
        }
    }
    if (j == 0 || j > 1 )
    {
        printf("2: This is not a binary tree\n");
        abort();
    }
    return j;
}


int get_ancestor(int node, int phyl[], int num_vertices)
{
    int start, i, is_root = -1;
    start = node*num_vertices;
    for (i=0; i<num_vertices; ++i)
    {
        if (phyl[start+i] == 1)
        {
            if (is_root != -1)
            {
                printf("1: This is not a binary tree\n");
                abort();
            }
            is_root = i;
        }
    }
    return is_root;
}




