#include<bits/stdc++.h> 
#include<string>
using namespace std;

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
	   ~LeafNode(){}

       int weight() { return wgt;}
       E val() {return it;}
       bool isLeaf() { return true; } 
       HuffNode<E>* left() { return nullptr;}
       HuffNode<E>* right() { return nullptr;}
       void print() {cout<<it<<wgt; }
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
		~IntlNode(){}

        int weight(){ return wgt;}
        bool isLeaf(){ return false; }

        HuffNode<E>* left() { return lc;}
        HuffNode<E>* right() { return rc;}

        void setLeft(HuffNode<E>* b){ lc = (HuffNode<E>*)b; }
        void setRight(HuffNode<E>* b) { rc = (HuffNode<E>)b; }

        void print() {cout<<wgt; }
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
		HuffTree<E>* leftChild;
		HuffTree<E>* rightChild;
    public:
        int time;
        class Node;
        // Constructor 
        HuffTree(): root(nullptr), leftChild(nullptr), rightChild(nullptr){}
        HuffTree(const E& val, int freq, int ti) {  //using for LeafNode
			root = new LeafNode<E>(val, freq); 
			time = ti; 
			leftChild = rightChild = nullptr;
		}
        HuffTree(HuffTree<E>*& l, HuffTree<E>*& r, int ti) { //using for IntlNode
			root = new IntlNode<E>(l->Root(), r->Root()); 
			time = ti; 
			this->leftChild = l;
			this->rightChild = r;
		}
		~HuffTree(){
			if(leftChild) delete leftChild; leftChild = nullptr;
			if(rightChild) delete rightChild; rightChild = nullptr;
			if(root) delete root; root = nullptr;
		}

        HuffNode<E> *Root(){ return root; }
        int weight(){ return root->weight(); }

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
		string binEncypt(const string& str) {
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
			}
			reverse(out.begin(), out.end());
			string result(out, 0 , 10);
            return result;
		}
		int binaryToDecimal(const string& binaryString) {
			int decimalValue = 0;
			int base = 1; // Hệ số cơ số

			for (int i = binaryString.size() - 1; i >= 0; --i, base *= 2) 
				if (binaryString[i] == '1') decimalValue += base;

			return decimalValue;
		}
		// Nhớ xóa 3 hàm dưới này
		int getHeight(HuffNode<E>* r){
			if (r == NULL)
                return 0;
            int lh = this->getHeight(r->left());
            int rh = this->getHeight(r->right());
            return (lh > rh ? lh : rh) + 1;
		}
		void printNSpace(int n)
		{
			for (int i = 0; i < n - 1; i++)
				cout << " ";
		}
		void drawTree(){
			if(root== nullptr) {
				cout<<endl; 
				return ;
			}
			int height = this->getHeight(root);
        	cout << "height: " << height << "(h)\n";
			queue<HuffNode<E>* > q;
			queue<HuffNode<E>* > myQ; 
			q.push(root);
			myQ.push(root);
			HuffNode<E> *temp;
			int numNode = 0;
			for(int i =0 ; i< height; i++){
				numNode += pow(2, i);
			}
			int idrun = 0;
			while (!q.empty())
			{
				temp = q.front();
				q.pop();
				if(!temp){
					q.push(NULL); q.push(NULL);
					myQ.push(NULL); myQ.push(NULL);
				}
				else {
					q.push(temp->left()); q.push(temp->right());
					myQ.push(temp->left()); myQ.push(temp->right());
				}
				idrun++;
				if(idrun == numNode) break;
			}
			int f_space = pow(2, height-1);
			int space = pow(2, height);
			int maxNode = 1;
			int level = 0;
			int keyOnly = height - 1;
			printNSpace(f_space + keyOnly--);
			while (!myQ.empty())
			{
				for(int i=0 ; i< maxNode; i++){
					temp = myQ.front();
					myQ.pop();
					if(temp == NULL) {
						cout<<"_";
						printNSpace(space + 1);
						continue;
					}
					temp->print();
					if(temp->isLeaf()){
						LeafNode<E>* leaf = static_cast<LeafNode<E>*>(temp);
						string x = ""; x += leaf->val();  x += to_string(leaf->weight());
						int len = x.length();
						printNSpace(space - len + 2);
						//cout<<"space: "<<space<<" len: "<<len<<endl;
					}
					else {
						string x = to_string(temp->weight());
						int len = x.length();
						printNSpace(space - len + 3);
					}
				} 
				cout<<endl;
				maxNode *= 2;
				level++;
				space /= 2;
				f_space /=2;
				printNSpace(f_space + keyOnly--);
				if (level == height) return;
			}
		}

		void traverseInOrder(HuffNode<E>* r) const {
            if (r == nullptr) return;
            traverseInOrder(r->left()); // Đệ quy in cây con trái
            if(!r->isLeaf()) cout<<r->weight()<<endl;
            else {
                LeafNode<E> * x = static_cast<LeafNode<E>*>(r);
                cout<<x->val()<<endl;
            } 
            traverseInOrder(r->right()); // Đệ quy in cây con phải
        }
		void H_HAND() const {
			traverseInOrder(root);
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

        int getHeightRec(HuffNode<E>* r){
			if (r == NULL)
                return 0;
            int lh = this->getHeight(r->left());
            int rh = this->getHeight(r->right());
            return (lh > rh ? lh : rh) + 1;
		}

        // rotate func
        HuffNode<E> *rotateLeft(HuffNode<E> *subroot){
            HuffNode<E> *upNode = subroot->right();
            subroot->right() = upNode->left();
            upNode->left() = subroot;
            return upNode;
        };
        HuffNode<E> *rotateRight(HuffNode<E> *subroot){
            HuffNode<E> *upNode = subroot->left();
            subroot->left() = upNode->right();
            upNode->right() = subroot;
            return upNode;
        };
        
        
		int getBalance(HuffNode<E> *subroot){
			if (!subroot) return 0;
			return getHeightRec(subroot->left()) - getHeightRec(subroot->right());
		}

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

		HuffNode<E>* balanceTree(HuffNode<E>* root) {}
};

template <typename E>
class minTreeComp {
public:
    bool operator()(HuffTree<E>*& tree1, HuffTree<E>*& tree2) {  // func return true ->swap, return false ->nothing
        if(tree1->weight() == tree2->weight()){
            return tree1->time > tree2->time;
        }
        return tree1->weight() > tree2->weight(); // So sánh theo trọng số, giả sử ưu tiên cây có trọng số nhỏ hơn
    }
};
template <typename E>
HuffTree<E>* buildHuff(const string& na, map<char, int>& freq) {
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
    huffTree->drawTree();
    cout<<"bin encypt: "<<huffTree->binEncypt(na)<<endl;
    return 0;
}