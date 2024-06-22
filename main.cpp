#include<iostream>
#include<fstream>
using namespace std;

struct node {
    int data;
    struct node* left;
    struct node* right;
};
class avlTree {
public:
    node* root;
    avlTree();
    void print(node*, int);
    bool find(node*, int);
    node* insert(node*, int);
    node* deleteNode(node*, int);
private:
    int height(node*);
    int subtractChildren(node*);
    node* rightRotate(node*);
    node* leftRotate(node*);
    node* doubleLeftRotate(node*);
    node* doublRightRotate(node*);
    node* checkRotation(node*);
};
// constructor, creates empty tree
avlTree::avlTree() {
    root = NULL;
}
// print avl tree to termain with root on the left and leaves on the right
void avlTree::print(node* top, int depth) {
    if (top != NULL) {
        print(top->right, depth + 1);       // print nodes and print levels in columns
        std::cout << endl;

        for (int i = 0; i < depth && top != root; i++) 
            std::cout << "        ";

        if (top == root)
            std::cout << "Root -> ";

        std::cout<< top->data;
        
        std::cout << "(";                   // prints height data, for error checking 
        std::cout << height(top);
        std::cout << ")";

        print(top->left, depth + 1);
    }
}
// check if node is in the tree, prevents duplicate values
bool avlTree::find(node* n, int check) {
    if (n == NULL)
        return false;

    if (n->data == check)
        return true;

    if (check < n->data) 
        return find(n->left, check);
    
    if (check > n->data)                    // if node matches passed value return true
        return find(n->right, check);
            
    return false;                           // value not found, return false
}
// insert value into tree once inserted, check if rotation is needed
node* avlTree::insert(node* root, int num) {
    if (find(root, num) != true) {
        if (root == NULL) {
            root = new node;
            root->data = num;
            root->left = NULL;
            root->right = NULL;
            return root;
        }
        else if (num < root->data) {
            root->left = insert(root->left, num);
            root = checkRotation(root);
        }
        else if (num >= root->data) {
            root->right = insert(root->right, num);
            root = checkRotation(root);
        }
    }
    return root;
}
// delete passed value from tree, moves children of deleted node to balance the tree
node* avlTree::deleteNode(node* root, int num) {
    if (root == NULL)
        return root;

    if (num < root->data)
        root->left = deleteNode(root->left, num);

    else if (num > root->data)
        root->right = deleteNode(root->right, num);

    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            node* temp = root->left ?
                root->left :
                root->right;

            if (temp == NULL) {                      // node has no children
                temp = root;
                root = NULL;
            }
            else                                    // node has one child
                *root = *temp;

            free(temp);
        }
        else
        {
            node* temp = root->right;               // node has two children, moves smallest value into node

            while (temp->left != NULL)
                temp = temp->left;

            root->data = temp->data;

            root->right = deleteNode(root->right, temp->data);
        }
    }
    return root;
}
// increment height for each level a node is away from leaf
int avlTree::height(node* n) {
    int h = 0;
    if (n != NULL)
        h = max(height(n->left), height(n->right)) + 1;
    return h;
}
// subtracts left and right child, for use in checkRotation
int avlTree::subtractChildren(node* n) {
    return height(n->left) - height(n->right);
}
// rotate right, balances tree
node* avlTree::rightRotate(node* parent) {
    node* n = parent->right;
    parent->right = n->left;
    n->left = parent;
    return n;
}
// rotate left, balances tree
node* avlTree::leftRotate(node* parent) {
    node* n = parent->left;
    parent->left = n->right;
    n->right = parent;
    return n;
}
// rotate right then left, or double rotation, balances tree
node* avlTree::doubleLeftRotate(node* parent) {
    node* n = parent->left;
    parent->left = rightRotate(n);
    return leftRotate(parent);
}
// rotate left then right, or double rotation, balances tree
node* avlTree::doublRightRotate(node* parent) {
    node* n = parent->right;
    parent->right = leftRotate(n);
    return rightRotate(parent);
}
// determine if tree needs to be balanced, uses height data
node* avlTree::checkRotation(node* n) {
    int balance = subtractChildren(n);          // check the number of nodes on either side of passed node
    if (balance > 1) {
        if (subtractChildren(n->left) > 0)
            n = leftRotate(n);
        else
            n = doubleLeftRotate(n);
    }
    else if (balance < -1) {
        if (subtractChildren(n->right) > 0)
            n = doublRightRotate(n);
        else
            n = rightRotate(n);
    }
    return n;
}
int main() {
    avlTree tree;
    string file;
    int num;
    ifstream inFile;

    std::cout << "Enter file with list: ";
    std::cin >> file;
    inFile.open(file);

    while (inFile.is_open() == false) {                // grab list of tree nodes
        std::cout << "Invalid file, please re-enter: ";
        std::cin >> file;
        inFile.open(file);
    }

    while (inFile.eof() == false) {
        inFile >> num;
        tree.root = tree.insert(tree.root, num);                  // insert numbers until the end of file is reached
        //tree.print(tree.root, 1);                          // uncomment to view tree after every insertion and rotation
    }

    tree.print(tree.root, 1);                                // print tree after insertion

    inFile.close();
    std::cout << "\nEnter file with deletion list: ";
    std::cin >> file;
    inFile.open(file);

    while (inFile.is_open() == false) {                 // grab list of nodes to be deleted
        std::cout << "Invalid file, please re-enter: ";
        std::cin >> file;
        inFile.open(file);
    }

    while (inFile.eof() == false) {
        inFile >> num;
        tree.root = tree.deleteNode(tree.root, num);              // delete numbers until the end of file is reached
        //tree.print(root, 1);                          // uncomment to view tree after every deletion and roatation
    }

    tree.print(tree.root, 1);                                // print tree after deletion
}