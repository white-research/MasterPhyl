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

TEST(RobinsonFoulds, ClusterProcessTest) {
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
    std::vector<std::vector<int>> clusters(tree.getNNodes(), std::vector<int>(tree.getNTips()));
    treeClusters(tree, *(tree.getRootNode()), labels, clusters);
    std::vector<std::array<int, 2>> cluster_list(tree.getNNodes()-tree.getNTips(), std::array<int,2>({0,0}));
    int cluster_count = processClusters(tree, clusters, cluster_list);
    EXPECT_EQ(cluster_list.size(), 4);
    EXPECT_EQ(cluster_count, 4);
    EXPECT_EQ(cluster_list[0][0], 1);
    EXPECT_EQ(cluster_list[0][1], 5);
    EXPECT_EQ(cluster_list[3][0], 4);
    EXPECT_EQ(cluster_list[3][1], 5);
}


TEST(RobinsonFoulds, RFDistIdenticalTreesTest) {
    std::unique_ptr<std::vector<std::array<int, 2>>> branches = std::make_unique<std::vector<std::array<int, 2>>>();
    branches->push_back(std::array<int, 2>{1,2});
    branches->push_back(std::array<int, 2>{1,3});
    branches->push_back(std::array<int, 2>{3,4});
    branches->push_back(std::array<int, 2>{3,5});
    branches->push_back(std::array<int, 2>{4,6});
    branches->push_back(std::array<int, 2>{4,7});
    branches->push_back(std::array<int, 2>{5,8});
    branches->push_back(std::array<int, 2>{5,9});
    Tree tree1 = Tree(branches, 1);
    Tree tree2 = Tree(branches, 1);
    int RF_dist = robinsonFouldsDistance(tree1, tree2);
    EXPECT_EQ(RF_dist, 0);
}

TEST(RobinsonFoulds, RFDistSameClustersDifferentTreesTest) {
    // tips: 2, 6, 7, 8, 9
    std::unique_ptr<std::vector<std::array<int, 2>>> branches1 = std::make_unique<std::vector<std::array<int, 2>>>();
    branches1->push_back(std::array<int, 2>{1,2});
    branches1->push_back(std::array<int, 2>{1,3});
    branches1->push_back(std::array<int, 2>{3,4});
    branches1->push_back(std::array<int, 2>{3,5});
    branches1->push_back(std::array<int, 2>{4,6});
    branches1->push_back(std::array<int, 2>{4,7});
    branches1->push_back(std::array<int, 2>{5,8});
    branches1->push_back(std::array<int, 2>{5,9});
    Tree tree1 = Tree(branches1, 1);
    std::unique_ptr<std::vector<std::array<int, 2>>> branches2 = std::make_unique<std::vector<std::array<int, 2>>>();
    branches2->push_back(std::array<int, 2>{1,5});
    branches2->push_back(std::array<int, 2>{1,3});
    branches2->push_back(std::array<int, 2>{3,4});
    branches2->push_back(std::array<int, 2>{3,9});
    branches2->push_back(std::array<int, 2>{4,6});
    branches2->push_back(std::array<int, 2>{4,7});
    branches2->push_back(std::array<int, 2>{5,8});
    branches2->push_back(std::array<int, 2>{5,2});
    Tree tree2 = Tree(branches2, 1);
    int RF_dist = robinsonFouldsDistance(tree1, tree2);
    EXPECT_EQ(RF_dist, 2);
}

TEST(RobinsonFoulds, RFDistDifferentClustersAndTreesTest) {
    // tips: 2, 6, 7, 8, 9
    // t1: 1->(all); 3->(6,7,8,9); 4->(6,7); 5->(8,9)
    // t2: 1->(all); 5->(8,9); 3->(2,6,7); 4->(6,7)
    std::unique_ptr<std::vector<std::array<int, 2>>> branches1 = std::make_unique<std::vector<std::array<int, 2>>>();
    branches1->push_back(std::array<int, 2>{1,2});
    branches1->push_back(std::array<int, 2>{1,3});
    branches1->push_back(std::array<int, 2>{3,4});
    branches1->push_back(std::array<int, 2>{3,5});
    branches1->push_back(std::array<int, 2>{4,6});
    branches1->push_back(std::array<int, 2>{4,7});
    branches1->push_back(std::array<int, 2>{5,8});
    branches1->push_back(std::array<int, 2>{5,9});
    Tree tree1 = Tree(branches1, 1);
    std::unique_ptr<std::vector<std::array<int, 2>>> branches2 = std::make_unique<std::vector<std::array<int, 2>>>();
    branches2->push_back(std::array<int, 2>{1,5});
    branches2->push_back(std::array<int, 2>{1,3});
    branches2->push_back(std::array<int, 2>{3,4});
    branches2->push_back(std::array<int, 2>{3,2});
    branches2->push_back(std::array<int, 2>{4,6});
    branches2->push_back(std::array<int, 2>{4,7});
    branches2->push_back(std::array<int, 2>{5,8});
    branches2->push_back(std::array<int, 2>{5,9});
    Tree tree2 = Tree(branches2, 1);
    int RF_dist = robinsonFouldsDistance(tree1, tree2);
    EXPECT_EQ(RF_dist, 1);
}

TEST(RobinsonFoulds, RFDistDifferentClustersAndTreesVersion2Test) {
    // tips: 2, 6, 7, 8, 9
    // t1: 1->(all); 3->(6,7,8,9); 4->(6,7); 5->(8,9)
    // t2: 1->(all); 3->(2,6,7,8); 4->(2,7); 5->(6,8)
    // expected RF = 3
    std::unique_ptr<std::vector<std::array<int, 2>>> branches1 = std::make_unique<std::vector<std::array<int, 2>>>();
    branches1->push_back(std::array<int, 2>{1,2});
    branches1->push_back(std::array<int, 2>{1,3});
    branches1->push_back(std::array<int, 2>{3,4});
    branches1->push_back(std::array<int, 2>{3,5});
    branches1->push_back(std::array<int, 2>{4,6});
    branches1->push_back(std::array<int, 2>{4,7});
    branches1->push_back(std::array<int, 2>{5,8});
    branches1->push_back(std::array<int, 2>{5,9});
    Tree tree1 = Tree(branches1, 1);
    std::unique_ptr<std::vector<std::array<int, 2>>> branches2 = std::make_unique<std::vector<std::array<int, 2>>>();
    branches2->push_back(std::array<int, 2>{1,9});
    branches2->push_back(std::array<int, 2>{1,3});
    branches2->push_back(std::array<int, 2>{3,4});
    branches2->push_back(std::array<int, 2>{3,5});
    branches2->push_back(std::array<int, 2>{4,2});
    branches2->push_back(std::array<int, 2>{4,7});
    branches2->push_back(std::array<int, 2>{5,8});
    branches2->push_back(std::array<int, 2>{5,6});
    Tree tree2 = Tree(branches2, 1);
    int RF_dist = robinsonFouldsDistance(tree1, tree2);
    EXPECT_EQ(RF_dist, 3);
}


