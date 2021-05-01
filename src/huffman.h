#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <math.h>
#include <string>
#include <bitset>
#include <string_view>

//typedefs

typedef std::pair<double, char> PQPairs;
//tree
struct node{
    PQPairs info;
    node* left;
    node* right;
};

typedef node* Node;
typedef std::pair< char, double> mapPairs;
typedef std::map< char,double > myMap;
typedef std::map< char,std::string > encodingMap; //used for saving the final Huffman encoding

//order is used for keep sorted the priority queue in correct order
struct order {
    constexpr bool operator()(Node const& a,Node const& b)
        const noexcept{
        return a->info.first >= b->info.first;
    }
};
typedef std::priority_queue< Node , std::vector<Node >, order > myPriorityQueue;

node* createNewNode(PQPairs );
void leftInsert(node* &, node* );
void rightInsert(node* &, node* );
node* sumNodesValues(node* &, node* &);
void printInorder(Node );
encodingMap extractCode(Node , char* , int );
void printArr(char* , int);
int height(Node);