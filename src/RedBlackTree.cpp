#include "RedBlackTree.h"

void RedBlackTree::insert(int value) {
    shared_ptr<RBTreeNode> z = make_shared<RBTreeNode>(value);
    z->color = RED;

    shared_ptr<TreeNode> parent = nullptr;
    shared_ptr<TreeNode> current = root;

    while (current) {
        parent = current;
        if (z->value < current->value)
            current = current->left;
        else
            current = current->right;
    }

    z->parent = parent;

    if (!parent)
        root = z;
    else if (z->value < parent->value)
        parent->left = z;
    else
        parent->right = z;

    insert_fixup(z);
}

void RedBlackTree::remove(int value) {
    auto z = dynamic_pointer_cast<RBTreeNode>(find_node(value));
    if (!z) return;

    shared_ptr<RBTreeNode> y = z;
    Color y_original_color = y->color;

    shared_ptr<RBTreeNode> x;
    shared_ptr<RBTreeNode> x_parent;

    if (!z->left) {
        x = dynamic_pointer_cast<RBTreeNode>(static_pointer_cast<TreeNode>(z->right));
        x_parent = dynamic_pointer_cast<RBTreeNode>(z->parent);  // 修正
        x_parent = dynamic_pointer_cast<RBTreeNode>(z->parent);

        transplant(z, z->right);
    } else if (!z->right) {
        x = dynamic_pointer_cast<RBTreeNode>(static_pointer_cast<TreeNode>(z->left));
        x_parent = dynamic_pointer_cast<RBTreeNode>(z->parent);  // 修正
        x_parent = dynamic_pointer_cast<RBTreeNode>(z->parent);

        transplant(z, z->left);
    } else {
        y = dynamic_pointer_cast<RBTreeNode>(z->right);
        while (y->left)
            y = dynamic_pointer_cast<RBTreeNode>(y->left);

        y_original_color = y->color;
        x = dynamic_pointer_cast<RBTreeNode>(static_pointer_cast<TreeNode>(y->right));

        if (y->parent == z) {
            x_parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            if (y->right) y->right->parent = y;
            x_parent = dynamic_pointer_cast<RBTreeNode>(y->parent);

        }

        transplant(z, y);
        y->left = z->left;
        if (y->left) y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == BLACK) {
        remove_fixup(x, x_parent);
    }
}


void RedBlackTree::insert_fixup(shared_ptr<RBTreeNode> z) {
    while (z->parent && dynamic_pointer_cast<RBTreeNode>(z->parent)->color == RED) {
        auto parent = dynamic_pointer_cast<RBTreeNode>(z->parent);
        auto gp = dynamic_pointer_cast<RBTreeNode>(parent->parent);  // 祖父

        if (parent == gp->left) {
            auto y = dynamic_pointer_cast<RBTreeNode>(gp->right); // 叔叔節點
            if (y && y->color == RED) {
                parent->color = BLACK;
                y->color = BLACK;
                gp->color = RED;
                z = gp;
            } else {
                if (z == parent->right) {
                    z = parent;
                    rotate_left(z);
                    parent = dynamic_pointer_cast<RBTreeNode>(z->parent);
                    gp = dynamic_pointer_cast<RBTreeNode>(parent->parent);
                }
                parent->color = BLACK;
                gp->color = RED;
                rotate_right(gp);
            }
        } else {
            auto y = dynamic_pointer_cast<RBTreeNode>(gp->left); // 對稱情況
            if (y && y->color == RED) {
                parent->color = BLACK;
                y->color = BLACK;
                gp->color = RED;
                z = gp;
            } else {
                if (z == parent->left) {
                    z = parent;
                    rotate_right(z);
                    parent = dynamic_pointer_cast<RBTreeNode>(z->parent);
                    gp = dynamic_pointer_cast<RBTreeNode>(parent->parent);
                }
                parent->color = BLACK;
                gp->color = RED;
                rotate_left(gp);
            }
        }
    }

    auto root_rb = dynamic_pointer_cast<RBTreeNode>(root);
    if (root_rb) root_rb->color = BLACK;
}


