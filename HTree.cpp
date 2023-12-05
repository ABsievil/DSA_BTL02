#include<bits/stdc++.h> 
#include<string>
using namespace std;

/* Build HuffTree */
template<typename E> 
class HuffNode {
    public:
        virtual ~HuffNode(){}
        virtual int weight() =0;
        virtual bool isLeaf()= 0;
        virtual HuffNode<E>* left() =0; 
        virtual HuffNode<E>* right() =0;
        virtual void print() = 0;
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
       HuffNode<E>* left() { return nullptr;}
       HuffNode<E>* right() { return nullptr;}
       void print() {cout<<"val: "<<it<<" freq: "<<wgt<<endl; }
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

        HuffNode<E>* left() { return lc;}
        HuffNode<E>* right() { return rc;}

        void setLeft(HuffNode<E>* b){ lc = (HuffNode<E>*)b; }
        void setRight(HuffNode<E>* b) { rc = (HuffNode<E>)b; }

        void print() {cout<<"freq: "<<wgt<<endl; }
};

enum BalanceValue
{
    LH = -1,
    EH = 0,
    RH = 1
};  
template<typename E>
class HuffTree {
    private:
        HuffNode<E>* root;
    public:
        int time;
        class Node;
        // External Constructor 
        HuffTree(): root(nullptr){}
        HuffTree(const E& val, int freq, int ti) { root = new LeafNode<E>(val, freq); time = ti; }
        HuffTree(HuffTree<E>* l, HuffTree<E>* r, int ti) { root = new IntlNode<E>(l->Root(), r->Root()); time = ti; }
        ~HuffTree(){}

        HuffNode<E> *Root(){ return root; }
        int weight(){ return root->weight(); }

        void traveseHelper(HuffNode<E>* r) const {
            if (r == nullptr) return;
            traveseHelper(r->left()); // Đệ quy in cây con trái
            traveseHelper(r->right()); // Đệ quy in cây con phải
            if(!r->isLeaf()) cout<<"freq:"<<r->weight()<<endl;
            else {
                LeafNode<E> * x = static_cast<LeafNode<E>*>(r);
                cout<<"val: "<<x->val()<<" freq: "<<x->weight()<<endl;
            } 
            //r->print();
        }
        void travese() const {
            traveseHelper(root);
        }
        
        string getHuffmanCode(HuffNode<E>* r, char c, string currentPath = "") {
            if (r == nullptr) return "";

            // Nếu đến lá và ký tự là ký tự mong muốn, trả về đường đi
            if (r->isLeaf()) {
                LeafNode<E>* leaf = static_cast<LeafNode<E>*>(r);
                if (leaf->val() == c) {
                    return currentPath;
                }
                return "";
            }

            // Nếu không phải lá, kiểm tra ở cả hai phía
            string leftPath = getHuffmanCode(r->left(), c, currentPath + "0");
            string rightPath = getHuffmanCode(r->right(), c, currentPath + "1");

            // Nếu tìm thấy ở một trong hai phía, trả về đường đi
            if (!leftPath.empty()) {
                return leftPath;
            } else if (!rightPath.empty()) {
                return rightPath;
            }

            return ""; // Nếu không tìm thấy ở cả hai phía, trả về chuỗi rỗng
        }
		string binEncypt(string str) {
            reverse(str.begin(), str.end());
			string out = "";
			for(char c: str){
				string leftPath = getHuffmanCode(root->left(), c, "0");
                string rightPath = getHuffmanCode(root->right(), c, "1");

                string tempOut = "";
                if (!leftPath.empty()) {
                    tempOut = leftPath;
                } else if (!rightPath.empty()) {
                    tempOut = rightPath;
                }
                out += tempOut;
                if(out.length() >=10) {
                    string result(out, 0, 10);
                    return result;
                }
			}
            return out;
		}
    public:
        class Node
        {
        private:
            E data;
            Node *pLeft, *pRight;
            BalanceValue balance;
            friend class HuffTree<E>;

        public:
            Node(E value) : data(value), pLeft(NULL), pRight(NULL), balance(EH) {}
            ~Node() {}
        };

        int getHeightRec(Node *node){
            if (node == NULL)
                return 0;
            int lh = this->getHeightRec(node->pLeft);
            int rh = this->getHeightRec(node->pRight);
            return (lh > rh ? lh : rh) + 1;
        }

        // rotate func
        Node *rotateLeft(Node *subroot){
            Node *upNode = subroot->pRight;
            subroot->pRight = upNode->pLeft;
            upNode->pLeft = subroot;
            return upNode;
        };
        Node *rotateRight(Node *subroot){
            Node *upNode = subroot->pLeft;
            subroot->pLeft = upNode->pRight;
            upNode->pRight = subroot;
            return upNode;
        };
        
        HuffNode<E>* balanceTree(HuffNode<E>* root) {}

        // // insert func
        // bool righTallerBalance(Node *&node){}
        // bool leftTallerBalance(Node *&node){}
        // bool insert(Node *&node, const E &value){}
        // void insert(const E &value){}
        
        // //remove func
        // bool rightShorterBalance(Node *&node){}
        // bool leftShorterBalance(Node *&node){}
        // bool remove(Node *&node, const E &value){}
        // void remove(const E &value){}

};

template <typename E>
class minTreeComp {
public:
    bool operator()(HuffTree<E>* tree1, HuffTree<E>* tree2) {
        if(tree1->weight() == tree2->weight()){
            return tree1->time > tree2->time;
        }
        return tree1->weight() > tree2->weight(); // So sánh theo trọng số, giả sử ưu tiên cây có trọng số nhỏ hơn
    }
};

template <typename E>
HuffTree<E>* buildHuff( string na, map<char, int>& freq) {
    priority_queue<HuffTree<E>*, vector<HuffTree<E>*>, minTreeComp<E>> forest;

    // Bước 1: Tạo một cây cho mỗi ký tự trong chuỗi 'na'
    int len = na.length();
    int i = 0; 
    for (char ch : na) {
        HuffTree<E>* tree = new HuffTree<E>(ch, freq[ch], i++); // Tạo một cây Huffman với nút lá này và thêm vào hàng đợi
        forest.push(tree);
    }
    
    // Bước 2: Xây dựng cây Huffman từ các cây con
    while (forest.size() > 1) {
        HuffTree<E>* temp1 = forest.top();
        forest.pop();
        HuffTree<E>* temp2 = forest.top();
        forest.pop();

        // Thêm cân bằng cây Huffman dùng recursion
        //internalNode = balanceTree(internalNode);

        // Tạo một cây mới với nút nội và thêm vào hàng đợi
        HuffTree<E>* newTree = new HuffTree<E>(temp1, temp2, i++);
        forest.push(newTree);
    }
    // Bước 3: Cây Huffman cuối cùng là cây duy nhất còn lại trong hàng đợi
    HuffTree<E>* huffmanTree = forest.top();
    return huffmanTree;
}


int main() {
    string na = "Efemo";
    std::map<char, int> freq;
    freq.insert({'E', 3});
    freq.insert({'e', 5});
    freq.insert({'f', 4});
    freq.insert({'m', 8});
    freq.insert({'o', 4});
    HuffTree<char>* huffTree = buildHuff<char>(na, freq);
    huffTree->travese();
    cout<<"bin encypt: "<<huffTree->binEncypt(na)<<endl;
    return 0;
}