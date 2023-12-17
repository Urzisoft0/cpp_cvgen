#include <iostream>
#include <Profiler.h>
using namespace std;

Profiler prof("AVL Tree");

/*
 *  Delete and select operations are both Log(n) in terms of complexity, for they only develop on a branch and
 *  the tree is balanced.
 *  Build operation is linear, because it computes each node's size afterwards.
 */

struct Node {
    int val{}, height{}, size{};
    Node* right{}, * left{}, * parent{};
};

//  Function to initiate a node
Node* spawn(int val, Node* parent) {
    auto tree = new Node;
    tree->val = val;

    tree->right = nullptr;
    tree->left = nullptr;

    tree->parent = parent;
    tree->height = 1;

    return tree;
}

// Function to return the height of a node
int height(Node* tree) {
    if (tree == nullptr) return 0;
    return tree->height;
}

//  Return maximum height between two nodes
int max_height(int x, int y) {
    return x > y ? x : y;
}

//  Function to left rotate
Node* left_rotate(Node* node, Operation &sample) {
    Node* x = node->right;
    Node* y = x->left;

    x->left = node;
    node->right = y;

    x->parent = node->parent;
    node->parent = x;

    if (y != nullptr)
        y->parent = node;

    x->size = node->size;
    int l = node->left != nullptr ? node->left->size : 0;
    int r = node->right != nullptr ? node->right->size : 0;
    node->size = r + l + 1;

    sample.count(2);

    node->height = max_height(height(node->right), height(node->left)) + 1;
    x->height = max_height(height(x->right), height(x->left)) + 1;

    return x;
}

//  Function to right rotate
Node* right_rotate(Node* node, Operation &sample) {
    Node* x = node->left;
    Node* y = x->right;

    x->right = node;
    node->left = y;

    x->parent = node->parent;
    node->parent = x;

    if (y != nullptr)
        y->parent = node;

    x->size = node->size;
    int l = node->left != nullptr ? node->left->size : 0;
    int r = node->right != nullptr ? node->right->size : 0;
    node->size = r + l + 1;

    sample.count(2);

    node->height = max_height(height(node->right), height(node->left)) + 1;
    x->height = max_height(height(x->right), height(x->left)) + 1;

    return x;
}

//  Function to left rotate
Node* left_rotate2(Node* node) {
    Node* x = node->right;
    Node* y = x->left;

    x->left = node;
    node->right = y;

    x->parent = node->parent;
    node->parent = x;

    if (y != nullptr)
        y->parent = node;

    node->height = max_height(height(node->right), height(node->left)) + 1;
    x->height = max_height(height(x->right), height(x->left)) + 1;

    return x;
}

//  Function to right rotate
Node* right_rotate2(Node* node) {
    Node* x = node->left;
    Node* y = x->right;

    x->right = node;
    node->left = y;

    x->parent = node->parent;
    node->parent = x;

    if (y != nullptr)
        y->parent = node;

    node->height = max_height(height(node->right), height(node->left)) + 1;
    x->height = max_height(height(x->right), height(x->left)) + 1;

    return x;
}

//  Return balance of a node (left->height - right->height)
int balance(Node* node) {
    return node == nullptr ? 0 : height(node->left) - height(node->right);
}

//  Function to resize the tree after build
int resize(Node* root, Operation sample) {
    if (root == nullptr)
        return 0;

    int left = resize(root->left, sample);
    int right = resize(root->right, sample);
    root->size = left + right + 1;
    sample.count();

    return left + right + 1;
}

//  Function to insert in an AVL tree
Node* insert(Node* tree, int val, Node* parent) {
    if (tree == nullptr)
        return spawn(val, parent);

    if (val < tree->val) {
        tree->left = insert(tree->left, val, tree);
    }

    else if (val > tree->val) {
        tree->right = insert(tree->right, val, tree);
    }

    tree->height = max_height(height(tree->left), height(tree->right)) + 1;
    int bal = balance(tree);

    if (bal > 1) {
        if (val < tree->left->val)
            return right_rotate2(tree);
        else {
            tree->left = left_rotate2(tree->left);
            return right_rotate2(tree);
        }
    }

    if (bal < -1) {
        if (val > tree->right->val)
            return left_rotate2(tree);
        else {
            tree->right = right_rotate2(tree->right);
            return left_rotate2(tree);
        }
    }

    return tree;
}

//  Function to return the maximum node in the left subtree (used in deletion)
Node *max_node(Node *tree) {
    Node *temp = tree;

    if (temp) {
        while (temp->right)
            temp = temp->right;
    }
    return temp;
}

