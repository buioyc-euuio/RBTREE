#ifndef __REDBLACKTREE_H__
#define __REDBLACKTREE_H__

#include "BinarySearchTree.h"
enum Color { RED, BLACK };

struct RBTreeNode : TreeNode {
    Color color;

    RBTreeNode(int val) : TreeNode(val), color(RED) {}
};
class RedBlackTree : public BinarySearchTree {
public:
    RedBlackTree() : BinarySearchTree() {};
    void insert(int value) override;
    void remove(int value) override;
    std::string serialize() override;
    void deserialize(const std::string& code) override;

private:
    void insert_fixup(shared_ptr<RBTreeNode> z);
    void remove_fixup(shared_ptr<RBTreeNode> x, shared_ptr<RBTreeNode> parent);
};

#endif