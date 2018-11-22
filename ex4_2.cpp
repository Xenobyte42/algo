#include <iostream>
#include <type_traits>

template<typename T>
class AVLTree {
  public:
    struct Node {
        T key;
        unsigned char height;
        Node* left;
        Node* right;

        Node();

        ~Node() {
            delete left;
            delete right;
        }

        Node(const T& key);

        unsigned char height_left();
        
        unsigned char height_right();

        void update_height();

        int balance_factor();

        Node* rotate_right();

        Node* rotate_left();

        Node* balance();

    };

    AVLTree() = default;
    ~AVLTree() {
        delete root;
    }
    AVLTree(const AVLTree&) = delete;
    AVLTree(AVLTree&&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    AVLTree& operator=(AVLTree&&) = delete;

    size_t find_num(const T& key);

    bool insert(const T& key);

    void remove(const T& key);

  private:
    size_t find_num_impl(Node* nd, const T& key);
    
    Node* remove_impl(Node* nd, const T& key);
    
    Node* insert_impl(Node* nd, const T& key);
    
    Node* findmin(Node* nd);
    
    Node* removemin_impl(Node* nd);
    

    Node* root = nullptr;
};


template<typename T>
AVLTree<T>::Node::Node()
: height(0), left(nullptr), right(nullptr) {
    if (std::is_pod<T>::value) {
        key = T(0);
    }
}

template<typename T>
AVLTree<T>::Node::Node(const T& key)
: key(key), height(0), left(nullptr), right(nullptr) {
}

template<typename T>
unsigned char AVLTree<T>::Node::height_left() {
    if (!left) {
        return 0;
    }
    return left->height;
}

template<typename T>
unsigned char AVLTree<T>::Node::height_right() {
    if (!right) {
        return 0;
    }
    return right->height;
}

template<typename T>
void AVLTree<T>::Node::update_height() {
    height = std::max(height_left(), height_right()) + 1;
}

template<typename T>
int AVLTree<T>::Node::balance_factor() {
    return height_right() - height_left();
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::Node::rotate_right() {
    Node* new_root = left;
    left = new_root->right;
    new_root->right = this;
    update_height();
    new_root->update_height();
    return new_root;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::Node::rotate_left() {
    Node* new_root = right;
    right = new_root->left;
    new_root->left = this;
    update_height();
    new_root->update_height();
    return new_root;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::Node::balance() {
    update_height();
    if (balance_factor() == 2) {
        if (right->balance_factor() < 0) {
            right = right->rotate_right();
        }
        return rotate_left();
    }
    if (balance_factor() == -2) {
        if (left->balance_factor() > 0) {
            left = left->rotate_left();
        }
        return rotate_right();
    }
    return this;
}


template<typename T>
size_t AVLTree<T>::find_num(const T& key) {
    return find_num_impl(root, key);
}

template<typename T>
bool AVLTree<T>::insert(const T& key) {
    Node* tmp = insert_impl(root, key);
    if (!tmp) {
        return false;
    }
    root = tmp;
    return true;
}

template<typename T>
void AVLTree<T>::remove(const T& key) {
    remove_impl(root, key);
}

template<typename T>
size_t AVLTree<T>::find_num_impl(Node* nd, const T& key) {
    if (!nd) {
        return 0;
    }
    if (nd->key > key) {
         return find_num_impl(nd->left, key) + find_num_impl(nd->right, key) + 1;
    }
    if (nd->key <= key) {
        return find_num_impl(nd->right, key);
    }
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::remove_impl(Node* nd, const T& key) {
    if (!nd) {
        return 0;
    }

    if (key < nd->key) {
        nd->left = remove_impl(nd->left, key);
        return nd->balance();
    }
    if (key > nd->key) {
        nd->right = remove_impl(nd->right, key);  
        return nd->balance();
    }
    Node* left = nd->left;
    Node* right = nd->right;
    nd->left = nullptr;
    nd->right = nullptr;
    delete nd;
    if (!right) {
        return left;
    }
    Node* min = findmin(right);
    min->right = removemin_impl(right);
    min->left = left;
    return min->balance();
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::insert_impl(Node* nd, const T& key) {
    if (!nd) {
        return new Node(key);
    }
    if (key < nd->key) {
        Node* tmp = insert_impl(nd->left, key); 
        if (!tmp) {
            return nullptr;
        }
        nd->left = tmp; 
    } else if (key > nd->key) {
        Node* tmp = insert_impl(nd->right, key); 
        if (!tmp) {
            return nullptr;
        }
        nd->right = tmp;
        // std::cout << nd->key << " " << nd->right->key << std::endl;
    } else {
        return nullptr;
    }
    return nd->balance();
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::findmin(Node* nd) {
    if (!nd->left) {
        return nd;
    }
    return findmin(nd->left);
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::removemin_impl(Node* nd) {
    if (!nd->left) {
        return nd->right;
    }
    nd->left = removemin_impl(nd->left);
    return nd->balance();
}

int main() {
    int k = 0;
    std::cin >> k;
    AVLTree<int> avl;
    for (int i = 0; i < k; ++i) {
        int command = 0;
        int key = 0;
        std::cin >> command >> key;
        if (command == 1) {
            avl.insert(key);
            std::cout << avl.find_num(key) << std::endl;
        } else if (command == 2) {
            avl.remove(key);
        }
    }
}
