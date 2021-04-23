#include "huffman.h"

using namespace std;

Node createNewNode(PQPairs x){
    Node nn = new node;
    nn->info = x;
    nn->left  = nullptr;
    nn->right = nullptr;

    return nn;
}

void leftInsert(Node &root, Node x){
    root->left = x;
}

void rightInsert(Node &root, Node x){
    root->right = x;
}

//takes 2 nodes and return a node with the sums of each info values
Node sumNodesValues(Node &a, Node &b){
    PQPairs ret;
    ret.first = a->info.first + b->info.first; 
    ret.second = char(NULL); //???
    return createNewNode(ret);
}

bool isLeaf(Node l){
    return(l->left == nullptr && l->right == nullptr);
}



void printInorder(Node root){
    // if node is null, return
    static string code = ""; 
    if (!root)
        return;
     
    // if node is leaf node, print its info   
    if (isLeaf(root)){
        cout << root->info.second << " code:" << code << endl;
        code.clear();
        return;
    }
 
    // if left child exists, check for leaf
    // recursively
    if (root->left){
       code += "1";
       printInorder(root->left);
    }
         
    // if right child exists, check for leaf
    // recursively
    if (root->right){
       code += "0";
       printInorder(root->right);
    }
}

/* UTILITY FUNCTIONS */
/* Utility that prints out an array on a line. */
void printArray(PQPairs ints[], int len) 
{ 
    int i; 
    for (i = 0; i < len; i++) 
    { 
        cout << ints[i].second << "  "; 
    } 
    cout<<endl; 
} 
  
/* Recursive helper function -- given a node, 
and an array containing the path from the root
node up to but not including this node, 
print out all the root-leaf paths.*/
void printPathsRecur(Node node, PQPairs path[], int pathLen) 
{ 
    if (node == NULL) 
        return; 
      
    /* append this node to the path array */
    path[pathLen] = node->info; 
    pathLen++; 
      
    /* it's a leaf, so print the path that led to here */
    if (isLeaf(node)) 
    { 
        printArray(path, pathLen); 
    } 
    else
    { 
        /* otherwise try both subtrees */
        printPathsRecur(node->left, path, pathLen); 
        printPathsRecur(node->right, path, pathLen); 
    } 
} 
  
void printPaths(Node node) 
{ 
    PQPairs path[1000]; 
    printPathsRecur(node, path, 0); 
} 
  