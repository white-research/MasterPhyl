#include <array>
#include <random>


#include "tree.h"

Node::Node(int id, Node *a, Node *d1, Node *d2) :id{id}, anc{a}, desc1{d1}, desc2{d2} {}
Node::~Node()
{
    delete desc1;
    desc1 = 0;
    delete desc2;
    desc2 = 0;
}

Tree::Tree() :ntips{0}, nnodes{0}, nbranches{0}, max_id{0}, root_node{nullptr} {}

Tree::~Tree()
{
    delete root_node;
    root_node = 0;
}

int Tree::addTipRandomly() {
    if (ntips == 0) {
        root_node = new Node(++max_id, nullptr, nullptr, nullptr);
        ntips++;
        nnodes++;
        return root_node->get_id();
    } else if (ntips == 1){
        Node* first_tip = root_node;
        root_node = new Node(++max_id, nullptr, first_tip, nullptr);
        Node* new_tip = new Node(++max_id, nullptr, nullptr, nullptr);
        root_node->desc2 = new_tip;
        ntips++;
        nnodes += 2;
        nbranches += 2;
        return new_tip->get_id();
    } else {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(1, nbranches);
        int insertion_branch = distribution(generator);
        int count = insertNodeAtBranch(insertion_branch, 1, root_node, root_node->desc1);
        if (count < insertion_branch){
            insertNodeAtBranch(insertion_branch, count+1, root_node, root_node->desc2);
        }
        return max_id;
    }
}

int Tree::insertNodeAtBranch(int insert_number, int current_branch, Node* anc, Node* desc){
    if (insert_number == current_branch){
        Node *new_internal_node = new Node(++max_id, anc, nullptr, nullptr);
        Node *new_tip_node = new Node(++max_id, new_internal_node, nullptr, nullptr);
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
        ntips++;
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

int Tree::getRootID() {
    if (!root_node){ return 0; }
    return root_node->get_id();
}

void parseTree(std::vector<std::array<int, 2>> * branchList, Node * currentNode)
{
    if (currentNode->desc1 != nullptr) {
        std::array<int, 2> branch1 = {currentNode->get_id(), currentNode->desc1->get_id()};
        branchList->push_back(branch1);
        parseTree(branchList, currentNode->desc1);
    }
    if (currentNode->desc2 != nullptr) {
        std::array<int, 2> branch2 = {currentNode->get_id(), currentNode->desc2->get_id()};
        branchList->push_back(branch2);
        parseTree(branchList, currentNode->desc2);
    }
}

std::vector<std::array<int, 2>> * Tree::getBranchList()
{
    if (!root_node) return nullptr;
    std::vector<std::array<int, 2>> * branchList = new std::vector<std::array<int, 2>>;
    parseTree(branchList, root_node);
    return branchList;
}