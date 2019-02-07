//
// Created by dominic on 1/10/19.
//

#include "gtest/gtest.h"
#include "../../src/tree.h"


TEST(Example, Equals) {
    EXPECT_EQ(1, 1);
}

TEST(Example2, NullNode_Test) {
    Node testNode(0, nullptr, nullptr, nullptr);
    EXPECT_EQ(testNode.anc, nullptr);
    EXPECT_EQ(testNode.desc1, nullptr);
    EXPECT_EQ(testNode.desc2, nullptr);
}
