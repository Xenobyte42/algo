#include <iostream>
#include <stack>
#include <algorithm>


template<typename Value>
class BinaryTree {
    struct Node {
        Value key;
        size_t height = 1;
        Node* left = nullptr;
        Node* right = nullptr;
    };
    
  public:
    BinaryTree() = default;
    ~BinaryTree();

    BinaryTree(const BinaryTree&) = delete;
    BinaryTree(BinaryTree&&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;
    BinaryTree& operator=(BinaryTree&&) = delete;

    void insert(Value& val);
    void visit(Node* node);
    void print();
    
    size_t get_height() const {
        return root->height;
    }

  private:
    Node* create_new(Value& val) {
        Node* new_node = new Node;
        new_node->key = val;
        return new_node;
    }

    void update_height(Node* node);
      
    Node* root = nullptr;
};

/**************************************************/

template<typename Value>
void BinaryTree<Value>::update_height(Node* node) {
    if (!node->right || !node->left) {
        if (node->right) {
            node->height = node->right->height + 1;
        } else if (node->left) {
            node->height = node->left->height + 1;
        }
        return;
    }
    node->height = std::max(node->left->height, node->right->height) + 1;
    return;
}

template<typename Value>
BinaryTree<Value>::~BinaryTree() {
    if (root) {
        std::stack<Node*> nodes;
        nodes.push(root);
        while (!nodes.empty()) {
            Node* current = nodes.top();
            if (current->left) {
                current = current->left;
                nodes.push(current);
            } else if (current->right) {
                current = current->right;
                nodes.push(current);
            } else {
                nodes.pop();
                if (!nodes.empty()) {
                    Node* parent = nodes.top();
                    
                    if (parent->left == current) {
                        parent->left = nullptr;
                    } else {
                        parent->right = nullptr;
                    }   
                }
                delete current;
            }
        }
        root = nullptr;
    }
}

template<typename Value>
void BinaryTree<Value>::insert(Value& val) {
    if (!root) {
        root = create_new(val);
        return;
    }
    Node* node = root;

    std::stack<Node*> nodes;

    while (node) {
        if (val >= node->key) {
            nodes.push(node);
            node = node->right;
            if (!node) {
                Node* parent = nodes.top();
                nodes.pop();
                parent->right = create_new(val);
                update_height(parent);
                while (!nodes.empty()) {
                    parent = nodes.top();
                    nodes.pop();
                    update_height(parent);
                } 
                return;
            }
        } else {
            nodes.push(node);
            node = node->left;
            if (!node) {
                Node* parent = nodes.top();
                nodes.pop();
                parent->left = create_new(val);
                update_height(parent);
                while (!nodes.empty()) {
                    parent = nodes.top();
                    nodes.pop();
                    update_height(parent);
                } 
                return;
            }
        }
    }
    return;
}

template<typename Value>
void BinaryTree<Value>::visit(Node* node) {
    if (!node) {
        return;
    }

    visit(node->left);
        std::cout << node->key << " ";
    visit(node->right);
}

template<typename Value>
void BinaryTree<Value>::print() {
    Node* node = root;
    std::stack<Node*> nodes;
    while (!nodes.empty() || node) {
        if (node) {
            nodes.push(node);
            node = node->left;
        } else {
            node = nodes.top();
            nodes.pop();
            std::cout << node->key << " ";
            node = node->right;
        }
        
    }
    std::cout << std::endl;
}

/**************************************************/

int main() {
    BinaryTree<int> tree;

    size_t N = 0;
    std::cin >> N;

    for (size_t i = 0; i < N; i++) {
        int tmp = 0;
        std::cin >> tmp;
        tree.insert(tmp);
    }

    tree.print();

    return 0;
}
