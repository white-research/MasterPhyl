#include <array>
#include <exception>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_map>

#include "tree.h"


Node::Node(int id)
:id{id}
{
    anc = nullptr;
    desc1 = nullptr;
    desc2 = nullptr;
}


Node::Node(int id, std::shared_ptr<Node> a, std::shared_ptr<Node> d1, std::shared_ptr<Node> d2)
:id{id}
{
    anc = a;
    desc1 = d1;
    desc2 = d2;
}


bool Node::hasDescendents()
{
    if(desc1) return true;
    return false;
}


Tree::Tree()
:max_id{0}, ntips{0}, nnodes{0}, nbranches{0}
{
    root_node = nullptr;
}

Tree::Tree(std::unique_ptr<std::vector<std::array<int, 2>>>& branch_list, int root_id)
:max_id{0}, ntips{0}, nnodes{0}, nbranches{0}
{
    root_node = nullptr;
    // TODO: check proper construction: no circular branches & no floating, disassociated nodes (i.e. binary rooted digraph)

    // Add nodes, keeping a temporary Map to each.
    std::unordered_map<int, std::shared_ptr<Node>> node_map {};
    for (const auto& branch : *branch_list) {
        if (branch[1] == root_id)
            throw std::logic_error("Root node cannot have an ancestor");
        std::shared_ptr<Node> anc, desc;

        // Find or create branch's ancestor node
        auto anc_search = node_map.find(branch[0]);
        if (anc_search != node_map.end()) { // found
            anc = anc_search->second;
            if (!anc->hasDescendents())
                ntips--;
        }
        else {
            anc = std::make_shared<Node>(Node(branch[0]));
            node_map[branch[0]] = anc;
            nnodes++;
            if (max_id < branch[0])
                max_id = branch[0];
            if (branch[0] == root_id) {
                root_node = anc;
            }
        }

        // Find or create branch's descendent node
        auto desc_search = node_map.find(branch[1]);
        if (desc_search != node_map.end()) { // found
            desc = desc_search->second;
        }
        else {
            desc = std::make_shared<Node>(Node(branch[1]));
            node_map[branch[1]] = desc;
            nnodes++;
            ntips++;
            if (max_id < branch[1])
                max_id = branch[1];
        }

        // Create branch
        if (anc->desc1 == nullptr) {
            anc->desc1 = desc;
        }
        else if (anc->desc2 == nullptr) {
            anc->desc2 = desc;
        }
        else {
            throw std::logic_error("Cannot add more than 2 descendents to a node");
        }
        desc->anc = anc;
        nbranches++;
    }

    // Check root_node was created properly
    if (root_node == nullptr)
        throw std::logic_error("Root node was never added");

}


void deleteTree(const std::shared_ptr<Node>& n)
{
    if (n->desc1 != nullptr) {
        deleteTree(n->desc1);
        n->desc1 = nullptr;
    }
    if (n->desc2 != nullptr) {
        deleteTree(n->desc2);
        n->desc2 = nullptr;
    }
    n->anc = nullptr;
}


Tree::~Tree(){
    if (root_node != nullptr)
        deleteTree(root_node);
}


std::unique_ptr<Tree> Tree::createRandomTree(int ntaxa) {
    auto tree = std::make_unique<Tree>();
    for (int tip_count=0; tip_count < ntaxa; tip_count++){
        tree->addTipRandomly();
    }
    return tree;
}

std::shared_ptr<Node> Tree::getNode(int node_id, std::shared_ptr<Node> current_node) {
    if (current_node->get_id() == node_id){
        return current_node;
    }
    if (current_node->desc1 != nullptr) {
        auto node = getNode(node_id, current_node->desc1);
        if (node != nullptr) {
            return node;
        }
    }
    if (current_node->desc2 != nullptr) {
        auto node = getNode(node_id, current_node->desc2);
        if (node != nullptr) {
            return node;
        }
    }
    return nullptr;
}

