//
// Created by dominic on 5/22/19.
//

#include "gtest/gtest.h"
#include "../../lib/tree/rf.h"

#include <map>

TEST(RobinsonFoulds, RelabelTreeTest) {
    std::unique_ptr<std::vector<std::array<int, 2>>> branches = std::make_unique<std::vector<std::array<int, 2>>>();
    branches->push_back(std::array<int, 2>{1,2});
    branches->push_back(std::array<int, 2>{1,3});
    branches->push_back(std::array<int, 2>{3,4});
    branches->push_back(std::array<int, 2>{3,5});
    branches->push_back(std::array<int, 2>{4,6});
    branches->push_back(std::array<int, 2>{4,7});
    branches->push_back(std::array<int, 2>{5,8});
    branches->push_back(std::array<int, 2>{5,9});
    Tree tree = Tree(branches, 1);
    std::map<int,int> labels = relabelTree(tree);
    EXPECT_EQ(labels.size(), 5);
    EXPECT_EQ(labels[2], 1);
    EXPECT_EQ(labels[6], 2);
    EXPECT_EQ(labels[7], 3);
    EXPECT_EQ(labels[8], 4);
    EXPECT_EQ(labels[9], 5);
}