
#include "huffman.h"

using namespace std;


/********
-- Utility --
*********/

//'efficent' way to count all chars in a file; seek to the end of file,
// save the number of chars and then seeks to the beginning of file
unsigned int wc(ifstream &in){
    unsigned int chars;
    in.seekg(0,in.end);
    streampos end_pos = in.tellg();
    chars = int(end_pos);
    in.seekg(0);

    return chars;
}

//for debug: print the map
void printMap(myMap m){
    myMap::iterator itrP;
    for (itrP = m.begin(); itrP != m.end(); ++itrP) {
            cout << itrP->first
              << ':'
              << itrP->second
              << endl;
    }
    cout << endl;
}

//for debug: print the map
void printEncMap(encodingMap em){
    encodingMap::iterator itrE;
    for (itrE = em.begin(); itrE != em.end(); ++itrE) {
            cout << itrE->first
              << ':'
              << itrE->second
              << endl;
    }
    cout << endl;
}

//for debug: print the queue
void printPQ(myPriorityQueue gq){
    myPriorityQueue g = gq;
    while (!g.empty()) {
        pair<double,char> top = g.top()->info;
        cout << "k:" << top.first << "\t";
        cout << "v:" << top.second << endl;
        g.pop();
    }
    cout << endl;
}

//count the chars occurency of a file, saves in a file and calculate the frequency of each char
myMap getFrequencyInFile (string filename, uint &charInFile){
    ifstream in (filename);
    char c;
    myMap m;
    charInFile = wc(in);
#ifdef DEBUG
    cout << charInFile << endl;
#endif
    //fill map with the occurency
    while (in.get(c) && in.is_open()) {
        if(m.count(c) > 0)
            m.at(c)++;
        else
            m.insert(mapPairs(c,1));
    }
    //calculate the frequency of each character
    myMap::iterator itr;
    for (itr = m.begin(); itr != m.end(); ++itr) {
        itr->second = itr->second/charInFile;
    }
#ifdef DEBUG
    printMap(m);
#endif
    return m;
}

//convert map in a Priority Queue of nodes
myPriorityQueue createPQ (myMap m){
    myPriorityQueue pq;
    
    myMap::iterator itr;
    for (itr = m.begin(); itr != m.end(); ++itr) {
        pq.push(createNewNode(make_pair(itr->second,itr->first)));
    }
#ifdef DEBUG
    printPQ(pq);
#endif
    return pq;
}

//calculate the shannon entropy
double ShannonEntropy (myMap m){
    myMap::iterator itrP;
    double se;
    for (itrP = m.begin(); itrP != m.end(); ++itrP) {
        se += itrP->second * log2(1/itrP->second);
    }
    return se;
}

//build the huffman coding tree
void HuffmanCode(myPriorityQueue &pq){

    uint pqsize = pq.size(); 
    for(uint i = 0; i < pqsize-1; i++){
        
        Node x = pq.top();
#ifdef DEBUG
        cout << "x:"<< pq.top()->info.first ;
#endif
        pq.pop();
        Node y = pq.top();
#ifdef DEBUG
        cout << " ~~~~ y:"<< pq.top()->info.first ;
#endif
        pq.pop();
        Node z = sumNodesValues(x,y);
#ifdef DEBUG
        cout << " ~~~~ z:"<< z->info.first << endl;
#endif
        leftInsert(z,x);
        rightInsert(z,y);

        pq.push(z);
    }
    
}

//used to add padding '0' at the end of the binary compressed file
void addPadding(string &bitString){
    while(bitString.length()%8 != 0)
        bitString.push_back('0');
}

//write the compressed file in to a file
void writeBits(string bitString,int bitLen,string filename){
    bitset<8> byte;
    int bitcount = 0;
    
    ofstream compr(filename);
    if(compr){
        while(bitcount < bitLen){
            for (int i = 0;i<8;++i ){//fill a byte with 1 or 0(8 bit in a byteset)
                if(bitString.at(bitcount) == '1'){
                    byte[i] = 1;
                    bitcount++;
                }else if(bitString.at(bitcount) == '0'){
                    byte[i] = 0;
                    bitcount++;
                }
            }
            compr << (char) byte.to_ulong();//cast the bitset to a char and write it to file

            byte.reset();

        }
    }
    compr.close();
}

