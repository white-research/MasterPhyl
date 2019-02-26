//
// Created by dominic on 1/10/19.
//

#include "gtest/gtest.h"
#include "../../lib/tree/tree.h"


TEST(Example, Equals) {
    EXPECT_EQ(1, 1);
}

TEST(NodeTests, EmptyNodeTest) {
    std::shared_ptr<Node> d1(nullptr);
    std::shared_ptr<Node> d2(nullptr);
    std::shared_ptr<Node> a(nullptr);
    Node testNode(1, a, d1, d2);
    EXPECT_EQ(testNode.anc, nullptr);
    EXPECT_EQ(testNode.desc1, nullptr);
    EXPECT_EQ(testNode.desc2, nullptr);
    EXPECT_EQ(testNode.get_id(), 1);
}

TEST(TreeTests, CreateEmptyTree) {
    Tree testTree = Tree();
    EXPECT_EQ(testTree.getNTips(), 0);
    EXPECT_EQ(testTree.getNBranches(), 0);
    EXPECT_EQ(testTree.getNNodes(), 0);
    EXPECT_EQ(testTree.getRootID(), 0);
}

TEST(TreeTests, AddRootToEmptyTree) {
    Tree testTree = Tree();
    int tip_id = testTree.addTipRandomly();
    EXPECT_EQ(testTree.getNTips(), 1);
    EXPECT_EQ(testTree.getNBranches(), 0);
    EXPECT_EQ(testTree.getNNodes(), 1);
    EXPECT_EQ(tip_id, 1);
    EXPECT_EQ(testTree.getRootID(), 1);
}

TEST(TreeTests, CreateTwoTipTree) {
    Tree testTree = Tree();
    testTree.addTipRandomly();
    int tip_id = testTree.addTipRandomly();
    EXPECT_EQ(tip_id, 3);
    EXPECT_EQ(testTree.getNTips(), 2);
    EXPECT_EQ(testTree.getNBranches(), 2);
    EXPECT_EQ(testTree.getNNodes(), 3);
    EXPECT_EQ(testTree.getRootID(), 2);
}

TEST(TreeTests, CreateThreeTipTree) {
    Tree testTree = Tree();
    testTree.addTipRandomly();
    testTree.addTipRandomly();
    int tip_id = testTree.addTipRandomly();
    EXPECT_EQ(tip_id, 5);
    EXPECT_EQ(testTree.getNTips(), 3);
    EXPECT_EQ(testTree.getNBranches(), 4);
    EXPECT_EQ(testTree.getNNodes(), 5);
    EXPECT_EQ(testTree.getRootID(), 2);
}

TEST(TreeTests, NullBranchListFromEmptyTree){
    Tree testTree = Tree();
    std::unique_ptr<std::vector<std::array<int, 2>>> bl = testTree.getBranchList();
    EXPECT_EQ(bl->size(), 0);
}

TEST(TreeTests, TwoBranchListFromTree){
    Tree testTree = Tree();
    testTree.addTipRandomly();
    testTree.addTipRandomly();
    std::unique_ptr<std::vector<std::array<int, 2>>> branchList = testTree.getBranchList();
    EXPECT_EQ(branchList->size(), 2);
    // Check that branches are correct:
    std::array<int, 2> branch1 = (*branchList)[0];
    EXPECT_EQ(branch1[0], 2);
    EXPECT_EQ(branch1[1], 1);
    std::array<int, 2> branch2 = (*branchList)[1];
    EXPECT_EQ(branch2[0], 2);
    EXPECT_EQ(branch2[1], 3);
}

