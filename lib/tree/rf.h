#ifndef MASTERPHYL_TREE_RF_H
#define MASTERPHYL_TREE_RF_H

#include "tree.h"

int robinsonFouldsDistance(Tree& t1, Tree& t2);

// TODO: internal functions -> don't export except for tests
std::map<int, int> relabelTree(Tree& tree);

#endif
