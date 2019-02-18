#include <random>

#include "tree.h"

Node::Node(int id, Node *a, Node *d1, Node *d2) :id{id}, anc{a}, desc1{d1}, desc2{d2} {}


Tree::Tree() :ntips{0}, nnodes{0}, nbranches{0}, max_id{0} {}


int Tree::getNTips() {
    return ntips;
}

void Tree::addTipRandomly() {
    if (ntips == 0) {
        root_node = new Node(max_id+1, nullptr, nullptr, nullptr);
        max_id += 1;
        ntips += 1;
        nnodes += 1;
    } else if (ntips == 1){
        Node* first_tip = root_node;
        Node* new_tip = new Node(max_id+1, nullptr, nullptr, nullptr);
        max_id += 1;
        root_node = new Node(max_id+1, nullptr, first_tip, new_tip);
        ntips += 1;
        max_id += 1;
        nnodes += 2;
        nbranches += 2;
    } else {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(1, nbranches);
        int insertion_branch = distribution(generator);
        int count = insertNodeAtBranch(insertion_branch, 1, root_node, root_node->desc1);
        if (count < insertion_branch){
            insertNodeAtBranch(insertion_branch, count+1, root_node, root_node->desc2);
        }
    }
}

int Tree::insertNodeAtBranch(int insert_number, int current_branch, Node* anc, Node* desc){
    if (insert_number == current_branch){
        Node *new_internal_node = new Node(max_id+1, anc, nullptr, nullptr);
        max_id += 1;
        Node *new_tip_node = new Node(max_id+1, new_internal_node, nullptr, nullptr);
        max_id += 1;
        if (anc->desc1 == desc) {
            anc->desc1 = new_internal_node;
            new_internal_node->desc1 = desc;
            new_internal_node->desc2 = new_tip_node;
        }
        else {
            anc->desc2 = new_internal_node;
            new_internal_node->desc2 = desc;
            new_internal_node->desc1 = new_tip_node;
        }
        desc->anc = new_internal_node;
        nnodes += 2;
        nbranches += 2;
        ntips += 1;
        return current_branch;
    }
    else {
        int count = insertNodeAtBranch(insert_number, current_branch+1, desc, desc->desc1);
        if (count < insert_number){
            insertNodeAtBranch(insert_number, count+1, desc, desc->desc2);
        }
        else {
            return count;
        }
    }
}