TEST(TreeTests, EightBranchListFromTree){
    std::unique_ptr<std::vector<std::array<int, 2>>> branches = std::make_unique<std::vector<std::array<int, 2>>>();
    branches->push_back(std::array<int, 2>{1,2}); // 0
    branches->push_back(std::array<int, 2>{1,3}); // 7
    branches->push_back(std::array<int, 2>{3,4}); // 3
    branches->push_back(std::array<int, 2>{4,6}); // 1
    branches->push_back(std::array<int, 2>{4,7}); // 2
    branches->push_back(std::array<int, 2>{3,5}); // 6
    branches->push_back(std::array<int, 2>{5,8}); // 4
    branches->push_back(std::array<int, 2>{5,9}); // 5
    Tree tree = Tree(branches, 1);
    std::unique_ptr<std::vector<std::array<int, 2>>> branchList = tree.getBranchList();
    EXPECT_EQ(branchList->size(), 8);
    // Check that branches are correct:
    std::array<int, 2> branch1 = (*branchList)[0];
    EXPECT_EQ(branch1[0], 1);
    EXPECT_EQ(branch1[1], 2);
    std::array<int, 2> branch2 = (*branchList)[1];
    EXPECT_EQ(branch2[0], 4);
    EXPECT_EQ(branch2[1], 6);
    std::array<int, 2> branch3 = (*branchList)[5];
    EXPECT_EQ(branch3[0], 5);
    EXPECT_EQ(branch3[1], 9);
}

TEST(TreeTests, CreatBranchListUpToStopId){
    std::unique_ptr<std::vector<std::array<int, 2>>> branches = std::make_unique<std::vector<std::array<int, 2>>>();
    branches->push_back(std::array<int, 2>{1,2}); // 0
    branches->push_back(std::array<int, 2>{1,3}); // 5
    branches->push_back(std::array<int, 2>{3,4}); // 1
    branches->push_back(std::array<int, 2>{4,6}); // stop
    branches->push_back(std::array<int, 2>{4,7}); // stop
    branches->push_back(std::array<int, 2>{3,5}); // 4
    branches->push_back(std::array<int, 2>{5,8}); // 2
    branches->push_back(std::array<int, 2>{5,9}); // 3
    Tree tree = Tree(branches, 1);
    std::unique_ptr<std::vector<std::array<int, 2>>> branchList = tree.getBranchList(0, 4); // Skip all branches from node 4
    EXPECT_EQ(branchList->size(), 6);
    // Check that branches are correct:
    std::array<int, 2> branch1 = (*branchList)[0];
    EXPECT_EQ(branch1[0], 1);
    EXPECT_EQ(branch1[1], 2);
    std::array<int, 2> branch2 = (*branchList)[3];
    EXPECT_EQ(branch2[0], 5);
    EXPECT_EQ(branch2[1], 9);
    std::array<int, 2> branch3 = (*branchList)[5];
    EXPECT_EQ(branch3[0], 1);
    EXPECT_EQ(branch3[1], 3);
}

TEST(TreeTests, CreateRandomTree){
    auto tree = Tree::createRandomTree(10);
    EXPECT_EQ(tree->getNTips(), 10);
    EXPECT_EQ(tree->getNBranches(), 18);
}

TEST(TreeTests, CheckTwoTipTreeValidity){
    auto tree = Tree::createRandomTree(2);
    bool is_valid = tree->checkValid();
    EXPECT_EQ(is_valid, true);
}

TEST(TreeTests, CheckThreeTipTreeValidity){
    auto tree = Tree::createRandomTree(3);
    bool is_valid = tree->checkValid();
    EXPECT_EQ(is_valid, true);
}

TEST(TreeTests, CheckTenTipTreeValidity){
    auto tree = Tree::createRandomTree(10);
    bool is_valid = tree->checkValid();
    EXPECT_EQ(is_valid, true);
}

TEST(TreeTests, ExistingNodeCanBeFoundInTree){
    auto tree = Tree::createRandomTree(10);
    EXPECT_EQ(tree->hasNode(18), true);
    EXPECT_EQ(tree->hasNode(19), true);
}

TEST(TreeTests, NonexistantNodeCannotBeFoundInTree){
    auto tree = Tree::createRandomTree(10);
    EXPECT_EQ(tree->hasNode(20), false);
}

TEST(TreeTests, CreateTwoTipTreeFromTwoBranchList){
    std::unique_ptr<std::vector<std::array<int, 2>>> branches = std::make_unique<std::vector<std::array<int, 2>>>();
    branches->push_back(std::array<int, 2>{1,2});
    branches->push_back(std::array<int, 2>{1,3});
    Tree tree = Tree(branches, 1);
    EXPECT_EQ(tree.getNTips(), 2);
    EXPECT_EQ(tree.checkValid(), true);
}

TEST(TreeTests, CreateFiveTipTreeFromFiveBranchList){
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
    EXPECT_EQ(tree.getNTips(), 5);
    EXPECT_EQ(tree.getNNodes(), 9);
    EXPECT_EQ(tree.checkValid(), true);
}