//  Function to delete a node. It has 3 cases: 0 children, 1 child, 2 children.
Node *del(Node *tree, int val, Operation sample) {
    if (tree == nullptr)
        return tree;

    if (val < tree->val)
        tree->left = del(tree->left, val, sample);
    else if (val > tree->val)
        tree->right = del(tree->right, val, sample);
    else {
        int ok = 2;
        if (tree->left == nullptr) ok--;
        if (tree->right == nullptr) ok--;

        if (ok == 1) {
            Node *parent = tree->parent;
            while (parent != nullptr) {
                parent->size--;
                sample.count();
                parent = parent->parent;
            }
            Node *x;
            Node *aux_parent = tree->parent;
            if (tree->left != nullptr)
                x = tree->left;
            else
                x = tree->right;

            if (x == nullptr) {
                tree = nullptr;
            } else
                *tree = *x;

            tree->parent = aux_parent;
            x = nullptr;

            free(x);
        } else if (ok == 2) {
            Node *x = max_node(tree->left);
            tree->val = x->val;
            tree->left = del(tree->left, x->val, sample);
        }
        else if (ok == 0) {
            Node *parent = tree->parent;
            while (parent != nullptr) {
                parent->size--;
                sample.count();
                parent = parent->parent;
            }
            tree = nullptr;
        }
    }
    if (tree == nullptr)
        return tree;

    tree->height = max_height(height(tree->left), height(tree->right)) + 1;
    int bal = balance(tree);

    if (bal > 1 && balance(tree->left) >= 0) {
        if (balance(tree->left) >= 0)
            return right_rotate(tree, sample);
        else {
            tree->left = left_rotate(tree->left, sample);
            return right_rotate(tree, sample);
        }
    }

    if (bal < -1 && balance(tree->right) <= 0) {
        if (balance(tree->right) <= 0)
            return left_rotate(tree, sample);
        else {
            tree->right = right_rotate(tree->right, sample);
            return left_rotate(tree, sample);
        }
    }
    return tree;
}

//  Function to build the tree
Node* build(Node* tree, int n) {
    Operation build = prof.createOperation("build", n);

//    int *a = (int *)malloc(sizeof(int) * n);
//    FillRandomArray(a, n, 1, n, true, ASCENDING);

    for (int i = 1; i <= n; i++)
        tree = insert(tree, i, nullptr);
    resize(tree, build);

    return tree;
}

//  Function to efficiently select the i'th smaller node
int os_select(Node *tree, int i, int n) {
    int r;
    Operation sel = prof.createOperation("select", n);
    if (tree != nullptr) {
        r = tree->left != nullptr ? tree->left->size + 1 : 1;
        sel.count();
    }
    else
        return 0;
    if (i == r)
        return tree->val;
    else if (i < r)
        return os_select(tree->left, i, n);
    else
        return os_select(tree->right, i - r, n);
}

//  Print in order
void in_order(Node* root) {
    if (root != nullptr) {
        in_order(root->left);
        if (root->parent == nullptr)
            cout << root->val << "-" << 0 << "-" << root->size << " ";
        else
            cout << root->val << "-" << root->parent->val << "-" << root->size << " ";
        in_order(root->right);
    }
}

//  Pretty print
void pretty_print(Node *root, int indent) {
    if (root != nullptr) {
        cout << string(indent, ' ') << root->val << endl;
        pretty_print(root->right, indent + 4);
        pretty_print(root->left, indent + 4);
    }
}

//  Print all nodes by rank
void print_nodes(Node *node) {
    int l = node->left != nullptr ? node->left->size : 0;
    int r = node->right != nullptr ? node->right->size : 0;
    int n = node != nullptr ? l + r + 1 : 0;

    cout << endl;
    for (int i = 1; i <= n; i++) {
        int val = os_select(node, i, n);
        cout << i << ":" << val << " ";
    }
    cout << "\n" << string(60, '.') << "\n";
}

void test() {
    cout << "\t-PRINT ORDER-\nI: In Order(val, parent, size)\n"
            "II: pretty print\nIII: i'th smallest node(rank, val)\n\n";
    Operation sample = prof.createOperation("sample", 11);
    Node* tree = nullptr;
    pretty_print(tree, 0);

    tree = build(tree, 11);
    in_order(tree);
    cout << endl << endl;
    pretty_print(tree, 0);
    print_nodes(tree);

    tree = del(tree, 4, sample);
    in_order(tree);
    cout << endl << endl;
    pretty_print(tree, 0);
    print_nodes(tree);

    tree = del(tree, 8, sample);
    in_order(tree);
    cout << endl << endl;
    pretty_print(tree, 0);
    print_nodes(tree);

    tree = del(tree, 2, sample);
    in_order(tree);
    cout << endl << endl;
    pretty_print(tree, 0);
    print_nodes(tree);
}

void perf() {
    int starting_size = 100;
    int final_size = 10000;
    int increment = 100;

    for (int i = 1; i <= 5; i++) {
        for (int n = starting_size; n <= final_size; n += increment) {
            Node *tree = nullptr;
            tree = build(tree, n);
            int size = n;

            for (int k = 1; k <= n; k++) {
                Operation erase = prof.createOperation("delete", n);
                int b[1];
                FillRandomArray(b, 1, 1, size + 1, true, UNSORTED);
                int q = os_select(tree, b[0], n);
                tree = del(tree, q, erase);
                size--;
            }
        }
    }
    prof.divideValues("select", 5);
    prof.divideValues("delete", 5);
    prof.divideValues("build", 5);

    prof.createGroup("AVL Tree", "select", "build", "delete");
    prof.createGroup("Select", "select");
    prof.createGroup("Delete", "delete");
    prof.showReport();
}

int main() {
    test();
    //perf();
}
