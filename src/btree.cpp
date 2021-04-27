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

int height(Node root){
  int h = 0;
  if(root != NULL)
  {
    int lHeight = height(root->left);
    int rHeight = height(root->right);
    int maxHeight = max(lHeight,rHeight);
    h = maxHeight + 1;
  }
  return h;
}

encodingMap printCodes(Node root, char* arr, int i, string &ret){
    static encodingMap encoding;//saves the encoding table
    if (root->left) {
        arr[i] = '0';//if left append a 0
        printCodes(root->left, arr, i + 1,ret);
    }
    if (root->right) {
        arr[i] = '1';//if left append a 1
        printCodes(root->right, arr, i + 1,ret);
    }
    if (isLeaf(root)) {
        string aux(arr,i);//char array to string
        ret += aux;//append the char array to the 'stream' string
        encoding.insert(make_pair(root->info.second,aux)); //add row to map    
    }
    return encoding;
}