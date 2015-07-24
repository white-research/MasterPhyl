#include <stdio.h>
#include <stdlib.h>
#include "tree.h"


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




