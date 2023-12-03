#include <iostream>
#include <vector>
#define ROOT -1

//  Structure for the Binary Tree
struct binTree {
    int value;
    binTree *right, *left;
};

//  Structure for the Multi-Way Tree
struct mWayTree {
    int value = 0;
    std::vector<mWayTree *> vector;
};

// typedef std::vector and std::string to avoid long declarations
using nodeVector = std::vector<mWayTree *>;
using space = std::string;

using Vec = std::vector<int>;

/*  Pretty-Print for Parent-Vector (First Representation)
 *  We use and indent in order to set the spacing between parents and children
 *  Tree Traversal -> Recursive Call
 */
void printParent(const Vec& vector, int parent, int indent) {
    for (int i = 0; i < vector.size(); i++)
        if (vector[i] == parent) {
            std::cout << space(indent, ' ') << i + 1 << "\n";
            printParent(vector, i + 1, indent + 3);
        }
}

/*  First Transformation (from Parent-Vector to Multi-Way Tree)
 *  Prior to this function, we have to initiate the Node Vector
 *  ( which will be 1, 2, ..., vec.size() ) ! SEE LINE 103-108 !
 *
 */
void parentToMulti(const nodeVector& vector, Vec v) {
    int i = -1;
    while (++i < v.size())
        if (v[i] != -1)
            (*vector[v[i] - 1]).vector.push_back(vector[i]);
}

//  Pretty-Print for Multi-Way tree
void printMulti(mWayTree* node, int indent) {
    std::cout << space(indent, ' ') << (*node).value << std::endl;
    for (int i = 0; i < (*node).vector.size(); i++)
        printMulti((*node).vector[i], indent + 3);
}

//  Function to set the value and branches (NULL initially) for a Binary-Tree's node
void assign(mWayTree *el, binTree *cell) {
    (*cell).value = (*el).value;
    (*cell).right = nullptr;
    (*cell).left = nullptr;
}

/*  Second Transformation (from Multi-Way Tree to Binary Tree)
 *  It has a complexity of O(N), because it traverses each node of the Multi-Way tree once,
 *  inserting left and right side cells.
 */
void multiToBin(mWayTree *el, binTree *cell) {
    if (el) {
        assign(el, cell); // setting the value, left and right of cell
        if (!(*el).vector.empty()) { // if the Multi-Way tree's vector is not empty
            (*cell).left = (binTree *)malloc(sizeof(binTree *));
            multiToBin((*el).vector[0], (*cell).left); // recursive call to instantiate all left-side nodes
        }

        int index = 1; // after defining all leftmost nodes, search for their "siblings"
        while (index < (*el).vector.size()) { // link each left-side node to its sibling
            binTree *aux = (*cell).left;

            while ((*aux).right)
                aux = (*aux).right;

            (*aux).right = new binTree;
            /*
             * -NOT CORRECT-
             *  while((*cell).left->right)
             *      (*cell).left = (*cell).left->right;

             * (*cell).left->right = new binTree;
             * -NOT CORRECT-
            */
            multiToBin((*el).vector[index++], (*aux).right);
        }
    }
}

//  Function to get the root
int get_root(Vec vector) {
    int i = 0;
    for (; i != vector.size(); i++)
        if (vector[i] == ROOT)
            return i;
    return INT_MIN;
}

//Pretty-Print for Binary-Tree. It behaves like a preorder traversal
void printBin(binTree* tree, int indent) {
    if (tree != nullptr) {
        std::cout << space(indent, ' ') << (* tree).value << std::endl;
        printBin((* tree).left, indent + 3);
        printBin((* tree).right, indent);
    }
}

void demo() {
    Vec v = { 2, ROOT, 1, 1, 3, 5, 2, 2 }; // parent vector
    nodeVector multi; // multi-way structure vector
    auto bin = new binTree; // binary tree

    for(int i = 0; i < v.size(); i++) {
        auto node = new mWayTree;
        (*node).value = i + 1;
        multi.push_back(node);
    } // initiate multi-way structure vector

    int r = get_root(v);
    if (r == INT_MIN)
        return;
    parentToMulti(multi, v); //T1
    multiToBin(multi[r], bin); //T2

    std::cout << "Multi-way Tree Representation: \n";
    printMulti(multi[r], 0); //R2

    std::cout << "Parent Representation: \n";
    printParent(v, -1, 0); //R1

    std::cout << "Binary Tree Representation: \n";
    printBin(bin, 0); //R3

    std::free(bin);
}

void demo2() {
    Vec v = { 2, 7, 5, 2, 7, 7, ROOT, 5, 2 };
    nodeVector multi;
    auto bin = new binTree;

    for(int i = 0; i < v.size(); i++) {
        auto node = new mWayTree;
        (*node).value = i + 1;
        multi.push_back(node);
    }

    int r = get_root(v);
    parentToMulti(multi, v);
    multiToBin(multi[r], bin);

    std::cout << "Multi-way Tree Representation: \n";
    printMulti(multi[r], 0);

    std::cout << "Parent Representation: \n";
    printParent(v, -1, 0);

    std::cout << "Binary Tree Representation: \n";
    printBin(bin, 0);

    std::free(bin);
}

int main() {
    demo();
    demo2();
}
