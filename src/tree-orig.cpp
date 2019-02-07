#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "tree-orig.h"

#define DEBUG 0

int taxcount_uppass(Tree *t, Node *current_node, int count){
//    printf("On node: %i, current count=%i\n", current_node->id, count);
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

void check_nodes(Tree *t, Node *n){
    if (DEBUG == 1) printf("New node:");
    if (DEBUG == 1) printf("%i\n", n->id);
    if (DEBUG == 1) printf("Checking root node\n");
    if (n == t->root_node){
        if (n->anc != NULL){
            if (DEBUG == 1) printf("Root node does not have a null ancestor\n");
            if (DEBUG == 1) printf("Ancestor is: %i\n", n->anc->id);
            assert(!"Ancestor");
        }
    }
    if (DEBUG == 1) printf("Checking for matching NULL pointers\n");
    if (n->desc1 == NULL){
        if (n->desc2 != NULL){
            if (DEBUG == 1) printf("Desc1 is NULL, but desc2 points to node %i\n", n->desc2->id);
            assert(!"Not both descs pointing to NULL");
        }
    } else{
        if (DEBUG == 1) printf("Checking for pointers match with desc 1 (%i)\n", n->desc1->id);
        if (n->desc1->anc == NULL){
            if (DEBUG == 1) printf("Points to NULL...\n");
        }
        if (n != n->desc1->anc){
            if (DEBUG == 1) printf("Desc1 points to node %i but that has ancestor %i\n", n->desc1->id, n->desc1->anc->id);
            assert(!"descendent and descendents ancestor don't match\n");
        }
    }
    if (DEBUG == 1) printf("Checking for matching NULL pointers part 2\n");
    if (n->desc2 == NULL){
        if (n->desc1 != NULL){
            if (DEBUG == 1) printf("Desc2 is NULL, but desc1 points to node %i\n", n->desc1->id);
            assert(!"Not both descs pointing to NULL");
        }
    } else{
        if (DEBUG >= 1) printf("Checking for pointers match with desc 2\n");
        if (n != n->desc2->anc){
            if (DEBUG == 1) printf("Desc2 points to node %i but that has ancestor %i\n", n->desc2->id, n->desc2->anc->id);
            assert(!"descendent and descendents ancestor don't match\n");
        }
    }
    if (n->desc1 != NULL){
        check_nodes(t, n->desc1);
    }
    if (n->desc2 != NULL){
        check_nodes(t, n->desc2);
    }
}

int tree_is_correct(Tree *t){
    if (DEBUG == 1) printf("Taxon count: %i\n", taxcount_uppass(t, t->root_node, 0));
    assert(taxcount_uppass(t, t->root_node, 0) == t->ntaxa);
    
    // check nodes and links
    check_nodes(t, t->root_node);
    return 1;
}


void add_taxon_nonrandomly(Tree *t, int id, int max_taxa){
    if (DEBUG >= 1) printf("\n\nAdding tip %i randomly\n", id);
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->id = id;
    new_node->anc = NULL;
    new_node->desc1 = NULL;
    new_node->desc2 = NULL;
    if (t->ntaxa == 0){ // ADD INITIAL TAXON
        if (DEBUG >= 1) printf("\nAdding first taxon\n");
        Node *root_node = (Node *)malloc(sizeof(Node));
        root_node->anc = NULL;
        root_node->id = max_taxa;
        root_node->desc1 = new_node;
        root_node->desc2 = NULL;
        new_node->anc = root_node;
        t->ntaxa = 1;
        t->max_id = max_taxa;
        t->root_node = root_node;
        if (DEBUG >= 1) printf("Root node: id=%i, desc1 id=%i\n", t->root_node->id, t->root_node->desc1->id);
        if (DEBUG >= 1) printf("Des1 node: id=%i, root  id=%i\n", new_node->id, new_node->anc->id);
    }
    else {
        if (t->ntaxa == 1){ // ADD SECOND TAXON
            if (DEBUG >= 1) printf("\nAbout to add root's descendent 2\n");
            if (DEBUG >= 1) printf("Root:%i\n", t->root_node->id);
            t->root_node->desc2 = new_node;
            if (DEBUG >= 1) printf("About to add root as node's ancestor\n");
            new_node->anc = t->root_node;
            if (DEBUG >= 1) printf("About to increment total taxa\n");
            t->ntaxa++;
            if (DEBUG >= 1) printf("Root node: id=%i, desc1 id=%i\n", t->root_node->id, t->root_node->desc1->id);
            if (DEBUG >= 1) printf("Root node: id=%i, desc2 id=%i\n", t->root_node->id, t->root_node->desc2->id);
            if (DEBUG >= 1) printf("Des1 node: id=%i, root  id=%i\n", new_node->id, new_node->anc->id);
            if (DEBUG >= 1) printf("Done.\n\n");
        }
        else{
            if (DEBUG >= 1) printf("\nAbout to insert new node into tree\n");
            Node *old_desc1 = t->root_node->desc1;
            if (DEBUG >= 1) printf("Inserting onto branch from %i to %i\n", old_desc1->anc->id, old_desc1->id);
            Node *new_internal_node = (Node *)malloc(sizeof(Node));
            new_internal_node->id = t->max_id+1;
            new_internal_node->anc = t->root_node;
            t->root_node->desc1 = new_internal_node;
            old_desc1->anc = new_internal_node;
            new_internal_node->desc1 = new_node;
            new_internal_node->desc2 = old_desc1;
            new_node->anc = new_internal_node;
            t->ntaxa++;
            t->max_id++;
            if (DEBUG >= 1) printf("New node: id=%i, anc id=%i\n", new_node->id, new_node->anc->id);
            if (DEBUG >= 1) printf("total taxa in tree: %i\n\n", t->ntaxa);
        }
    }
}


Tree *make_nonrandom_tree(int ntaxa){
    int included[ntaxa];
    for (int idx=0; idx<ntaxa; idx++){
        included[idx] = 1;
    }
    Tree *newt = (Tree *)malloc(sizeof(Tree));
    newt->ntaxa=0;
    for(int taxa_left=ntaxa; taxa_left > 0; taxa_left--){
        int r = rand() % taxa_left;
        int counter = -1, idx = 0;
        while (counter < r) {
            counter += included[idx];
            if (DEBUG >= 1) printf("%i:", idx);
            if (DEBUG >= 1) printf("%i ", included[idx]);
            idx++;
        }
        included[idx-1] = 0;
//        printf("\nIncluded\n");
        for (int idx=0; idx<ntaxa; idx++){
            if (DEBUG >= 1) printf("%i ", included[idx]);
        }
//        printf("\nrandom int:%i - taxa left: %i\n", r, taxa_left);
        add_taxon_nonrandomly(newt, idx-1, ntaxa);
    }
    if (DEBUG >= 1) print_nodes(newt, newt->root_node);
    if (DEBUG >= 1) printf("\n");
    if (DEBUG >= 1) print_tree(newt, newt->root_node, 1);
    if (DEBUG >= 1) printf("\n");
    int *br = get_branch_list(newt);
    if (DEBUG >= 1) printf("Printed branches\n");
    free(br);
    return newt;
}




void add_taxon_randomly(Tree *t, int id, int max_taxa){
    if (DEBUG >= 1) printf("\n\nAdding tip %i randomly\n", id);
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->id = id;
    new_node->anc = NULL;
    new_node->desc1 = NULL;
    new_node->desc2 = NULL;
    if (t->ntaxa == 0){ // ADD INITIAL TAXON
        if (DEBUG >= 1) printf("\nAdding first taxon\n");
        Node *root_node = (Node *)malloc(sizeof(Node));
        root_node->anc = NULL;
        root_node->id = max_taxa;
        root_node->desc1 = new_node;
        root_node->desc2 = NULL;
        new_node->anc = root_node;
        t->ntaxa = 1;
        t->max_id = max_taxa;
        t->root_node = root_node;
        if (DEBUG >= 1) printf("Root node: id=%i, desc1 id=%i\n", t->root_node->id, t->root_node->desc1->id);
        if (DEBUG >= 1) printf("Des1 node: id=%i, root  id=%i\n", new_node->id, new_node->anc->id);
    }
    else {
        if (t->ntaxa == 1){ // ADD SECOND TAXON
            if (DEBUG >= 1) printf("\nAbout to add root's descendent 2\n");
            if (DEBUG >= 1) printf("Root:%i\n", t->root_node->id);
            t->root_node->desc2 = new_node;
            if (DEBUG >= 1) printf("About to add root as node's ancestor\n");
            new_node->anc = t->root_node;
            if (DEBUG >= 1) printf("About to increment total taxa\n");
            t->ntaxa++;
            if (DEBUG >= 1) printf("Root node: id=%i, desc1 id=%i\n", t->root_node->id, t->root_node->desc1->id);
            if (DEBUG >= 1) printf("Root node: id=%i, desc2 id=%i\n", t->root_node->id, t->root_node->desc2->id);
            if (DEBUG >= 1) printf("Des1 node: id=%i, root  id=%i\n", new_node->id, new_node->anc->id);
            if (DEBUG >= 1) printf("Done.\n\n");
        }
        else{
            if (DEBUG >= 1) printf("\nAbout to insert new node into tree\n");
            int rand_node = rand() %id;
            Node *old_desc=NULL;
            old_desc = find_node(t, rand_node, t->root_node, old_desc);
            if (DEBUG >= 1) printf("Inserting onto branch from %i to %i\n", old_desc->anc->id, old_desc->id);
            Node *new_internal_node = (Node *)malloc(sizeof(Node));
            new_internal_node->id = t->max_id+1;
            new_internal_node->anc = old_desc->anc;
            if (old_desc->anc->desc1 == old_desc) old_desc->anc->desc1 = new_internal_node;
            else old_desc->anc->desc2 = new_internal_node;
            
            new_internal_node->desc1 = new_node;
            new_node->anc = new_internal_node;
            new_internal_node->desc2 = old_desc;
            old_desc->anc = new_internal_node;
            
            t->ntaxa++;
            t->max_id++;
            if (DEBUG >= 1) printf("New node: id=%i, anc id=%i\n", new_node->id, new_node->anc->id);
            if (DEBUG >= 1) printf("total taxa in tree: %i\n\n", t->ntaxa);
            
        }
    }
}


Tree *make_random_tree(int ntaxa){
    int included[ntaxa];
    for (int idx=0; idx<ntaxa; idx++){
        included[idx] = 1;
    }
    Tree *newt = (Tree *)malloc(sizeof(Tree));
    newt->ntaxa=0;
    for(int taxa_left=ntaxa; taxa_left > 0; taxa_left--){
        int r = rand() % taxa_left;
        int counter = -1, idx = 0;
        while (counter < r) {
            counter += included[idx];
            if (DEBUG >= 1) printf("%i:", idx);
            if (DEBUG >= 1) printf("%i ", included[idx]);
            idx++;
        }
        included[idx-1] = 0;
//        printf("\nIncluded\n");
        for (int idx=0; idx<ntaxa; idx++){
            if (DEBUG >= 1) printf("%i ", included[idx]);
        }
//        printf("\nrandom int:%i - taxa left: %i\n", r, taxa_left);
        add_taxon_nonrandomly(newt, idx-1, ntaxa);
    }
    if (DEBUG >= 1) print_nodes(newt, newt->root_node);
    if (DEBUG >= 1) printf("\n");
    if (DEBUG >= 1) print_tree(newt, newt->root_node, 1);
    if (DEBUG >= 1) printf("\n");
    int *br = get_branch_list(newt);
    if (DEBUG >= 1) printf("Printed branches\n");
    free(br);
    return newt;
}




void print_nodes(Tree *t, Node *current_node){
    printf("\nCurrent node:%2i, ", current_node->id);
    if (current_node->desc1 != NULL){
        printf("Descendent 1:%2i, ", current_node->desc1->id);
        printf("Descendent 2:%2i, ", current_node->desc2->id);
        print_nodes(t, current_node->desc1);
    } else{
        printf(" ");
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


// ***NEEDS WORK***
void print_tree(Tree *t, Node *current_node, int level){
    
    for(int idx = 1; idx<level; idx++){
        printf("  ");
    }
    printf("--%2i", current_node->id);
    if (current_node->desc1 != NULL){
        print_tree(t, current_node->desc1, level+1);
    } else{
        printf("\n");
    //    for(int idx = 0; idx<level; idx++){
    //        printf("    ");
    //    }
    }
    if (current_node->desc2 != NULL){
        print_tree(t, current_node->desc2, level+1);
    }// else{
    //    printf("\n  ");
    //    for(int idx = 0; idx<level; idx++){
    //        printf("    ");
    //    }
    // }
}


int _fill_branch_list_recursive(Tree *t, int *branch_list, Node *current_node, int branch_counter){
    //printf("Branch # %i from node %i\n", branch_counter, current_node->id);
    if (current_node->desc1 != NULL){
        //printf("  D1: From node %i to %i\n", current_node->id, current_node->desc1->id);
        branch_list[branch_counter*2] = current_node->id;
        branch_list[branch_counter*2+1] = current_node->desc1->id;
        branch_counter++;
        branch_counter = _fill_branch_list_recursive(t, branch_list, current_node->desc1, branch_counter);
    }
    //printf("Back to node #%i\n", current_node->id);
    if (current_node->desc2 != NULL){
        //printf("  D2: From node %i to %i\n", current_node->id, current_node->desc2->id);
        branch_list[branch_counter*2] = current_node->id;
        branch_list[branch_counter*2+1] = current_node->desc2->id;
        branch_counter++;
        branch_counter = _fill_branch_list_recursive(t, branch_list, current_node->desc2, branch_counter);
    }
    return branch_counter;
}


int *get_branch_list(Tree *t){
    int *branch_list = (int *)malloc(sizeof(int)*2*(2*t->ntaxa - 2));
    assert(branch_list !=NULL);
    int final_branch = _fill_branch_list_recursive(t, branch_list, t->root_node, 0);
    if (DEBUG >= 1) printf("Final branch_count = %i\n", final_branch);
    assert(final_branch == 2*t->ntaxa - 2);
//    for (int idx = 0; idx < 2*t->ntaxa - 2; idx++){
//        printf("%2i -> %2i\n", branch_list[2*idx], branch_list[2*idx+1]);
//    }
    if (DEBUG >= 1) printf("Done\n");
    
    assert(branch_list !=NULL);
    return branch_list;
}


Node *find_node(Tree *t, int id, Node *current_node, Node *node_to_return){
    if (DEBUG == 1) printf("\n Finding node %i, currently on node %i\n", id, current_node->id);
    if (current_node->id == id){
        if (DEBUG == 1) printf("  Found it!\n");
        assert(node_to_return == NULL && "Node appears twice in tree");
        node_to_return = current_node;
    }
    if (DEBUG == 1) printf("  Checking %i's descendant1...\n", current_node->id);
    if (current_node->desc1 != NULL){
        node_to_return = find_node(t, id, current_node->desc1, node_to_return);
    }
    if (DEBUG == 1) printf("  Checking %i's descendant2...\n", current_node->id);
    if (current_node->desc2 != NULL){
        node_to_return = find_node(t, id, current_node->desc2, node_to_return);
    }
    if (DEBUG == 1) printf("  Finished on node %i\n", current_node->id);
    return node_to_return;
}


void copy_nodes_recur(Tree *orig_tree, Node *orig_tree_node, Node *subtree_node){
    subtree_node->id = orig_tree_node->id;
    //subtree_node->anc = orig_tree_node->anc;
    if (orig_tree_node->desc1 != NULL){
        Node *new_desc1 = (Node *)malloc(sizeof(Node));
        new_desc1->anc = subtree_node;
        subtree_node->desc1 = new_desc1;
        copy_nodes_recur(orig_tree, orig_tree_node->desc1, new_desc1);
    } else{
        subtree_node->desc1 = NULL;
    }
    if (orig_tree_node->desc2 != NULL){
        Node *new_desc2 = (Node *)malloc(sizeof(Node));
        new_desc2->anc = subtree_node;
        subtree_node->desc2 = new_desc2;
        copy_nodes_recur(orig_tree, orig_tree_node->desc2, new_desc2);
    } else{
        subtree_node->desc2 = NULL;
    }
}


Tree *copy_tree(Tree *orig_tree){
    Tree *new_tree = (Tree *)malloc(sizeof(Tree));
    if (DEBUG >= 1) printf("Copying ntaxa\n");
    new_tree->ntaxa = orig_tree->ntaxa;
    if (DEBUG >= 1) printf("Copied ntaxa\n");
    if (orig_tree->root_node != NULL){
        Node *new_root_node = (Node *)malloc(sizeof(Node));
        new_tree->root_node = new_root_node;
        new_tree->root_node->anc = NULL;
        copy_nodes_recur(orig_tree, orig_tree->root_node, new_root_node);
    } else{
        new_tree->root_node = NULL;
    }
    return new_tree;
}


void copy_nodes_inc_recur(Tree *orig_tree, Node *orig_tree_node, Node *subtree_node){
    subtree_node->id = orig_tree_node->id+20;
    //subtree_node->anc = orig_tree_node->anc;
    if (orig_tree_node->desc1 != NULL){
        Node *new_desc1 = (Node *)malloc(sizeof(Node));
        new_desc1->anc = subtree_node;
        subtree_node->desc1 = new_desc1;
        copy_nodes_inc_recur(orig_tree, orig_tree_node->desc1, new_desc1);
    } else{
        subtree_node->desc1 = NULL;
    }
    if (orig_tree_node->desc2 != NULL){
        Node *new_desc2 = (Node *)malloc(sizeof(Node));
        new_desc2->anc = subtree_node;
        subtree_node->desc2 = new_desc2;
        copy_nodes_inc_recur(orig_tree, orig_tree_node->desc2, new_desc2);
    } else{
        subtree_node->desc2 = NULL;
    }
}


Tree *copy_tree_inc(Tree *orig_tree){
    Tree *new_tree = (Tree *) malloc(sizeof(Tree));
    if (DEBUG >= 1) printf("Copying ntaxa\n");
    new_tree->ntaxa = orig_tree->ntaxa;
    if (DEBUG >= 1) printf("Copied ntaxa\n");
    if (orig_tree->root_node != NULL){
        Node *new_root_node = (Node *)malloc(sizeof(Node));
        new_tree->root_node = new_root_node;
        copy_nodes_inc_recur(orig_tree, orig_tree->root_node, new_root_node);
    } else{
        new_tree->root_node = NULL;
    }
    return new_tree;
}


int count_tips(Tree *t, Node *n){
    int count = 0;
    if (n->desc1 != NULL){
        count = count_tips(t, n->desc1);
    }
    if (n->desc2 != NULL){
        count = count + count_tips(t, n->desc2);
    }
    else{
        assert(n->desc1 == NULL);
        count = 1;
    }
    return count;
}


void split_tree(Tree *t, int anc_id, int des_id, Tree *subtree_array[2]){
    
    if (DEBUG >= 1) printf("  Tree has %i taxa\n", t->ntaxa);
    if (DEBUG >= 1) printf("  Making list of branches to check that branch to split at exists.\n");
    int *branches = get_branch_list(t);
    int branch_in_tree = 0;
    for (int idx = 0; idx < 2*t->ntaxa - 2; idx++){
        if (DEBUG >= 1) printf("   Index: %i\n", idx);
        if (DEBUG >= 1) printf("   Branch %i -> %i\n", branches[2*idx], branches[2*idx+1]);
        if (branches[2*idx] == anc_id){
            if (branches[2*idx+1] == des_id){
                branch_in_tree = 1;
            }
        }
    }
    assert(branch_in_tree==1 && "Attempting to split tree at a branch that does not exist");
    free(branches);
    
    
    if (DEBUG >= 1) printf("Branch to split at = %i -> %i\n", anc_id, des_id);
    
    Tree *copy_of_tree = copy_tree(t);
    if (DEBUG >= 1) printf("Copy of original tree within split tree function:\n");
    if (DEBUG >= 1) print_tree(copy_of_tree, copy_of_tree->root_node, 0);
    if (DEBUG >= 1) print_nodes(copy_of_tree, copy_of_tree->root_node);
    if (DEBUG >= 1) printf("\n");
    tree_is_correct(copy_of_tree);
    Tree *subtree1 = (Tree *)malloc(sizeof(Tree));
    Tree *subtree2 = (Tree *)malloc(sizeof(Tree));
    
    
    if (DEBUG >= 1) printf("Created subtrees via malloc, finding node to remove with id: %i\n", anc_id);
    Node *node_to_remove = NULL; 
    node_to_remove = find_node(copy_of_tree, anc_id, copy_of_tree->root_node, node_to_remove);
    assert(node_to_remove != NULL);
    if (DEBUG >= 1) printf("Found node to remove, id: %i\n", node_to_remove->id);
    assert(node_to_remove->desc1 != NULL && node_to_remove->desc2 != NULL && "Can't split tree at non-existent branch");
    
    if (node_to_remove->id == copy_of_tree->root_node->id){ // if splitting at root node
        if (DEBUG >= 1) printf("Splitting tree at root node %i\n", node_to_remove->id);
        subtree1->root_node = node_to_remove->desc1;
        subtree1->root_node->anc = NULL;
        subtree2->root_node = node_to_remove->desc2;
        subtree2->root_node->anc = NULL;
        free(node_to_remove);
    } else{ //if splitting at interior node other than root node
        if (DEBUG >= 1) printf("Splitting tree at interior node %i\n", node_to_remove->id);
        Node *node_to_shorten;
        int d;
        Node *node_to_remove_anc = node_to_remove->anc;
        if (DEBUG >= 1) printf("Node_to_remove_anc is: %i\n", node_to_remove_anc->id);
        if (node_to_remove_anc->desc1->id == node_to_remove->id){
            d = 1;
        } else {
            d = 2;
        }
        
        subtree1->root_node = copy_of_tree->root_node;
        
        if (node_to_remove->desc1->id == des_id){
            subtree2->root_node = node_to_remove->desc1;
            node_to_shorten = node_to_remove->desc2;
        } else{
            subtree2->root_node = node_to_remove->desc2;
            node_to_shorten = node_to_remove->desc1;
        }
        if (DEBUG >= 1) printf("Node_to_shorten is: %i\n", node_to_shorten->id);
        node_to_shorten->anc = node_to_remove_anc;
        if (d == 1){ node_to_remove_anc->desc1 = node_to_shorten; }
        else { node_to_remove_anc->desc2 = node_to_shorten; }
        subtree2->root_node->anc = NULL;
        if (DEBUG >= 1) printf("subtree1 root assigned to node %i", subtree1->root_node->id);
        if (DEBUG >= 1) printf("subtree2 root assigned to node %i", subtree2->root_node->id);
        
        free(node_to_remove);
    }
    
    subtree1->ntaxa = count_tips(subtree1, subtree1->root_node);
    if (DEBUG >= 1) printf("subtree1 has %i taxa\n", subtree1->ntaxa);
    if (DEBUG >= 1) print_nodes(subtree1, subtree1->root_node);
    subtree2->ntaxa = count_tips(subtree2, subtree2->root_node);
    if (DEBUG >= 1) printf("subtree2 has %i taxa\n", subtree2->ntaxa);
    if (DEBUG >= 1) print_nodes(subtree2, subtree2->root_node);
    
    copy_of_tree->root_node = NULL;
    free_tree(copy_of_tree);
    
    tree_is_correct(subtree1);
    tree_is_correct(subtree2);
    
    subtree_array[0] = subtree1;
    subtree_array[1] = subtree2;
    
}


Tree *join_trees(Tree *st1, Tree *st2, int sister_id, int new_node_id){ // inserts subtree2 into subtree1 next to node: new_sister_id
    Tree *subt1 = copy_tree(st1);
    if (DEBUG >= 1) printf("\nChecking subtree 1\n");
    tree_is_correct(subt1);
    Tree *subt2 = copy_tree(st2);
    if (DEBUG >= 1) printf("\nChecking subtree 2\n");
    tree_is_correct(subt2);
    Tree *joined_tree = (Tree *)malloc(sizeof(Tree));
    assert(subt1->ntaxa > 1 && "Cannot spr on size 1 tree");
    Node *sister = NULL;
    sister = find_node(subt1, sister_id, subt1->root_node, sister);
    if (DEBUG >= 1) printf("Joining trees, to sister node %i\n", sister_id);
    Node *new_internal_node = (Node *)malloc(sizeof(Node));
    new_internal_node->id = new_node_id;
    Node *sisters_anc = sister->anc;
    if (sisters_anc == NULL){
        if (DEBUG >= 1) printf("NULL node\n");
        new_internal_node->anc = NULL;
        joined_tree->root_node = new_internal_node;
        new_internal_node->desc1 = subt1->root_node;
        new_internal_node->desc1->anc = new_internal_node;
        new_internal_node->desc2 = subt2->root_node;
        new_internal_node->desc2->anc = new_internal_node;
    } else{
        if (DEBUG >= 1) printf("Sisters ancestor is %i\n", sisters_anc->id);
        
        new_internal_node->desc1 = subt2->root_node;
        subt2->root_node->anc = new_internal_node;
        
        new_internal_node->desc2 = sister;
        sister->anc = new_internal_node;
        
        new_internal_node->anc = sisters_anc;
        if (sisters_anc->desc1->id == sister_id){
            sisters_anc->desc1 = new_internal_node;
        } else{
            sisters_anc->desc2 = new_internal_node;
        }
        
        joined_tree->root_node = subt1->root_node;
        
        if (DEBUG >= 1) printf("11\n");
    }
    joined_tree->ntaxa = count_tips(joined_tree, joined_tree->root_node);
    assert(joined_tree->ntaxa == subt1->ntaxa + subt2->ntaxa);
    
    subt1->root_node = NULL;
    subt2->root_node = NULL;
    free_tree(subt1);
    free_tree(subt2);
    if (DEBUG >= 1) printf("14\n");
    
    if (DEBUG >= 1) print_tree(joined_tree, joined_tree->root_node, 0);
    if (joined_tree->root_node->anc != NULL){
        if (DEBUG >= 1) printf("Error with root node's ancestor: ");
        if (DEBUG >= 1) printf("Root nodes ancestor is: %i\n", joined_tree->root_node->anc->id);
    }
    else { if (DEBUG >= 1) printf("Joined tree: Root node's ancestor is NULL\n"); }
    assert(joined_tree->root_node->anc == NULL);
    if (DEBUG >= 1) printf("\nChecking joined tree\n");
    tree_is_correct(joined_tree);
    return joined_tree;
}


void reroot(Tree *t, int outgroup_id){
    if (DEBUG >= 1) printf("\nStarting rerooting on node %i\n", outgroup_id);
    if (DEBUG >= 1) printf("Current root is %i\n", t->root_node->id);
    if (DEBUG >= 1) print_nodes(t, t->root_node);
    Node *outgroup_node = NULL;
    outgroup_node = find_node(t, outgroup_id, t->root_node, outgroup_node);
    
    reroot_branch(t, outgroup_node->anc->id, outgroup_id);
}


void reroot_branch(Tree *t, int branch_anc, int branch_des){
    
    if (DEBUG >= 1) printf("Finding anc node: %i\n", branch_anc);
    Node *anc_node = NULL;
    anc_node = find_node(t, branch_anc, t->root_node, anc_node);
    if (DEBUG >= 1) printf("Finding des node: %i\n", branch_des);
    Node *des_node = NULL;
    des_node = find_node(t, branch_des, t->root_node, des_node);
    
    assert(anc_node != NULL && des_node != NULL && "Cannot reroot on non-existent nodes");
    assert(des_node->anc == anc_node);
    if (DEBUG >= 1) printf("Branch exists\n");
    
    Node *old_root = t->root_node;
    
    if (old_root != anc_node){
        if (DEBUG >= 1) printf("Tree needs rerooting...\n");
        Node *new_root = (Node *)malloc(sizeof(Node));
        if (DEBUG >= 1) printf("Created new root node\n");
        
        new_root->anc = NULL;
        new_root->id = t->ntaxa + 1;
        new_root->desc1 = anc_node;
        new_root->desc2 = des_node;
        if (DEBUG >= 1) printf("New root node, desc1=%i and desc2=%i\n", new_root->desc1->id, new_root->desc2->id );
        des_node->anc = new_root;
        
        Node *node_to_reroot = anc_node;
        Node *old_desc = des_node;
        Node *new_anc = new_root;
        Node *next_node_to_reroot = node_to_reroot->anc;
        if (DEBUG >= 1) printf("Starting iterations...\n");
        
        while (next_node_to_reroot != NULL){
            if (DEBUG >= 1) printf("Currently rerooting node: %i\n", node_to_reroot->id);
            if (node_to_reroot->desc1 == old_desc){ node_to_reroot->desc1 = next_node_to_reroot; }
            else { node_to_reroot->desc2 = next_node_to_reroot; }
            if (DEBUG >= 1) printf(" with rearranged descendents %i and %i\n", node_to_reroot->desc1->id, node_to_reroot->desc2->id);
            if (DEBUG >= 1) printf("  and ancestor %i\n", node_to_reroot->anc->id);
            node_to_reroot->anc = new_anc;
            if (DEBUG >= 1) printf("   which is changed to %i\n", node_to_reroot->anc->id);
            new_anc = node_to_reroot;
            old_desc= node_to_reroot;
            node_to_reroot = next_node_to_reroot;            
            next_node_to_reroot = next_node_to_reroot->anc;
            
        }
        if (DEBUG >= 1) printf("\nExited while loop, node_to_reroot is %i\n", node_to_reroot->id);
        if (DEBUG >= 1) printf("Node_to_reroot descs are %i and %i\n", node_to_reroot->desc1->id, node_to_reroot->desc2->id);
        if (DEBUG >= 1) printf("Old desc: %i\n", old_desc->id);
        
        if (DEBUG >= 1) printf("\nTree's root node is: %i\n", t->root_node->id);
        assert(node_to_reroot == t->root_node);
        if (DEBUG >= 1) printf("Reached old root\n");
        if (node_to_reroot->desc1 == old_desc){
            if (DEBUG >= 1) printf("Descendent 1 is last rerooted node (future ancestor)\n\n");
            if (DEBUG >= 1) printf(" so switching descendent 2 (%i)'s ancestor (%i) to %i\n", node_to_reroot->desc2->id, node_to_reroot->desc2->anc->id, old_desc->id);
            
            //node_to_reroot->desc2->anc = old_desc;
            
            
            node_to_reroot->desc2->anc = old_desc;
            if (old_desc->desc1 == node_to_reroot){
                if (DEBUG >= 1) printf("Also switching desc1 (%i) of node %i to %i", old_desc->desc1->id, old_desc->id, node_to_reroot->desc2->id);
                old_desc->desc1 = node_to_reroot->desc2;
            } else{
                if (DEBUG >= 1) printf("Also switching desc2 (%i) of node %i to %i", old_desc->desc2->id, old_desc->id, node_to_reroot->desc2->id);
                old_desc->desc2 = node_to_reroot->desc2;
            }
        }else{
            if (DEBUG >= 1) printf("Descendent 2 is last rerooted node (future ancestor)\n\n");
            if (DEBUG >= 1) printf(" so switching descendent 1 (%i)'s ancestor (%i) to %i\n", node_to_reroot->desc1->id, node_to_reroot->desc1->anc->id, old_desc->id);
            node_to_reroot->desc1->anc = old_desc;
            if (old_desc->desc1 == node_to_reroot){
                if (DEBUG >= 1) printf("Also switching desc1 (%i) of node %i to %i", old_desc->desc1->id, old_desc->id, node_to_reroot->desc1->id);
                old_desc->desc1 = node_to_reroot->desc1;
            } else{
                if (DEBUG >= 1) printf("Also switching desc2 (%i) of node %i to %i", old_desc->desc2->id, old_desc->id, node_to_reroot->desc1->id);
                old_desc->desc2 = node_to_reroot->desc1;
            }
        }
        t->root_node = new_root;
        new_root->id = node_to_reroot->id;
        if (DEBUG >= 1) print_nodes(t, t->root_node);
        free(node_to_reroot);
        
        // Assign id to new root
        // Free old root
    }
    //assert(!"HELP");
}


void free_node_recur(Node *n){
    if (n->desc1 != NULL){
        free_node_recur(n->desc1);
    }
    if (n->desc2 != NULL){
        free_node_recur(n->desc2);
    }
    free(n);
}


void free_tree(Tree *t){
    if (t->root_node != NULL){
        free_node_recur(t->root_node);
    }
    free(t);
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

int main(){
    return 0;
}


