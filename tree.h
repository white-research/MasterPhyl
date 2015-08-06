#ifndef _tree_h
#define _tree_h

typedef struct treeNode{
    int id;
    struct treeNode *anc, *desc1, *desc2;
} Node;

typedef struct {
    int ntaxa, max_id;
    Node *root_node, *first_taxon;
} Tree;

extern int get_children(int node, int phyl[], int num_vertices, int children[2]);
extern int get_ancestor(int node, int phyl[], int num_vertices);
extern Tree *make_random_tree(int ntaxa);

#endif
