#include "BinarySearchTree.h"
#include <functional>
#include <stdexcept>


static void clear(shared_ptr<TreeNode> node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    node->left = nullptr;
    node->right = nullptr;
    node->parent = nullptr;
}


BinarySearchTree::~BinarySearchTree() {
    clear(root);
}



bool TreeNode::operator<(const TreeNode& other) const {
    return this->value < other.value;
}



shared_ptr<TreeNode> BinarySearchTree::find_node(int value) {
    shared_ptr<TreeNode> current = root;
    while (current) {
        if (value == current->value) return current;
        if (value < current->value) current = current->left;
        else current = current->right;
    }
    return nullptr;
}


void BinarySearchTree::insert(shared_ptr<TreeNode> node) {
    shared_ptr<TreeNode> parent = nullptr;
    shared_ptr<TreeNode> current = root;

    while (current) {
        parent = current;
        if (node->value < current->value)
            current = current->left;
        else
            current = current->right;
    }

    node->parent = parent;

    if (!parent)
        root = node;  // tree was empty
    else if (node->value < parent->value)
        parent->left = node;
    else
        parent->right = node;
}

void BinarySearchTree::insert(int value) {
    insert(make_shared<TreeNode>(value));
}


void BinarySearchTree::transplant(shared_ptr<TreeNode> u, shared_ptr<TreeNode> v) {
    if (!u->parent)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v)
        v->parent = u->parent;
}


void BinarySearchTree::remove(int value) {
     shared_ptr<TreeNode> z = find_node(value);
    if (!z) return;

    if (!z->left)
        transplant(z, z->right);
    else if (!z->right)
        transplant(z, z->left);
    else {
        // find successor
        shared_ptr<TreeNode> y = z->right;
        while (y->left)
            y = y->left;

        if (y->parent != z) {
            transplant(y, y->right);
            y->right = z->right;
            if (y->right) y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        if (y->left) y->left->parent = y;
    }
}


void BinarySearchTree::rotate_left(shared_ptr<TreeNode> x) {    
    
    if (!x || !x->right) return;

    shared_ptr<TreeNode> y = x->right;
    x->right = y->left;
    if (y->left)
        y->left->parent = x;

    y->parent = x->parent;

    if (!x->parent)
        root = y;
    else if (x.get() == x->parent->left.get())
        x->parent->left = y;
    else
        x->parent->right = y;


    y->left = x;
    x->parent = y;
}


void BinarySearchTree::rotate_right(shared_ptr<TreeNode> y) {
    if (!y || !y->left) return;

    shared_ptr<TreeNode> x = y->left;
    y->left = x->right;
    if (x->right)
        x->right->parent = y;

    x->parent = y->parent;

   if (!x->parent)
        root = y;
    else if (x.get() == x->parent->left.get())
        x->parent->left = y;
    else
        x->parent->right = y;

    x->right = y;
    y->parent = x;
}


#include <sstream>
#include <queue>


std::string BinarySearchTree::serialize() {
    std::ostringstream out;
    if (!root) return "";

    std::queue<shared_ptr<TreeNode>> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();
        bool allNull = true;

        for (int i = 0; i < size; ++i) {
            shared_ptr<TreeNode> node = q.front();
            q.pop();

            if (node) {
                out << node->value << " ";
                q.push(node->left);
                q.push(node->right);
                if (node->left || node->right)
                    allNull = false;
            } else {
                out << "# ";
                q.push(nullptr);
                q.push(nullptr);
            }
        }

        if (allNull) break;
    }

    std::string result = out.str();
    while (!result.empty() && (result.back() == ' ' || result.back() == '#')) {
        if (result.back() == ' ') result.pop_back();
        else if (result.size() >= 2 && result.substr(result.size() - 2) == "# ") result.erase(result.size() - 2);
        else break;
    }

    return result;
}

void BinarySearchTree::deserialize(const std::string& code) {
    if (code.empty()) return;

    std::istringstream in(code);
    std::vector<std::string> nodes;
    std::string val;
    while (in >> val) nodes.push_back(val);

    if (nodes.empty() || nodes[0] == "#") return;

    root = make_shared<TreeNode>(stoi(nodes[0]));
    std::queue<shared_ptr<TreeNode>> q;
    q.push(root);
    int i = 1;

    while (!q.empty() && i < nodes.size()) {
        auto node = q.front(); q.pop();

        if (i < nodes.size() && nodes[i] != "#") {
            node->left = make_shared<TreeNode>(stoi(nodes[i]));
            node->left->parent = node;
            q.push(node->left);
        }
        ++i;

        if (i < nodes.size() && nodes[i] != "#") {
            node->right = make_shared<TreeNode>(stoi(nodes[i]));
            node->right->parent = node;
            q.push(node->right);
        }
        ++i;
    }
}