//calculate the lenght
double getAL(encodingMap enc , myMap freq){
    myMap::iterator itrF;
    encodingMap::iterator itrE;
    double al;
    for (itrF = freq.begin(),itrE = enc.begin(); itrE != enc.end(); ++itrF,++itrE) {
        al += itrF->second * itrE->second.length();
    }
    return al;    
}

//encode the given file in to a string using the huffman encoding
string compress(encodingMap enc, string filename , uint &bitLen){
    string compress = "";
    ifstream in(filename);
    char c;
    while (in.get(c) && in.is_open()) {
        
        compress += enc.at(c); //count the bits in the string
        bitLen += enc.at(c).length();

    }
    return compress;
}

//write the huffman coding to a file pluse some other infos
void writeDecodeInfo(int bitLen, encodingMap em, string filename, myMap m,int charInFile,string toCompres){
    ofstream info(filename);
    if(info){
        info << to_string(bitLen);
        info << "\n";
        encodingMap::iterator itrE;
        for (itrE = em.begin(); itrE != em.end(); ++itrE) {
            info << (char)(itrE->first) << ":" << itrE->second << "|";
        }
        info << "\n";
        info << "entropy: " << ShannonEntropy(m) << "\n";
        info << "lenght: " << getAL(em,m) << "\n";
        info<< "Number of char before: "<< charInFile << "(" << charInFile * 8 << " bit)" << endl;
        info<< "Number of char after: "<< toCompres.length() / 8 << "(" << bitLen << " bit ,with padding "<< toCompres.length() <<")" << endl;
    }
    info.close();
}


int main(int argc,char** argv){
    myMap m;
    double entropy;
    double lenght;
    uint bitLen = 0; 
    uint charInFile = 0;
    if(argc < 2){
        cout<< "Usage:\n"<< argv[0] << " <file_to_compress>" << endl;
        return 1;
    }

    m = getFrequencyInFile(argv[1],charInFile);

    myPriorityQueue pq = createPQ(m);

#ifdef DEBUG
    cout << "size of pq: " << pq.size() << endl;
#endif
    HuffmanCode(pq);
#ifdef DEBUG
    printPQ(pq);
#endif
    char arr[height(pq.top())];
    encodingMap enc;
    enc = extractCode(pq.top(),arr,0);
    cout<< "Huffman code: "<< endl;
    printEncMap(enc);

    string toCompres = compress(enc,argv[1],bitLen);
    addPadding(toCompres);
    string arg1(argv[1]);
    char chose;
    cout << "save compressed file? [y/n] ";
    cin >> chose;
    if(chose == 'y' || chose == 'Y'){
        string outFile = arg1 + ".huff";
        cout << "saving compressed file to " << outFile << endl;
        writeBits(toCompres,toCompres.length(),outFile);
    }
    //prints compression infos
    entropy = ShannonEntropy(m);
    cout << "Shannon entropy of "<< argv[1] << ": \t" << entropy << endl;

    lenght = getAL(enc,m);
    cout<< "Lenght of Huffman coding: \t\t"<< lenght << endl;

    cout<< "Number of char before: \t\t\t"<< charInFile << "(" << charInFile * 8 << " bit)" << endl;

    cout<< "Number of char after: \t\t\t"<< toCompres.length() / 8 << "(" << bitLen << " bit ,with padding "<< toCompres.length() <<")" << endl;

    cout << "save info to file? [y/n] ";
    cin >> chose;
    if(chose == 'y' || chose == 'Y'){
        string outFileInfo = arg1 + ".info";
        cout << "saving infos about compressed file to " << outFileInfo  << endl;
        writeDecodeInfo(bitLen,enc,outFileInfo,m,charInFile,toCompres);
    }

    return 0;
}
