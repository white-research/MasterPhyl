#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "tree.h"

int taxcount_uppass(Tree *t, Node *current_node, int count){
    printf("On node: %i, current count=%i\n", current_node->id, count);
    if (current_node->desc1 != NULL){
        count = taxcount_uppass(t, current_node->desc1, count);
    }
    if (current_node->desc2 != NULL){
        count = taxcount_uppass(t, current_node->desc2, count);
    }
    if (current_node->desc1 == NULL && current_node->desc2 == NULL){
        count++;
    }
    return count;
}

int tree_is_correct(Tree *t){
    printf("Taxon count: %i\n", taxcount_uppass(t, t->root_node, 0));
    assert(taxcount_uppass(t, t->root_node, 0) == t->ntaxa);
    return 1;
}


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
        t->max_id = max_taxa;
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


void print_nodes(Tree *t, Node *current_node){
    printf("\nCurrent node:%2i, ", current_node->id);
    if (current_node->desc1 != NULL){
        printf("Descendent 1:%2i, ", current_node->desc1->id);
        printf("Descendent 2:%2i, ", current_node->desc2->id);
        print_nodes(t, current_node->desc1);
    } else{
        printf("\n   ");
        //for(int idx = 1; idx<level; idx++){
        //    printf("   ");
        //}
    }
    if (current_node->desc2 != NULL){
        print_nodes(t, current_node->desc2);
    } else{
        //printf("\n   ");
        //for(int idx = 1; idx<level; idx++){
        //    printf("   ");
        //}
    }
}

void print_tree(Tree *t, Node *current_node, int level){
    
    for(int idx = 1; idx<level; idx++){
        printf("  ");
    }
    printf("-%2i", current_node->id);
    if (current_node->desc1 != NULL){
        print_tree(t, current_node->desc1,level+1);
    } else{
        printf("\n  ");
        //for(int idx = 0; idx<level; idx++){
        //    printf(" ;");
        //}
    }
    if (current_node->desc2 != NULL){
        print_tree(t, current_node->desc2,level+1);
    } //else{
        //printf("\n  ");
        //for(int idx = 0; idx<level; idx++){
        //    printf(" ,");
        //}
    // }
}


int _fill_branch_list_recursive(Tree *t, int *branch_list, Node *current_node, int branch_counter){
    printf("Branch # %i\n", branch_counter);
    if (current_node->desc1 != NULL){
        branch_list[branch_counter*2] = current_node->id;
        branch_list[branch_counter*2+1] = current_node->desc1->id;
        branch_counter++;
        branch_counter = _fill_branch_list_recursive(t, branch_list, current_node->desc1, branch_counter);
    }
    if (current_node->desc2 != NULL){
        branch_list[branch_counter*2] = current_node->id;
        branch_list[branch_counter*2+1] = current_node->desc2->id;
        branch_counter++;
        branch_counter = _fill_branch_list_recursive(t, branch_list, current_node->desc2, branch_counter);
    }
    return branch_counter;
}


int *get_branch_list(Tree *t){
    int *branch_list = malloc(sizeof(int)*2*(2*t->ntaxa - 2));
    
    int final_branch = _fill_branch_list_recursive(t, branch_list, t->root_node, 0);
    printf("Final branch_count = %i\n", final_branch);
    assert(final_branch == 2*t->ntaxa - 2);
    for (int idx = 0; idx < 2*t->ntaxa - 2; idx++){
        printf("%2i -> %2i\n", branch_list[2*idx], branch_list[2*idx+1]);
    }
    return branch_list;
}


Tree *split_tree(Tree *t){
    int *branches = get_branch_list(t);
    int branch_to_split = rand() % (2*t->ntaxa - 2);
    printf("Random: %i = %i -> %i\n", branch_to_split, branches[2*branch_to_split], branches[2*branch_to_split+1]);
    
    Tree *second_tree = malloc(sizeof(Tree));
    
    free(branches);
    return second_tree;
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
            printf("%i:", idx);
            printf("%i ", included[idx]);
            idx++;
        }
        included[idx-1] = 0;
        printf("\nIncluded\n");
        for (int idx=0; idx<ntaxa; idx++){
            printf("%i ", included[idx]);
        }
        printf("\nrandom int:%i - taxa left: %i\n", r, taxa_left);
        add_taxon_randomly(newt, idx-1, ntaxa);
    }
    print_nodes(newt, newt->root_node);
    printf("\n");
    print_tree(newt, newt->root_node, 1);
    printf("\n");
    int *br = get_branch_list(newt);
    printf("\n");
    free(br);
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




