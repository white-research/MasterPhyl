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


TEST(RobinsonFoulds, ClusterCreationTest) {
    std::unique_ptr<std::vector<std::array<int, 2>>> branches = std::make_unique<std::vector<std::array<int, 2>>>();
    branches->push_back(std::array<int, 2>{1,2}); // tip label 1
    branches->push_back(std::array<int, 2>{1,3});
    branches->push_back(std::array<int, 2>{3,4});
    branches->push_back(std::array<int, 2>{3,5});
    branches->push_back(std::array<int, 2>{4,6}); // tip label 2
    branches->push_back(std::array<int, 2>{4,7}); // tip label 3
    branches->push_back(std::array<int, 2>{5,8});
    branches->push_back(std::array<int, 2>{5,9}); // tip label 6
    branches->push_back(std::array<int, 2>{8,10}); // tip label 4
    branches->push_back(std::array<int, 2>{8,11}); // tip label 5
    Tree tree = Tree(branches, 1);
    std::map<int,int> labels = relabelTree(tree);
    std::vector<std::vector<int>> clusters(tree.getNNodes(), std::vector<int>(tree.getNTips()));
    treeClusters(tree, *(tree.getRootNode()), labels, clusters);
    // Check that node 11 (aka tip label 5) is clustered correctly
    EXPECT_EQ(clusters[10][0], 0);
    EXPECT_EQ(clusters[10][1], 0);
    EXPECT_EQ(clusters[10][2], 0);
    EXPECT_EQ(clusters[10][3], 0);
    EXPECT_EQ(clusters[10][4], 1);
    EXPECT_EQ(clusters[10][5], 0);
    // Check that all tips are in root node cluster
    for (int i = 0; i < tree.getNTips(); i++) {
        EXPECT_EQ(clusters[0][i], 1);
    }
}