void RedBlackTree::remove_fixup(shared_ptr<RBTreeNode> x, shared_ptr<RBTreeNode> parent) {
    while ((x != root) && (!x || x->color == BLACK)) {
        if (x == parent->left) {
            auto w = dynamic_pointer_cast<RBTreeNode>(parent->right);
            if (!w) break;

            auto parent_rb = dynamic_pointer_cast<RBTreeNode>(parent);
            auto w_left = dynamic_pointer_cast<RBTreeNode>(w->left);
            auto w_right = dynamic_pointer_cast<RBTreeNode>(w->right);

            if (w->color == RED) {
                w->color = BLACK;
                parent_rb->color = RED;
                rotate_left(parent);
                w = dynamic_pointer_cast<RBTreeNode>(parent->right);
                if (!w) break;
                w_left = dynamic_pointer_cast<RBTreeNode>(w->left);
                w_right = dynamic_pointer_cast<RBTreeNode>(w->right);
            }

            if ((!w_left || w_left->color == BLACK) && (!w_right || w_right->color == BLACK)) {
                w->color = RED;
                x = parent_rb;
                parent = dynamic_pointer_cast<RBTreeNode>(x->parent);
            } else {
                if (!w_right || w_right->color == BLACK) {
                    if (w_left) w_left->color = BLACK;
                    w->color = RED;
                    rotate_right(w);
                    w = dynamic_pointer_cast<RBTreeNode>(parent->right);
                    w_left = dynamic_pointer_cast<RBTreeNode>(w->left);
                    w_right = dynamic_pointer_cast<RBTreeNode>(w->right);
                }
                w->color = parent_rb->color;
                parent_rb->color = BLACK;
                if (w_right) w_right->color = BLACK;
                rotate_left(parent);
                x = dynamic_pointer_cast<RBTreeNode>(root);
                break;
            }

        } else {
            // Symmetric case: x 是右子
            auto w = dynamic_pointer_cast<RBTreeNode>(parent->left);
            if (!w) break;

            auto parent_rb = dynamic_pointer_cast<RBTreeNode>(parent);
            auto w_left = dynamic_pointer_cast<RBTreeNode>(w->left);
            auto w_right = dynamic_pointer_cast<RBTreeNode>(w->right);

            if (w->color == RED) {
                w->color = BLACK;
                parent_rb->color = RED;
                rotate_right(parent);
                w = dynamic_pointer_cast<RBTreeNode>(parent->left);
                if (!w) break;
                w_left = dynamic_pointer_cast<RBTreeNode>(w->left);
                w_right = dynamic_pointer_cast<RBTreeNode>(w->right);
            }

            if ((!w_left || w_left->color == BLACK) && (!w_right || w_right->color == BLACK)) {
                w->color = RED;
                x = parent_rb;
                parent = dynamic_pointer_cast<RBTreeNode>(x->parent);
            } else {
                if (!w_left || w_left->color == BLACK) {
                    if (w_right) w_right->color = BLACK;
                    w->color = RED;
                    rotate_left(w);
                    w = dynamic_pointer_cast<RBTreeNode>(parent->left);
                    w_left = dynamic_pointer_cast<RBTreeNode>(w->left);
                    w_right = dynamic_pointer_cast<RBTreeNode>(w->right);
                }
                w->color = parent_rb->color;
                parent_rb->color = BLACK;
                if (w_left) w_left->color = BLACK;
                rotate_right(parent);
                x = dynamic_pointer_cast<RBTreeNode>(root);
                break;
            }
        }
    }

    if (x) x->color = BLACK;
}



#include <sstream>
#include <queue>
std::string RedBlackTree::serialize() {
    if (!root) return "";

    std::ostringstream out;
    std::queue<std::shared_ptr<TreeNode>> q;
    q.push(root);

    std::vector<std::string> tokens;

    while (!q.empty()) {
        auto node = q.front(); q.pop();
        if (node) {
            auto rb_node = std::dynamic_pointer_cast<RBTreeNode>(node);
            if (rb_node->color == RED)
                tokens.push_back("(" + std::to_string(rb_node->value) + ")");
            else
                tokens.push_back(std::to_string(rb_node->value));

            q.push(node->left);
            q.push(node->right);
        } else {
            tokens.push_back("#");
        }
    }

    // ✂️ 從尾巴把多餘的 # 移除
    while (!tokens.empty() && tokens.back() == "#") {
        tokens.pop_back();
    }

    std::ostringstream final;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i > 0) final << " ";
        final << tokens[i];
    }

    return final.str();
}

void RedBlackTree::deserialize(const string& code) {
    if (code.empty()) return;

    std::istringstream in(code);
    std::vector<string> nodes;
    string token;
    while (in >> token) nodes.push_back(token);
    if (nodes.empty() || nodes[0] == "#") return;

    auto create_node = [](const std::string& token) -> shared_ptr<RBTreeNode> {
        if (token == "#") return nullptr;
        bool is_red = token.front() == '(';
        int val = std::stoi(is_red ? token.substr(1, token.size() - 2) : token);
        auto node = make_shared<RBTreeNode>(val);
        node->color = is_red ? RED : BLACK;
        return node;
    };

    root = create_node(nodes[0]);
    std::queue<shared_ptr<RBTreeNode>> q;
    q.push(dynamic_pointer_cast<RBTreeNode>(root));

    int i = 1;
    while (!q.empty() && i < nodes.size()) {
        auto node = q.front(); q.pop();
        if (i < nodes.size()) {
            node->left = create_node(nodes[i++]);
            if (node->left) {
                node->left->parent = node;
                q.push(dynamic_pointer_cast<RBTreeNode>(node->left));
            }
        }
        if (i < nodes.size()) {
            node->right = create_node(nodes[i++]);
            if (node->right) {
                node->right->parent = node;
                q.push(dynamic_pointer_cast<RBTreeNode>(node->right));
            }
        }
    }
}
