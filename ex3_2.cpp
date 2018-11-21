#include <iostream>
#include <stack>
#include <cmath>


//TODO(Xenobyte): realize get_height cTree

/************ Бинарное дерево **************/

template<typename Value>
class BinaryTree {
    struct Node {
        Value key;
        size_t height = 1;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
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

/************ Декартово дерево **************/

template<
    typename KType,
    typename PType
>
class CartesianTree {
    struct Node {
        KType key;
        PType priority;
        char height = 1;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
    };

  public:
    CartesianTree() = default;
    ~CartesianTree();
    
    void insert(KType key, PType priority);

    char get_height() const {
        return root->height;
    }
    
  private:
    Node* create_node(KType key, PType prior, Node* left = nullptr,
                      Node* right = nullptr, Node* parent = nullptr) {
        Node* new_node = new Node;
        new_node->key = key;
        new_node->priority = prior;
        new_node->left = left;
        new_node->right = right;
        new_node->parent = parent;
        return new_node;
    }

    void update_height(Node* node);
    void split(Node* current, KType key, Node*& left, Node*& right);
    Node* merge(Node* left, Node* right);

    Node* root = nullptr;
};

/*********************** Методы бинарного дерева ***************************/

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
        Node* current = root;
        while (current) {
            if (current->left) {
                current = current->left;
            } else if (current->right) {
                current = current->right;
            } else {
                Node* parent = current->parent;
                if (parent) {
                    if (parent->left == current) {
                        parent->left = nullptr;
                    } else {
                        parent->right = nullptr;
                    }
                }
                delete current;
                current = parent;
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
    Node* parent = nullptr;
    while (node) {
        parent = node;
        if (val >= node->key) {
            node = node->right;
            if (!node) {
                parent->right = create_new(val);
                parent->right->parent = parent;
                while (parent) {
                    update_height(parent);
                    parent = parent->parent;
                } 
                return;
            }
        } else {
            node = node->left;
            if (!node) {
                parent->left = create_new(val);
                parent->left->parent = parent;
                while (parent) {
                    update_height(parent);
                    parent = parent->parent;
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

/********************** Методы декартового дерева ****************************/

template<typename KType, typename PType>
void CartesianTree<KType, PType>::update_height(Node* node) {
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

template<typename KType, typename PType>
CartesianTree<KType, PType>::~CartesianTree() {
    if (root) {
        Node* current = root;
        while (current) {
            if (current->left) {
                current = current->left;
            } else if (current->right) {
                current = current->right;
            } else {
                Node* parent = current->parent;
                if (parent) {
                    if (parent->left == current) {
                        parent->left = nullptr;
                    } else {
                        parent->right = nullptr;
                    }
                }
                delete current;
                current = parent;
            }
        }
        root = nullptr;
    }
}

template<typename KType, typename PType>
void CartesianTree<KType, PType>::split(Node* current, KType key, Node*& left, Node*& right) {
    if (!current) {
        left = nullptr;
        right = nullptr;
    } else if (current->key <= key) {
        split(current->right, key, current->right, right);
        left = current;
    } else {
        split(current->left, key, left, current->left);
        right = current;
    }
}

template<typename KType, typename PType>
typename CartesianTree<KType, PType>::Node* CartesianTree<KType, PType>::merge(Node* left, Node* right) {
    if (!left || !right) {
        return (!left) ? right : left;
    }

    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        return left;
    }

    right->left = merge(left, right->left);
    return right;
}

template<typename KType, typename PType>
void CartesianTree<KType, PType>::insert(const KType key, const PType priority) {
    Node* parent = nullptr;
    Node* node = root;
    while (node != nullptr && node->priority >= priority) {
        parent = node;
        node = (node->key < key) ? node->right: node->left;
    }

    Node* left = nullptr;
    Node* right = nullptr;

    split(node, key, left, right);
    Node* newNode = create_node(key, priority, left, right, parent);

    if (!parent) {
        root = newNode;
    }
    if (parent) {
        if (parent->key > newNode->key) parent->left = newNode;
        else parent->right = newNode;
    }
}
/*********************** /main/ ***************************/


int main() {
    BinaryTree<int> b_tree;
    CartesianTree<int, int> c_tree;

    size_t N = 0;
    std::cin >> N;
    for (size_t i = 0; i < N; i++) {
        int key = 0;
        int priority = 0;
        std::cin >> key >> priority;
        b_tree.insert(key);
        c_tree.insert(key, priority);
    }
    int b_tree_height = static_cast<int>(b_tree.get_height());
    int c_tree_height = static_cast<int>(c_tree.get_height());
    
    std::cout << std::abs(c_tree_height - b_tree_height) << std::endl; 

    return 0;
}
