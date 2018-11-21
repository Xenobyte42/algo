#include <iostream>
#include <stack>
#include <cmath>


/************ Бинарное дерево **************/

template<typename Value>
class BinaryTree {
    struct Node {
        Value key;
        size_t height = 1;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;

        Node() = default;
        ~Node() {
            if (left) {
                delete left;
            }
            if (right) {
                delete right;
            }
        }
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
        Node* left = nullptr;
        Node* right = nullptr;

        Node() = default;
        ~Node() {
            if (left) {
                delete left;
            }
            if (right) {
                delete right;
            }
        }
    };

  public:
    CartesianTree() = default;
    ~CartesianTree();
    CartesianTree(const CartesianTree&) = delete;
    CartesianTree(CartesianTree&&) = delete;
    CartesianTree& operator=(const CartesianTree&) = delete;
    CartesianTree& operator=(CartesianTree&&) = delete;
    
    void insert(KType key, PType priority);

    size_t get_height() const {
        if (!root) {
            return 0;
        }
        if (!root->right && !root->left) {
            return 1;
        }
        return node_height(root);
    }
    
  private:
    Node* create_node(KType key, PType prior) {
        Node* new_node = new Node;
        new_node->key = key;
        new_node->priority = prior;
        return new_node;
    }

    size_t node_height(Node* node) const {
        if(node == 0) {
            return 0;
        }

        size_t left = 0, right = 0;
        if (node->left) {
            left = node_height(node->left);
        }
        if (node->right) {
            right = node_height(node->right);
        }
        return std::max(left, right) + 1;
    }

    std::pair<Node*, Node*> split(Node* current, KType key);
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
    delete root;
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
CartesianTree<KType, PType>::~CartesianTree() {
    delete root;
}


template<typename KType, typename PType>
typename CartesianTree<KType, PType>::Node* CartesianTree<KType, PType>::merge(Node* first, Node* second) {
    if (first == nullptr) {
        return second;
    }
    if (second == nullptr) {
        return first;
    }
    if (first->priority <= second->priority) {
        first->right = merge(second->right, second);
        return first;
    } else {
        second->left = merge(first, second->left);
        return second;
    }
}

template<typename KType, typename PType>
std::pair<
    typename CartesianTree<KType, PType>::Node*, 
    typename CartesianTree<KType, PType>::Node*
>
CartesianTree<KType, PType>::split(Node* current, KType key) {
    if (!current) {
        return {nullptr, nullptr};
    }
    if (current->key <= key) {
        auto res = split(current->right, key);
        current->right = res.first;
        return {current, res.second};
    } else {
        auto res = split(current->left, key);
        current->left = res.second;
        return {res.first, current};
    }
}

template<typename KType, typename PType>
void CartesianTree<KType, PType>::insert(const KType key, const PType priority) {
    Node* parent = nullptr;
    Node* node = root;
    while (node && node->priority >= priority) {
        parent = node;
        node = (node->key < key) ? node->right: node->left;
    }

    auto res = split(node, key);
    Node* new_node = create_node(key, priority);
    new_node->left = res.first;
    new_node->right = res.second;

    if (!parent) {
        root = new_node;
    } else {
        if (parent->key >= new_node->key) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
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
