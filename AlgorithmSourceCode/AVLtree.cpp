#include <vector>
#include <algorithm>

struct Node {
    int key, height;
    Node *left, *right;
    Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    Node* root = nullptr;

    int height(Node* N) {
        return N ? N->height : 0;
    }

    int getBalance(Node* N) {
        return N ? height(N->left) - height(N->right) : 0;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        return y;
    }

    Node* insert(Node* node, int key) {
        if (!node) return new Node(key);
        
        if (key < node->key) 
            node->left = insert(node->left, key);
        else if (key >= node->key) 
            node->right = insert(node->right, key);

        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && getBalance(node->left) >= 0) return rightRotate(node);
        if (balance < -1 && getBalance(node->right) <= 0) return leftRotate(node);
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    Node* minValueNode(Node* node) {
        Node* curr = node;
        while (curr->left) curr = curr->left;
        return curr;
    }

    Node* deleteNode(Node* root, int key) {
        if (!root) return root;

        if (key < root->key) 
            root->left = deleteNode(root->left, key);
        else if (key > root->key) 
            root->right = deleteNode(root->right, key);
        else {
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;
                if (!temp) { temp = root; root = nullptr; }
                else *root = *temp;
                delete temp;
            } else {
                Node* temp = minValueNode(root->right);
                root->key = temp->key;
                root->right = deleteNode(root->right, temp->key);
            }
        }
        if (!root) return root;

        root->height = 1 + std::max(height(root->left), height(root->right));
        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
        if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }

    Node* search(Node* root, int key) {
        if (!root || root->key == key) return root;
        if (root->key > key) return search(root->left, key);
        return search(root->right, key);
    }

public:
    void Initialize(const std::vector<int>& keys) {
        root = nullptr; 
        for (int key : keys) {
            Insert(key);
        }
    }

    void Insert(int key) {
        root = insert(root, key);
    }

    void Delete(int key) {
        root = deleteNode(root, key);
    }

    Node* Search(int key) {
        return search(root, key);
    }
};