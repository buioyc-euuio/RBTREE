#ifndef __BINARYSEARCHTREE_H__
#define __BINARYSEARCHTREE_H__

#include <memory>
#include <string>

using namespace std;

struct TreeNode {
    int value;
    shared_ptr<TreeNode> parent;
    shared_ptr<TreeNode> left;
    shared_ptr<TreeNode> right;
    TreeNode(int v) : TreeNode(v, nullptr) {}
    TreeNode(int v, shared_ptr<TreeNode> p) : value(v), left(nullptr), right(nullptr), parent(p) {}
    virtual ~TreeNode() = default;
    virtual bool operator<(const TreeNode& other) const;
};

class BinarySearchTree {
public:
    BinarySearchTree() : root(nullptr) {};
    virtual ~BinarySearchTree();
    
    virtual void insert(int value);
    virtual void remove(int value);
    
    virtual std::string serialize();
    virtual void deserialize(const std::string& code);

    shared_ptr<TreeNode> find_node(int value);
    void insert(shared_ptr<TreeNode> node);
    
    void rotate_left(shared_ptr<TreeNode> x);
    void rotate_right(shared_ptr<TreeNode> x);

protected:
    void transplant(shared_ptr<TreeNode> u, shared_ptr<TreeNode> v);

    shared_ptr<TreeNode> root;
};


#endif