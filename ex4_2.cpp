#include <iostream>
#include <stack>


template<typename T>
class AVLTree {
    struct Node {
        T key;
        int height;
        size_t nodes;   // Число нод в поддереве
        Node* left;
        Node* right;

        Node(T key) : key(key), height(1), nodes(1), left(nullptr), right(nullptr) {}
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
    AVLTree() = default;
    ~AVLTree() {
        delete root;
    }

    AVLTree(const AVLTree&) = delete;
    AVLTree(AVLTree&&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    AVLTree& operator=(AVLTree&&) = delete;

    void insert(T key, int& position) {
        root = sub_insert(root, key, position);
    }

    void remove(int pos) {
        root = sub_remove(root, pos);
    }

  private:
    Node* sub_remove(Node* p, int key);
      
    int balance_factor(Node* node) {
        return get_height(node->right) - get_height(node->left);
    }
      
    size_t get_nodes(Node* node) {
        return (!node) ? 0 : node->nodes;
    }

    int get_height(Node* node) {
        return (!node) ? 0 : node->height;
    }

    Node* sub_insert(Node* p, T key, int& position);
    void fix_height(Node* p);
    Node* balance(Node* p);
    void fix_nodes(Node* p);
    Node* rotate_right(Node* p);
    Node* rotate_left(Node* p);
    Node* find_min(Node* p);
    Node* remove_min(Node* p);

    Node* root = nullptr;
};

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::sub_insert(Node* p, T key, int& position) {
    if (!p) {
        return new Node(key);
    }

    ++p->nodes;

    if (key < p->key) {
        position += get_nodes(p->right) + 1;
        p->left = sub_insert(p->left, key, position);
    } else {
        p->right = sub_insert(p->right, key, position);
    }

    return balance(p);
}

template<typename T>
void AVLTree<T>::fix_height(Node* p) {
    p->height = std::max(get_height(p->left), get_height(p->right)) + 1;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::balance(Node* p) {
    if (!p) {
        return nullptr;
    }
    fix_height(p);

    if (balance_factor(p) == -2) {
        if (balance_factor(p->left) > 0) {
            p->left = rotate_left(p->left);
        }

        return rotate_right(p);
    } else if (balance_factor(p) == 2) {
        if (balance_factor(p->right) < 0) {
            p->right = rotate_right(p->right);
        }

        return rotate_left(p);
    }
    return p;
}

template<typename T>
void AVLTree<T>::fix_nodes(Node* p) {
    if (!p) {
        return;
    }

    p->nodes = get_nodes(p->left) + get_nodes(p->right) + 1;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotate_right(Node* p) {
    Node* new_node = p->left;

    p->left = new_node->right;
    new_node->right = p;

    fix_nodes(p);
    fix_nodes(new_node);

    fix_height(p);
    fix_height(new_node);

    return new_node;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::rotate_left(Node* p) {
    Node* new_node = p->right;

    p->right = new_node->left;
    new_node->left = p;

    fix_nodes(p);
    fix_nodes(new_node);

    fix_height(p);
    fix_height(new_node);

    return new_node;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::sub_remove(Node* p, int pos) {
    if (!p) {
        return nullptr;
    }

    if (pos >= p->nodes) {
        return p;
    }

    int cur_pos = 0;
    std::stack<AVLTree<T>::Node*> node_stack;

    size_t right_nodes = get_nodes(p->right);
    while (pos - cur_pos != right_nodes) {
        if (pos - cur_pos > right_nodes) {
            node_stack.push(p);
            p = p->left;
            cur_pos += right_nodes + 1;
        } else if (pos - cur_pos < right_nodes) {
            node_stack.push(p);
            p = p->right;
        }
        right_nodes = get_nodes(p->right);
    }

    Node* left = p->left;
    Node* right = p->right;
    T key = p->key;

    p->left = nullptr;
    p->right = nullptr;
    delete p;

    if (!right) {
        if (!left) {
            if (!node_stack.empty()) {
                p = node_stack.top();
                node_stack.pop();
                --p->nodes;
                if (p->key > key) {
                    p->left = nullptr;
                } else {
                    p->right = nullptr;
                }
            } else {
                return nullptr;
            }
        } else {
            p = left;
        }
    } else {
        Node* min = find_min(right);
        min->right = remove_min(right);
        min->left = left;
        fix_nodes(min);
        p = balance(min);
    }

    while (!node_stack.empty()) {
        Node* tmp = node_stack.top();
        --tmp->nodes;

        if (tmp->key > p->key) {
            tmp->left = p;
        } else {
            tmp->right = p;
        }

        p = balance(tmp);
        node_stack.pop();
    }

    return p;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::find_min(Node* p) {
    if (!p->left) {
        return p;
    }
    return find_min(p->left);
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::remove_min(Node* p) {
    if (!p->left) {
        return p->right;
    }
    p->left = remove_min(p->left);
    --p->nodes;
    return balance(p);
}



int main() {
    size_t N = 0;

    std::cin >> N;

    AVLTree<int> avl_tree;

    for (size_t i = 0; i < N; i++) {
        int command = 0, key = 0, position = 0;
        std::cin >> command >> key;

        if (command == 1) {
            avl_tree.insert(key, position);
            std::cout << position << std::endl;
        } else if (command == 2) {
            avl_tree.remove(key);
        }
    }

    return 0;
}
