#ifndef MASTERPHYL_TREE_H
#define MASTERPHYL_TREE_H

class Node {
private:
    int id;
public:
    Node *anc, *desc1, *desc2;

    Node(int id, Node *a, Node *d1, Node *d2);
    int get_id(){return id;}
};

class Tree {
private:
    int max_id, ntips, nnodes, nbranches;
    Node *root_node;

    int insertNodeAtBranch(int insert_number, int current_branch, Node* anc, Node* desc);

public:

    Tree();
//TODO: copy constructor
//TODO: destructor

    int getNTips() {return ntips;};
    int getNBranches() {return nbranches;};
    int getNNodes() {return nnodes;};
    int getRootID();
    std::vector<std::array<int, 2>> * getBranchList();

    int addTipRandomly();
//TODO: split tree: extern void split_tree(Tree *t, int anc_id, int des_id, Tree *subtree_array[2]);
//TODO: join two trees: extern Tree *join_trees(Tree *subt1, Tree *subt2, int sister_id, int new_node_id);
//TODO: reroot tree: extern void reroot(Tree *t, int outgroup_id); & extern void reroot_branch(Tree *t, int branch_anc, int branch_des);

//TODO: check tree is correct: extern int tree_is_correct(Tree *t);

};

#endif
