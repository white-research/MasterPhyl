#include "tree.h"

Node::Node(int id, Node *a, Node *d1, Node *d2) :id{id}, anc{a}, desc1{d1}, desc2{d2} {
}

Tree::Tree() :ntaxa{0}, max_id{0} {}

//int main(){
//    return 0;
//}


