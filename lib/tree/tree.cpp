#include "tree.h"

Node::Node(int id, Node *a, Node *d1, Node *d2) :id{id}, anc{a}, desc1{d1}, desc2{d2} {
}


Tree::Tree() :ntaxa{0}, max_id{0} {}


int Tree::getNTaxa() {
    return ntaxa;
}

void Tree::addTipNode() {
    if (ntaxa == 0) {
        root_node = new Node(max_id+ 1, nullptr, nullptr, nullptr);
        max_id += 1;
        ntaxa += 1;
    }
}
