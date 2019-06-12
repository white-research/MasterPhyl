#ifndef MASTERPHYL_TREE_RF_H
#define MASTERPHYL_TREE_RF_H

#include "tree.h"

int robinsonFouldsDistance(Tree& t1, Tree& t2);

// TODO: internal functions -> don't export except for tests
std::map<int, int> relabelTree(Tree& tree);
void treeClusters(Tree& tree, Node& node, std::map<int,int>& labels, std::vector<std::vector<int>>& clusters);
void processClusters(Tree& tree, std::vector<std::vector<int>>& clusters, std::vector<std::array<int, 2>>& cluster_list);

#endif