TEST(TreeTests, FailTreeCreationFromInvalidBranchList){
    std::unique_ptr<std::vector<std::array<int, 2>>> branches = std::make_unique<std::vector<std::array<int, 2>>>();
    branches->push_back(std::array<int, 2>{1,2});
    branches->push_back(std::array<int, 2>{1,3});
    branches->push_back(std::array<int, 2>{3,4});
    branches->push_back(std::array<int, 2>{3,5});
    branches->push_back(std::array<int, 2>{3,6});
    bool exceptionRaised = false;
    try {
        Tree tree = Tree(branches, 1);
    }
    catch (std::logic_error) {
        exceptionRaised = true;
    }
    EXPECT_TRUE(exceptionRaised);
}

TEST(TreeTests, FailTreeCreationFromWrongRootNode){
    std::unique_ptr<std::vector<std::array<int, 2>>> branches = std::make_unique<std::vector<std::array<int, 2>>>();
    branches->push_back(std::array<int, 2>{1,2});
    branches->push_back(std::array<int, 2>{1,3});
    branches->push_back(std::array<int, 2>{3,4});
    branches->push_back(std::array<int, 2>{3,5});
    bool exceptionRaised = false;
    try {
        Tree tree = Tree(branches, 2);
    }
    catch (std::logic_error) {
        exceptionRaised = true;
    }
    EXPECT_TRUE(exceptionRaised);
}

TEST(TreeTests, TestCanSplitBasicTree){
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
    auto subtrees = std::array<std::unique_ptr<Tree>, 2>();
    tree.splitTree2(3, 4, subtrees);
    std::cout << "Checking tree 1, with " << subtrees[0]->getNTips() << " tips\n";
    EXPECT_TRUE(subtrees[0]->checkValid(true));
    std::cout << "Checking tree 2, with " << subtrees[1]->getNTips() << " tips\n";
    EXPECT_TRUE(subtrees[1]->checkValid());
    EXPECT_EQ(subtrees[0]->getNTips() + subtrees[1]->getNTips(), tree.getNTips());
}

TEST(TreeTests, TestCanSplitAlongTipBranch){
    std::unique_ptr<std::vector<std::array<int, 2>>> branches = std::make_unique<std::vector<std::array<int, 2>>>();
    branches->push_back(std::array<int, 2>{1,2});
    branches->push_back(std::array<int, 2>{1,3});
    branches->push_back(std::array<int, 2>{3,4});
    branches->push_back(std::array<int, 2>{3,5});
    Tree tree = Tree(branches, 1);
    auto subtrees = std::make_unique< std::array<std::unique_ptr<Tree>, 2> >();
    tree.splitTree(3, 4, subtrees);
    std::cout << "Checking tree 1, with " << (*subtrees)[0]->getNTips() << " tips\n";
    EXPECT_TRUE((*subtrees)[0]->checkValid(true));
    std::cout << "Checking tree 2, with " << (*subtrees)[1]->getNTips() << " tips\n";
    EXPECT_TRUE((*subtrees)[1]->checkValid());
    EXPECT_EQ((*subtrees)[0]->getNTips() + (*subtrees)[1]->getNTips(), tree.getNTips());
}

TEST(TreeTests, TestCanSplitTwoTipTree){
    std::unique_ptr<std::vector<std::array<int, 2>>> branches = std::make_unique<std::vector<std::array<int, 2>>>();
    branches->push_back(std::array<int, 2>{1,2});
    branches->push_back(std::array<int, 2>{1,3});
    Tree tree = Tree(branches, 1);
    auto subtrees = std::make_unique< std::array<std::unique_ptr<Tree>, 2> >();
    tree.splitTree(1, 2, subtrees);
    std::cout << "Checking tree 1, with " << (*subtrees)[0]->getNTips() << " tips\n";
    EXPECT_TRUE((*subtrees)[0]->checkValid(true));
    std::cout << "Checking tree 2, with " << (*subtrees)[1]->getNTips() << " tips\n";
    EXPECT_TRUE((*subtrees)[1]->checkValid());
    EXPECT_EQ((*subtrees)[0]->getNTips() + (*subtrees)[1]->getNTips(), tree.getNTips());
}