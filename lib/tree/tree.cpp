#include <algorithm>
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
    if (branch_list->size() == 1) { // TODO: Shouldn't return if no branches... delete this after refactoring splitTree
        root_node = std::make_shared<Node>((*branch_list)[0][0]);
        max_id = (*branch_list)[0][0];
        nnodes++;
        ntips++;
    }
    else {
        for (const auto &branch : *branch_list) {
            if (branch[1] == root_id)
                throw std::logic_error("Root node cannot have an ancestor");
            std::shared_ptr<Node> anc, desc;

            // Find or create branch's ancestor node
            auto anc_search = node_map.find(branch[0]);
            if (anc_search != node_map.end()) { // found
                anc = anc_search->second;
                if (!anc->hasDescendents())
                    ntips--;
            } else {
                anc = std::make_shared<Node>(branch[0]);
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
            } else {
                desc = std::make_shared<Node>(branch[1]);
                node_map[branch[1]] = desc;
                nnodes++;
                ntips++;
                if (max_id < branch[1])
                    max_id = branch[1];
            }

            // Create branch
            if (anc->desc1 == nullptr) {
                anc->desc1 = desc;
            } else if (anc->desc2 == nullptr) {
                anc->desc2 = desc;
            } else {
                throw std::logic_error("Cannot add more than 2 descendents to a node");
            }
            desc->anc = anc;
            nbranches++;
        }
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


//void Tree::addTipNextTo(int id, int sibling_id) {
//    /* Check that parameters are valid */
//    if (id > 0) {
//        if (id <= max_id) {
//            if (getNode(id, root_node) != nullptr) {
//                throw std::logic_error("Cannot add node with duplicate ID");
//            }
//        }
//    }
//    else {
//        id = ++max_id;
//    }
//    auto sibling = getNode(sibling_id, root_node);
//    if (sibling != nullptr) {
//        throw std::logic_error("Cannot find sibling node");
//    }
//
//    addTip(id, sibling, nullptr);
//}
//
void Tree::addTipFrom(int id, int anc_id) {
    /* Check that parameters are valid */
    if (id > 0) {
        if (id <= max_id) {
            if (getNode(id, root_node) != nullptr) {
                throw std::logic_error("Cannot add node with duplicate ID");
            }
        }
    }
    else {
        id = ++max_id;
    }
    /* anc_id of 0 indicates insertion of a root node into an empty tree */
    if (anc_id == 0) {
        if (getNNodes() != 0) {
            throw std::logic_error("Cannot add root to non-empty tree");
        }
        root_node = std::make_shared<Node>(id);
    }
    else {
//        auto ancestor = getNode(anc_id, )
    }
}
//
//void Tree::addTip(int id, std::shared_ptr<Node> sibling, std::shared_ptr<Node> ancestor) {
//
//    // If insertion position in tree is unspecified
//    if (sibling == nullptr && ancestor == nullptr){
//        // if empty tree, insert root
//        // otherwise throw exception
//    }
//    else if (ancestor == nullptr) {
//        // Add new inner node, and
//    }
//    else if (sibling == nullptr) { // Adding as sibling of another tip node
//        // add as another descendent of the ancestor
//    }
//
//}

int Tree::addTipRandomly() {
    if (ntips == 0) {
        root_node.reset(new Node(++max_id));
        ntips++;
        nnodes++;
        return root_node->get_id();
    } else if (ntips == 1){
        auto first_tip = root_node;
        root_node.reset(new Node(++max_id, nullptr, first_tip, nullptr));
        auto new_tip = std::make_shared<Node>(++max_id);
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
        auto new_internal_node = std::make_shared<Node>(++max_id, anc, nullptr, nullptr);
        auto new_tip_node = std::make_shared<Node>(++max_id, new_internal_node, nullptr, nullptr);
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
    // TODO: refactor to avoid start and stop nodes
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

bool checkTreeNode(std::shared_ptr<Node> node, int& node_count, int& tip_count, bool verbose){
    // TODO: check for circular loops
    node_count++;
    if (verbose == true) std::cout << "Checking node " << node->get_id() << "\n";
    if (node->desc1 != nullptr) { // non-tip node
        if (verbose == true) std::cout << " Checking that desc1->anc exists for desc1: " << node->desc1->get_id() << "\n";
        if (node->desc1->anc == nullptr) {
            std::cout << "Checking that desc1->anc exists for desc1: " << node->desc1->get_id() << "No ancestor!\n";
            return false;
        }
        if (verbose == true) std::cout << " Checking that anc->desc1 matches desc1->anc...\n";
        if (node->desc1->anc->get_id() != node->get_id()) {
            std::cout << "Invalid tree: desc1 and anc don't match on branch from node " << node->get_id() << "\n";
            return false;
        }
        if (verbose == true) std::cout << " Checking that there are two descendents or none...\n";
        if (node->desc2 == nullptr){
            std::cout << "Invalid tree: Node has desc1 but not desc2\n";
            return false;
        } else {
            if (node->desc2->anc != node) {
                std::cout << "Invalid tree: desc2 and anc don't match on branch\n";
                return false;
            }
        }
        checkTreeNode(node->desc1, node_count, tip_count, verbose);
        checkTreeNode(node->desc2, node_count, tip_count, verbose);
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

bool Tree::checkValid(bool verbose) {
    int node_count = 0;
    int tip_count = 0;
    if (root_node->anc != nullptr) {
        std::cout << "Invalid tree: Root node doesn't have null ancestor pointer\n";
        return false;
    }
    bool status = checkTreeNode(root_node, node_count, tip_count, verbose);
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


void Tree::splitTree(int anc_id, int desc_id, std::array<std::unique_ptr<Tree>, 2>& subtrees) {
    /* Check that anc and desc form a branch in the tree */
    std::shared_ptr<Node> anc_node = getNode(anc_id, root_node);
    if (anc_node->desc1->get_id() != desc_id && anc_node->desc2->get_id() != desc_id) {
        throw std::logic_error("Non-existent branch");
    }

    /* Parse the first subtree  */
    subtrees[0] = std::make_unique<Tree>();
    subtrees[0]->addTipFrom(root_node->get_id(), 0); // TODO: easier just to add constructor that takes a root_node id as param?
    subtrees[0]->nnodes++;
    subtrees[0]->max_id = root_node->get_id();
    copySubtree(*subtrees[0], subtrees[0]->root_node, *root_node, anc_id, desc_id);
    /* Parse the second subtree  */
    subtrees[1] = std::make_unique<Tree>();
    subtrees[1]->addTipFrom(desc_id, 0); // TODO: easier just to add constructor that takes a root_node id as param?
    subtrees[1]->nnodes++;
    subtrees[1]->max_id = desc_id;
    Node& break_desc_node = anc_node->desc1->get_id() == desc_id ? *(anc_node->desc1) : *(anc_node->desc2);
    copySubtree(*subtrees[1], subtrees[1]->root_node, break_desc_node, anc_id, desc_id);
}

void Tree::copySubtree(Tree& subtree, std::shared_ptr<Node>& subtree_node, Node& original_node, int breaking_branch_anc, int breaking_branch_desc) {
    /* If this node == breaking_branch_anc */
    /* Then join the other descendent of this node with this node's ancestor */
    /* And skip to the other descendent */
    if (original_node.get_id() == breaking_branch_anc) {
        // Find node that is the sibling of the breaking branck
        Node& grand_descendent = *(original_node.desc1);
        if (original_node.desc1->get_id() == breaking_branch_desc) { // TODO: assert that desc2 == breaking_branch_anc instead
            grand_descendent = *(original_node.desc2);
        }
        auto breaking_branch_sibling = std::make_shared<Node>(grand_descendent.get_id());
        if (subtree_node->anc == nullptr) { // If the breaking branch ancestor is the root of the original tree,
            // then delete the node and set its other descendent as the root
            subtree.root_node = breaking_branch_sibling;
        }
        else { // If not the root, then "delete" the node by joining its ancestor with its other descendent
            breaking_branch_sibling->anc = subtree_node->anc;
            if (subtree_node->anc->desc1 == subtree_node) {
                subtree_node->anc->desc1 = breaking_branch_sibling;
            } else {
                subtree_node->anc->desc2 = breaking_branch_sibling;
            }
        }
        copySubtree(subtree, breaking_branch_sibling, grand_descendent, breaking_branch_anc, breaking_branch_desc);
    }
    /* Otherwise, add each descendent and recursively parse them. */
    /* TODO: Need to increment/decrement subtree values */
    else {
        if (subtree_node->get_id() > subtree.max_id) {
            subtree.max_id = subtree_node->get_id();
        }
        if (original_node.hasDescendents()) {
            auto desc1_copy = std::make_shared<Node>(original_node.desc1->get_id());
            subtree_node->desc1 = desc1_copy;
            desc1_copy->anc = subtree_node;
            copySubtree(subtree, desc1_copy, *(original_node.desc1), breaking_branch_anc, breaking_branch_desc);
            auto desc2_copy = std::make_shared<Node>(original_node.desc2->get_id());
            subtree_node->desc2 = desc2_copy;
            desc2_copy->anc = subtree_node;
            copySubtree(subtree, desc2_copy, *(original_node.desc2), breaking_branch_anc, breaking_branch_desc);
            subtree.nnodes += 2;
            subtree.nbranches += 2;
        }
        else {
            subtree.ntips++;
        }
    }
}

void Tree::copyJoinedSubtrees(std::shared_ptr<Node>& new_tree_node, Node& copied_node, Tree& branch_tree, int sister_id, int new_node_id){
    if (copied_node.hasDescendents() == false) { // branch is tip
        ntips++;
    }
    else if (copied_node.desc1->get_id() == sister_id) { // Attach branch subtree on branch to descendent 1
        // Insert new node
        new_tree_node->desc1 = std::make_shared<Node>(new_node_id);
        new_tree_node->desc1->anc = new_tree_node;
        // Attach branch tree and original desc1 as new node's descendents
        new_tree_node->desc1->desc1 = std::make_shared<Node>(branch_tree.root_node->get_id());
        new_tree_node->desc1->desc1->anc = new_tree_node->desc1;
        new_tree_node->desc1->desc2 = std::make_shared<Node>(copied_node.desc1->get_id());
        new_tree_node->desc1->desc2->anc = new_tree_node->desc1;
        // Copy desc2 as normal
        new_tree_node->desc2 = std::make_shared<Node>(copied_node.desc2->get_id());
        new_tree_node->desc2->anc = new_tree_node;
        nnodes += 4;
        nbranches += 4;
        // Process all three descendents
        copyJoinedSubtrees(new_tree_node->desc1->desc1, *(branch_tree.root_node), branch_tree, sister_id, new_node_id);
        copyJoinedSubtrees(new_tree_node->desc1->desc2, *(copied_node.desc1), branch_tree, sister_id, new_node_id);
        copyJoinedSubtrees(new_tree_node->desc2, *(copied_node.desc2), branch_tree, sister_id, new_node_id);
    }
    else if (copied_node.desc2->get_id() == sister_id) { // Attach branch subtree on branch to descendent 2
        // Copy desc1 as normal
        new_tree_node->desc1 = std::make_shared<Node>(copied_node.desc1->get_id());
        new_tree_node->desc1->anc = new_tree_node;
        // Insert new node on desc2 branch
        new_tree_node->desc2 = std::make_shared<Node>(new_node_id);
        new_tree_node->desc2->anc = new_tree_node;
        // Attach branch tree and original desc2 as new node's descendents
        new_tree_node->desc2->desc1 = std::make_shared<Node>(branch_tree.root_node->get_id());
        new_tree_node->desc2->desc1->anc = new_tree_node->desc2;
        new_tree_node->desc2->desc2 = std::make_shared<Node>(copied_node.desc2->get_id());
        new_tree_node->desc2->desc2->anc = new_tree_node->desc2;
        nnodes += 4;
        nbranches += 4;
        // Process all three descendents
        copyJoinedSubtrees(new_tree_node->desc1, *(copied_node.desc1), branch_tree, sister_id, new_node_id);
        copyJoinedSubtrees(new_tree_node->desc2->desc1, *(branch_tree.root_node), branch_tree, sister_id, new_node_id);
        copyJoinedSubtrees(new_tree_node->desc2->desc2, *(copied_node.desc2), branch_tree, sister_id, new_node_id);
    }
    else { // Recursively process descendents
        new_tree_node->desc1 = std::make_shared<Node>(copied_node.desc1->get_id());
        new_tree_node->desc1->anc = new_tree_node;
        new_tree_node->desc2 = std::make_shared<Node>(copied_node.desc2->get_id());
        new_tree_node->desc2->anc = new_tree_node;
        nnodes += 2;
        nbranches += 2;
        copyJoinedSubtrees(new_tree_node->desc1, *(copied_node.desc1), branch_tree, sister_id, new_node_id);
        copyJoinedSubtrees(new_tree_node->desc2, *(copied_node.desc2), branch_tree, sister_id, new_node_id);
    }
}

Tree::Tree(Tree& base_tree, Tree& branch_tree, int sister_id, int new_node_id) {
    if (new_node_id == 0) {
        new_node_id = std::max(base_tree.max_id, branch_tree.max_id) + 1;
    }
    max_id = std::max(std::max(base_tree.max_id, branch_tree.max_id), new_node_id);
    // If joining subtrees as siblings of new root
    if (sister_id == base_tree.root_node->get_id()) {
        root_node = std::make_shared<Node>(new_node_id);
        root_node->desc1 = std::make_shared<Node>(branch_tree.root_node->get_id());
        root_node->desc1->anc = root_node;
        root_node->desc2 = std::make_shared<Node>(base_tree.root_node->get_id());
        root_node->desc2->anc = root_node;
        nnodes = 3;
        ntips = 0;
        nbranches = 2;
        copyJoinedSubtrees(root_node->desc1, *(branch_tree.root_node), branch_tree, sister_id, new_node_id);
        copyJoinedSubtrees(root_node->desc2, *(base_tree.root_node), branch_tree, sister_id, new_node_id);
    }
    else {
        root_node = std::make_shared<Node>(base_tree.root_node->get_id());
        nnodes = 1;
        ntips = 0;
        nbranches = 0;
        copyJoinedSubtrees(root_node, *base_tree.root_node, branch_tree, sister_id, new_node_id);
    }

}
