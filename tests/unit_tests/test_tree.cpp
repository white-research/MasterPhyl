//
// Created by dominic on 1/10/19.
//

#include "gtest/gtest.h"
#include "../../lib/tree/tree.h"


TEST(Example, Equals) {
    EXPECT_EQ(1, 1);
}

TEST(Example, NullNode_Test) {
    Node testNode(0, nullptr, nullptr, nullptr);
    EXPECT_EQ(testNode.anc, nullptr);
    EXPECT_EQ(testNode.desc1, nullptr);
    EXPECT_EQ(testNode.desc2, nullptr);
}

TEST(TreeTests, CreateEmptyTree) {
    Tree testTree = Tree();
    EXPECT_EQ(testTree.getNTips(), 0);
}

TEST(TreeTests, AddRootToEmptyTree) {
    Tree testTree = Tree();
    int tip_id = testTree.addTipRandomly();
    EXPECT_EQ(testTree.getNTips(), 1);
    EXPECT_EQ(tip_id, 1);
}

TEST(TreeTests, CreateTwoTipTree) {
    Tree testTree = Tree();
    int tip_id = testTree.addTipRandomly();
    EXPECT_EQ(tip_id, 1);
    tip_id = testTree.addTipRandomly();
    EXPECT_EQ(tip_id, 3);
    EXPECT_EQ(testTree.getNTips(), 2);
}

TEST(TreeTests, CreateThreeTipTree) {
    Tree testTree = Tree();
    int tip_id = testTree.addTipRandomly();
    EXPECT_EQ(tip_id, 1);
    tip_id = testTree.addTipRandomly();
    EXPECT_EQ(tip_id, 3);
    tip_id = testTree.addTipRandomly();
    EXPECT_EQ(tip_id, 5);
    EXPECT_EQ(testTree.getNTips(), 3);
}
