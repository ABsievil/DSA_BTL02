#include<bits/stdc++.h> 
#include<string>
using namespace std;

std::map<char, int> convertToMap(string na){
    std::map<char, int> list;
    char x ;
    int count =0;
    for(char c : na){
        if(c == na[0]){
            x = c;
            count++;
        }
        else if(c == x) count++;
        else {
            list.insert({x, count});
            x=c; 
            count =1; 
        }
    }   
    list.insert({x, count}); 
    return list;
}


string rearrangeName(string na, bool& isGreaterThan3ch){
    std::stack<char> S;
    std::queue<char> tempQ;
    string name = na;
    int numCharacter = 0;
    while (name != ""){
        bool isStart = false;
        for(char c:name){
            if(!isStart) {
                S.push(c);
                isStart = true;
                numCharacter++;
            }
            else if(S.top() == c) S.push(c);
            else tempQ.push(c);
        }
        string tempName = "";
        while (!tempQ.empty()){
            tempName += tempQ.front();
            tempQ.pop();
        }
        name = tempName;
    }
    string out = "";
    while(!S.empty()){
        out += S.top();
        S.pop();
    }
    reverse(out.begin(), out.end());
    if(numCharacter >=3) isGreaterThan3ch = true;
    return out;
}
char spEncypt(char ch, int shift){
    if (std::isalpha(ch)) {
        char base = std::isupper(ch) ? 'A' : 'a';
        return static_cast<char>((ch - base + shift + 26) % 26 + base);
    } 
    else return ch;  // if ch is not character then ignore
}
string encryptCaesar(string na){
    std::stack<char> S;
    int shift = 0;
    string out = "";
    for(char c: na){
        if(S.empty()) {
            S.push(c);
            shift++;
        }
        else if(S.top() == c) {
            S.push(c);
            shift++;
        }
        else {
            out += spEncypt(S.top(), shift);
            while(!S.empty()) S.pop();
            S.push(c);
            shift = 1;
        }
    }
    if(!S.empty()) out += spEncypt(S.top(), shift);
    return out;
}  

// Heap class
template <typename E, typename Comp> 
class heap {
    private:
        E* Heap; // Pointer to the heap array
        int maxsize; // Maximum size of the heap
        int n; // Number of elements now in the heap
        // Helper function to put element in its correct place
        void siftdown(int pos) {
            while (!isLeaf(pos)) { // Stop if pos is a leaf
                int j = leftchild(pos); int rc = rightchild(pos);
                if ((rc < n) && Comp::prior(Heap[rc], Heap[j]))
                j = rc; // Set j to greater child’s value
                if (Comp::prior(Heap[pos], Heap[j])) return; // Done
                swap(Heap, pos, j);
                pos = j; // Move down
            }
        }
    public:
        heap(E* h, int num, int max) { Heap = h; n = num; maxsize = max; buildHeap(); }
        ~heap(){}
        int size() const { return n; }
        bool isLeaf(int pos) const { return (pos >= n/2) && (pos < n); }
        int leftchild(int pos) const { return 2*pos + 1; } // Return leftchild position
        int rightchild(int pos) const { return 2*pos + 2; } // Return rightchild position
        int parent(int pos) const { return (pos-1)/2; }
        void buildHeap() { for (int i=n/2-1; i>=0; i--) siftdown(i); }
        // Insert "it" into the heap
        void insert(const E& it) {
            //Assert(n < maxsize, "Heap is full");
            int curr = n++;
            Heap[curr] = it; // Start at end of heap
            // Now sift up until curr’s parent > curr
            while ((curr!=0) && (Comp::prior(Heap[curr], Heap[parent(curr)]))) {
                swap(Heap, curr, parent(curr));
                curr = parent(curr);
            }
        }
};



template<typename E> 
class HuffNode {
    public:
        virtual ~HuffNode(){}
        virtual int weight() =0;
        virtual bool isLeaf()= 0;
};
template<typename E>
class LeafNode: public HuffNode<E>{
    private:
        E it;
        int wgt;
    public:
       LeafNode(const E& val, int freq): it(val), wgt(freq){}
       int weight() { return wgt;}
       E val() {return it;}
       bool isLeaf() { return true; } 
};
template<typename E>
class IntlNode: public HuffNode<E>{
    private:
        HuffNode<E>* lc; //left child
        HuffNode<E>* rc; //right child
        int wgt;         // subtree weight
    public:
        IntlNode(HuffNode<E> *l, HuffNode<E> *r){
            wgt = l->weight() + r->weight(); 
            lc = l;
            rc = r;
        }
        int weight(){ return wgt;}
        bool isLeaf(){ return false; }

        HuffNode<E>* left() const { return lc;}
        HuffNode<E>* right() const { return rc;}

        void setLeft(HuffNode<E>* b){ lc = (HuffNode<E>*)b; }
        void setRight(HuffNode<E>* b) { rc = (HuffNode<E>)b; }
};
template<typename E>
class HuffmanTree {
    private:
        HuffNode<E>* Root;
    public:
        HuffmanTree(const E& val, int freq) { Root = new LeafNode<E>(val, freq); }
        //Internal node constructor
        HuffmanTree(HuffmanTree<E>* l, HuffmanTree<E>* r) { 
            Root = new IntlNode<E>(l->root(), r->root());
        }
        ~HuffmanTree(){}
        HuffNode<E> *root(){ return Root; }
        int weight(){ return Root->weight(); }
};
template<typename E>
class HuffTree {
    private:
        HuffNode<E>* Root;
    public:
        HuffTree(const E& val, int freq) { Root = new LeafNode<E>(val, freq); }
        //Internal node constructor
        HuffTree(HuffTree<E>* l, HuffTree<E>* r) { 
            Root = new IntlNode<E>(l->root(), r->root());
        }
        ~HuffTree(){}
        HuffNode<E> *root(){ return Root; }
        int weight(){ return Root->weight(); }
};
template <typename E>
class minTreeComp {
public:
    bool operator()(const HuffTree<E>* tree1, const HuffTree<E>* tree2) const {
        return tree1->weight() > tree2->weight(); // So sánh theo trọng số, giả sử ưu tiên cây có trọng số nhỏ hơn
    }
};
// Build a Huffman tree from a collection of frequencies
template <typename E> 
HuffTree<E>* buildHuff(HuffTree<E>** TreeArray, int count) {
    heap<HuffTree<E>*,minTreeComp<E>>* forest = new heap<HuffTree<E>*, minTreeComp<E>>(TreeArray, count, count);
    HuffTree<char> *temp1, *temp2, *temp3 = NULL;

    while (forest->size() > 1) {
        temp1 = forest->removefirst(); // Pull first two trees
        temp2 = forest->removefirst(); // off the list
        temp3 = new HuffTree<E>(temp1, temp2);
        forest->insert(temp3); // Put the new tree back on list
        delete temp1; // Must delete the remnants
        delete temp2; // of the trees we created
    }
    return temp3;
}

int main() {
    cout<<"hi"<<endl;
    string m = "zzZZa";
    bool is = false;
    string x = rearrangeName(m, is);
    cout<<"value arranged: "<<x<<endl;
    string u = encryptCaesar(x);
    cout<<"value encypt: "<<u<<endl;
    string x2 = rearrangeName(u, is);
    cout<<"value rearranged lan 2: "<<x2<<endl;
    // map<char, int> myMap =  convertToMap(x);
    // for(const auto& c: myMap){
    //     cout<<c.first<<" "<<c.second<<endl;
    // }
    
    return 0;
}