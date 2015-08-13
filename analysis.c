#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "matrix.h"
#include "tree.h"
#include "dynamic_homology.h"

#define DEBUG 1 //0 means no debugging output, 1 means basic debugging, 2 means detailed debugging

int discrete_optimization(Tree *t, Partition *p){
    return 1;
}

float continuous_optimization(Tree *t, Partition *p){
    return 1.0;
}






float get_cost(Tree *t, Matrix *m){
    print_matrix(m);
    float total_cost = 1000.0;
    for (int idx = 0; idx < m->npart; idx++){
        printf("Working on partition %i of type %i\n", idx+1, m->partitions[idx]->data_type);
        if (m->partitions[idx]->data_type == 0){
            total_cost = total_cost+discrete_optimization(t, m->partitions[idx]);
        } else {
            if (m->partitions[idx]->data_type == 1){
                total_cost = total_cost+continuous_optimization(t, m->partitions[idx]);
            } else{
                if (m->partitions[idx]->data_type == 2){
                    total_cost = total_cost+dynamic_homology(t, m->partitions[idx], 1.0);
                }
                else{
                    assert(!"Unknown partition type");
                }
            }
        }
    }
    return total_cost;
}

void spr(Tree *start_tree, Matrix *m, float current_best_cost){
    printf("Checking start tree\n");
    tree_is_correct(start_tree);
    int *all_branches = get_branch_list(start_tree);
    for (int idx=0; idx < 1 /*2*start_tree->ntaxa-2 */; idx++){
        int branch_anc = all_branches[2*idx];
        int branch_des = all_branches[2*idx+1];
        
        printf("\n\n\n%i - Starting SPR at branch %i to %i\n\n Starting tree:\n", idx, branch_anc, branch_des);
        print_tree(start_tree, start_tree->root_node, 0);
        
        printf("\n\n Splitting trees at branch %i -> %i\n", branch_anc, branch_des);
        Tree *subtrees[2] = {NULL, NULL};
        split_tree(start_tree, branch_anc, branch_des, subtrees);
        printf("\n Created subtrees, with %i and %i taxa\n", subtrees[0]->ntaxa, subtrees[1]->ntaxa);
        //printf("Subtree1:\n");
        //print_tree(subtrees[0], subtrees[0]->root_node, 0);
        //print_nodes(subtrees[0], subtrees[0]->root_node);
        //printf("\nSubtree2:\n");
        //print_tree(subtrees[1], subtrees[1]->root_node, 0);
        //print_nodes(subtrees[1], subtrees[1]->root_node);
        //printf("\n\n");
        
        //printf("\n  Print of Start tree after making subtrees from it:\n");
        //print_tree(start_tree, start_tree->root_node, 0);
        //print_nodes(start_tree, start_tree->root_node);
        //printf("\n\n");
        
        if (subtrees[0]->ntaxa > 1){  // if subtree 1 has more than 1 tip:
            int *branches_to_attach_to = get_branch_list(subtrees[0]);
            
            for (int idx=0; idx < 1 /* 2*subtrees[0]->ntaxa-2 */; idx++){  // for branch in subtree branches
                printf("\n\nREJOINING\nReattaching as sister to node %i ", branches_to_attach_to[idx*2+1]);
                printf("on branch %i to %i\n", branches_to_attach_to[idx*2], branches_to_attach_to[idx*2+1]);
                //printf("\n  Print of Start tree after copying subtrees but before joing them:\n");
                //print_tree(start_tree, start_tree->root_node, 0);
                
                // reattach subt2 at this branch
                printf("\n\nStarting join_trees function:\n");
                Tree *rearranged_tree = join_trees(subtrees[0], subtrees[1], branches_to_attach_to[2*idx+1], branch_anc);
                printf("\n  Print of joined tree:\n");
                print_tree(rearranged_tree, rearranged_tree->root_node, 0);
                //printf("\n  Print of Start tree:\n");
                //print_tree(start_tree, start_tree->root_node, 0);
                //printf("Joined subtrees\n");
                
                // make sure it is rooted in the right place
                reroot(rearranged_tree, 0);
                printf("Finished rerooting tree\n");
                //printf("\n  Print of Start tree:\n");
                //print_tree(start_tree, start_tree->root_node, 0);
                
                // get cost
                float tree_cost = get_cost(rearranged_tree, m);
                printf("Calculated cost: %f\n", tree_cost);
                
                // based on cost, figure out what to do
                if (tree_cost > current_best_cost){
                    free_tree(rearranged_tree);
                }
            }
            
            free(branches_to_attach_to);
            
        }
        free_tree(subtrees[0]);
        free_tree(subtrees[1]);
        
        // free any created and unneeded structures
    }
    
    
    printf("\n");
    
    
    free(all_branches);
}