int Tree::getRootID() {
    if (!root_node){ return 0; }
    return root_node->get_id();
}

bool Tree::hasNode(int node_id) {
    if (getNode(node_id, root_node) != nullptr) {
        return true;
    }
    return false;
}

int Tree::addTipRandomly() {
    if (ntips == 0) {
        root_node.reset(new Node(++max_id));
        ntips++;
        nnodes++;
        return root_node->get_id();
    } else if (ntips == 1){
        auto first_tip = root_node;
        root_node.reset(new Node(++max_id, nullptr, first_tip, nullptr));
        auto new_tip = std::make_shared<Node>(Node(++max_id));
        root_node->desc2 = new_tip;
        first_tip->anc = root_node;
        new_tip->anc = root_node;
        ntips++;
        nnodes += 2;
        nbranches += 2;
        return new_tip->get_id();
    } else {
        std::random_device dev;
        std::default_random_engine generator(dev());
        std::uniform_int_distribution<int> distribution(1, nbranches);
        int insertion_branch = distribution(generator);
        int count = insertNodeAtBranch(insertion_branch, 1, root_node, root_node->desc1);
        if (count < insertion_branch){
            insertNodeAtBranch(insertion_branch, count+1, root_node, root_node->desc2);
        }
        return max_id;
    }
}


int Tree::insertNodeAtBranch(int insert_number, int current_branch, std::shared_ptr<Node> anc, std::shared_ptr<Node> desc){
    if (insert_number == current_branch){
        auto new_internal_node = std::make_shared<Node>(Node(++max_id, anc, nullptr, nullptr));
        auto new_tip_node = std::make_shared<Node>(Node(++max_id, new_internal_node, nullptr, nullptr));
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
        if (desc->hasDescendents()) {
            int count = insertNodeAtBranch(insert_number, current_branch + 1, desc, desc->desc1);
            if (count < insert_number) {
                count = insertNodeAtBranch(insert_number, count + 1, desc, desc->desc2);
            }
            return count;
        }
        return current_branch;
    }
}


void parseTree(std::unique_ptr<std::vector<std::array<int, 2>>> & branchList, const std::shared_ptr<Node>& currentNode, int stop_id)
{
    if (currentNode->get_id() != stop_id) {
        if (currentNode->desc1 != nullptr) {
//            if (currentNode->desc1->get_id() != stop_id) {
                parseTree(branchList, currentNode->desc1, stop_id);
//            }
            std::array<int, 2> branch1 = {currentNode->get_id(), currentNode->desc1->get_id()};
            branchList->push_back(branch1);
        }
        if (currentNode->desc2 != nullptr) {
//            if (currentNode->desc2->get_id() != stop_id) {
                parseTree(branchList, currentNode->desc2, stop_id);
//            }
            std::array<int, 2> branch2 = {currentNode->get_id(), currentNode->desc2->get_id()};
            branchList->push_back(branch2);
        }
    }
}


std::unique_ptr<std::vector<std::array<int, 2>>> Tree::getBranchList(int start_id, int stop_id)
{
    std::unique_ptr<std::vector<std::array<int, 2>>> branchList = std::make_unique<std::vector<std::array<int, 2>>>();
    if (start_id == 0 && root_node != nullptr) {
        parseTree(branchList, root_node, stop_id);
    }
    else if (start_id > 0) {
        std::shared_ptr<Node> start_node = getNode(start_id, root_node);
        if (start_node != nullptr){
            parseTree(branchList, start_node, stop_id);
        }
    }
    return branchList;
}

