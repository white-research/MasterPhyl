#ifndef MASTERPHYL_TREE_H
#define MASTERPHYL_TREE_H

class Node {
private:
    int id;
public:
    Node *anc, *desc1, *desc2;

    Node(int id, Node *a, Node *d1, Node *d2);
    int get_id();
};

class Tree {
private:
    int max_id, ntaxa;
public:
    Node *root_node; // *first_taxon -> present in original implementation...

    Tree();

//    void add_node(Node* new_node, int parent_node);
};

//class TreeSet

//typedef struct {
//    int ntrees;
//    float tcost;
//    Tree *trees[100];
//} TreeSet;
//
//extern int get_children(int node, int phyl[], int num_vertices, int children[2]);
//extern int get_ancestor(int node, int phyl[], int num_vertices);
//
//
//extern Tree *make_random_tree(int ntaxa);
//extern void split_tree(Tree *t, int anc_id, int des_id, Tree *subtree_array[2]);
//extern Tree *join_trees(Tree *subt1, Tree *subt2, int sister_id, int new_node_id);
//extern int tree_is_correct(Tree *t);
//extern void free_tree(Tree *t);
//extern int *get_branch_list(Tree *t);
//extern Tree *copy_tree(Tree *orig_tree);
//extern void reroot(Tree *t, int outgroup_id);
//extern void reroot_branch(Tree *t, int branch_anc, int branch_des);
//extern void print_tree(Tree *t, Node *current_node, int level);
//extern void print_nodes(Tree *t, Node *current_node);
//
//void free_node_recur(Node *n);
//Node *find_node(Tree *t, int id, Node *current_node, Node *node_to_return);

#endif
