#ifndef CS300_HW3_BST_H
#define CS300_HW3_BST_H

//CS300-HW3 CAGRI CAN SARACAYDIN 30984

using namespace std;

template <class Key, class Value>
class BST {
private:
    struct Node {
        Key key; // string
        Value value; // number
        Node* left;
        Node* right;

        Node(Key k, Value v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void makeEmpty(Node* & t) {
        if (t != nullptr) {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = nullptr;
    }


    Node* findMin(Node* node) {

        if (node != nullptr){ // going far left
            while (node->left != nullptr) {
                node = node->left;
            }
        }
        return node;
    }

    Node* insert(Node* node, Key key, Value value) {
        // insertion
        if (node == nullptr) {
            return new Node(key, value);
        }

        if (key < node->key) { // left child
            node->left = insert(node->left, key, value);
        }

        else if (key > node->key) { // right child
            node->right = insert(node->right, key, value);
        }

        else {
            return node; // already inserted
        }

        return node; // balanced
    }

    Value search(Node* t, Key key) {

        if (t == nullptr) {
            return nullptr;
        }

        else if (key < t->key) {
            return search(t->left, key); //if searched key is smaller go left child
        }

        else if (key > t->key) { // if searched key is bigger than root then check right child
            return search(t->right, key);
        }

        else  { // we found it
            return t->value;
        }

    }

    Node* deleteNode(Node* root, Key key) {
        // deletion
        if (root == nullptr) {
            return root;
        }

        if (key < root->key) {
            root->left = deleteNode(root->left, key);
        }

        else if (key > root->key) {
            root->right = deleteNode(root->right, key);
        }

        else if (root->left != nullptr && root->right != nullptr) { // two children

            Node* temp = findMin(root->right);

            root->key = temp->key;
            root->value = temp->value;

            root->right = deleteNode(root->right, temp->key);

        }

        else { // one or no children

            Node* temp = (root->left!= nullptr) ? root->left : root->right; // points to not null child if exists
            if (temp == nullptr) { // no child
                temp = root;
                root = nullptr;
            } else { // one child
                *root = *temp; // replace it with child node
            }
            delete temp;
        }

        return root; // balanced
    }

public:
    BST() : root(nullptr) {}

    ~BST() {  // destructor
        makeEmpty(root);
    }

    void makeEmpty() {
        makeEmpty(root);
    }

    void insert(Key key, Value value) {
        root = insert(root, key, value);
    }

    Value search(Key key) {
        return search(root, key);
    }
};

#endif //CS300_HW3_BST_H