#include <iostream>

template<typename T>
class AVLTree {
  public:
    struct Node {
        T key;
        unsigned char height = 1;
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

    AVLTree() = default;
    ~AVLTree() {
        delete root;
    }
    AVLTree(const AVLTree&) = delete;
    AVLTree(AVLTree&&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    AVLTree& operator=(AVLTree&&) = delete;

  private:
    Node* root = nullptr;
};


int main() {
    return 0;
}
