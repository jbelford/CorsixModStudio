#include <gtest/gtest.h>
#include "rainman/lua/CInheritTable.h"
#include <cstring>

class InheritTableTest : public ::testing::Test {
protected:
    CInheritTable table;
};

TEST_F(InheritTableTest, DefaultConstructorCreatesRoot) {
    CInheritTable::CNode* root = table.getRoot();
    ASSERT_NE(root, nullptr);
    EXPECT_STREQ(root->getFullName(), "");
    EXPECT_EQ(root->getParent(), nullptr);
    EXPECT_EQ(root->getChildCount(), 0u);
}

TEST_F(InheritTableTest, FindOrMakeCreatesNewNode) {
    CInheritTable::CNode* node = table.findOrMake("data\\attrib\\weapon.lua");
    ASSERT_NE(node, nullptr);
    EXPECT_STREQ(node->getFullName(), "data\\attrib\\weapon.lua");
    EXPECT_STREQ(node->getMiniName(), "weapon");
}

TEST_F(InheritTableTest, FindOrMakeIdempotent) {
    CInheritTable::CNode* node1 = table.findOrMake("data\\unit.lua");
    CInheritTable::CNode* node2 = table.findOrMake("data\\unit.lua");
    EXPECT_EQ(node1, node2);
}

TEST_F(InheritTableTest, FindOrMakeCaseInsensitive) {
    CInheritTable::CNode* node1 = table.findOrMake("Data\\Unit.lua");
    CInheritTable::CNode* node2 = table.findOrMake("data\\unit.lua");
    EXPECT_EQ(node1, node2);
}

TEST_F(InheritTableTest, NilExtensionSetsFlag) {
    CInheritTable::CNode* node = table.findOrMake("data\\attrib\\removed.nil");
    ASSERT_NE(node, nullptr);
    EXPECT_TRUE(node->getIsNil());
    EXPECT_STREQ(node->getMiniName(), "removed");
}

TEST_F(InheritTableTest, NonNilExtensionClearsFlag) {
    CInheritTable::CNode* node = table.findOrMake("data\\attrib\\weapon.lua");
    EXPECT_FALSE(node->getIsNil());
}

TEST_F(InheritTableTest, SetParentUpdatesRelationship) {
    CInheritTable::CNode* parent = table.findOrMake("data\\base.lua");
    CInheritTable::CNode* child = table.findOrMake("data\\derived.lua");

    child->setParent(parent);

    EXPECT_EQ(child->getParent(), parent);
    EXPECT_EQ(parent->getChildCount(), 1u);
    EXPECT_EQ(parent->getChild(0), child);
}

TEST_F(InheritTableTest, SetParentRemovesFromOldParent) {
    CInheritTable::CNode* parent1 = table.findOrMake("data\\base1.lua");
    CInheritTable::CNode* parent2 = table.findOrMake("data\\base2.lua");
    CInheritTable::CNode* child = table.findOrMake("data\\child.lua");

    child->setParent(parent1);
    EXPECT_EQ(parent1->getChildCount(), 1u);

    child->setParent(parent2);
    EXPECT_EQ(parent1->getChildCount(), 0u);
    EXPECT_EQ(parent2->getChildCount(), 1u);
    EXPECT_EQ(child->getParent(), parent2);
}

TEST_F(InheritTableTest, AssignOrphansToAssignsParentlessNodes) {
    CInheritTable::CNode* orphan = table.findOrMake("data\\orphan.lua");
    EXPECT_EQ(orphan->getParent(), nullptr);

    CInheritTable::CNode* root = table.getRoot();
    table.assignOrphansTo(root);

    EXPECT_EQ(orphan->getParent(), root);
}

TEST_F(InheritTableTest, AssignOrphansDoesNotReassignParented) {
    CInheritTable::CNode* parent = table.findOrMake("data\\parent.lua");
    CInheritTable::CNode* child = table.findOrMake("data\\child.lua");
    child->setParent(parent);

    CInheritTable::CNode* root = table.getRoot();
    table.assignOrphansTo(root);

    // child should still have parent, not root
    EXPECT_EQ(child->getParent(), parent);
}

TEST_F(InheritTableTest, MiniNameFromPathWithoutExtension) {
    CInheritTable::CNode* node = table.findOrMake("data\\folder\\filename");
    EXPECT_STREQ(node->getMiniName(), "filename");
}

TEST_F(InheritTableTest, MultipleChildrenTracked) {
    CInheritTable::CNode* parent = table.findOrMake("data\\base.lua");
    CInheritTable::CNode* c1 = table.findOrMake("data\\child1.lua");
    CInheritTable::CNode* c2 = table.findOrMake("data\\child2.lua");
    CInheritTable::CNode* c3 = table.findOrMake("data\\child3.lua");

    c1->setParent(parent);
    c2->setParent(parent);
    c3->setParent(parent);

    EXPECT_EQ(parent->getChildCount(), 3u);
}