bool checkTreeNode(std::shared_ptr<Node> node, int& node_count, int& tip_count){
    node_count++;
    std::cout << "Checking node " << node->get_id() << "\n";
    if (node->desc1 != nullptr) { // non-tip node
        std::cout << " Checking that desc1->anc exists for desc1: " << node->desc1->get_id() << "\n";
        if (node->desc1->anc == nullptr) {
            std::cout << "No ancestor!\n";
            return false;
        }
        std::cout << " Checking that anc->desc1 matches desc1->anc...\n";
        if (node->desc1->anc->get_id() != node->get_id()) {
            std::cout << "Invalid tree: desc1 and anc don't match on branch from node " << node->get_id() << "\n";
            return false;
        }
        std::cout << " Checking that there are two descendents or none...\n";
        if (node->desc2 == nullptr){
            std::cout << "Invalid tree: Node has desc1 but not desc2\n";
            return false;
        } else {
            if (node->desc2->anc != node) {
                std::cout << "Invalid tree: desc2 and anc don't match on branch\n";
                return false;
            }
        }
        checkTreeNode(node->desc1, node_count, tip_count);
        checkTreeNode(node->desc2, node_count, tip_count);
    } else { // tip node
        if (node->desc2 != nullptr) {
            std::cout << "Invalid tree: Node has desc2 but not desc1\n";
            return false;
        }
        tip_count++;
        return true;
    }
    return true;
}

bool Tree::checkValid() {
    int node_count = 0;
    int tip_count = 0;
    if (root_node->anc != nullptr) {
        std::cout << "Invalid tree: Root node doesn't have null ancestor pointer\n";
        return false;
    }
    bool status = checkTreeNode(root_node, node_count, tip_count);
    if (!status) {
        return status;
    }
    if (tip_count != getNTips()) {
        std::cout << "Invalid tree: Could not access all tips.\n";
        return false;
    }
    if (node_count != getNNodes()) {
        std::cout << "Invalid tree: Could not access all nodes.\n";
        return false;
    }
    return status;
}

void Tree::splitTree(int anc_id, int desc_id, std::unique_ptr<std::array<std::unique_ptr<Tree>, 2>>& subtrees) {
    std::shared_ptr<Node> anc_node = getNode(anc_id, root_node);
    if (anc_node->desc1->get_id() != desc_id && anc_node->desc2->get_id() != desc_id) {
        throw std::logic_error("Non-existent branch");
    }
    std::unique_ptr<std::vector<std::array<int, 2>>> st1_branches = getBranchList(0, desc_id);
    int new_anc, new_desc;
    for (long i = (*st1_branches).size() - 1; i >= 0; i--) {
        std::cout << "Deleting from branch (" << (*st1_branches)[i][0] << "," << (*st1_branches)[i][1] << ")\n";
        if ((*st1_branches)[i][0] == anc_id && (*st1_branches)[i][1] == desc_id) {
            std::cout << " ...Matched non-tip!\n";
            (*st1_branches).erase((*st1_branches).begin() + i);
        }
        else if ((*st1_branches)[i][1] == anc_id) {
            std::cout << " ...Matched branch to middle node!\n";
            new_anc = (*st1_branches)[i][0];
            (*st1_branches).erase((*st1_branches).begin() + i);
        }
        else if ((*st1_branches)[i][0] == anc_id) {
            std::cout << " ...Matched branch from middle node!\n";
            new_desc = (*st1_branches)[i][1];
            (*st1_branches).erase((*st1_branches).begin() + i);
        }
    }
    auto new_branch = std::array<int, 2>{new_anc, new_desc};
    (*st1_branches).push_back(new_branch);
    std::unique_ptr<std::vector<std::array<int, 2>>> st2_branches = getBranchList(desc_id);
    std::cout << "Making tree 1\n";
    std::cout << "Branches: ";
    for (auto b: *st1_branches){
        std::cout << "(" << b[0] << "," << b[1] << ") ";
    }
    std::cout << "\n";
    (*subtrees)[0] = std::make_unique<Tree>(st1_branches, root_node->get_id());
    std::cout << "Making tree 2\n";
    std::cout << "Branches: ";
    for (auto b: *st2_branches){
        std::cout << "(" << b[0] << "," << b[1] << ") ";
    }
    std::cout << "\n";
    (*subtrees)[1] = std::make_unique<Tree>(st2_branches, desc_id);
}