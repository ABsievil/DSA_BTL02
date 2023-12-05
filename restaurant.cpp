#include "main.h"

int MAXSIZE;
/*
NOTE: BST and Heap class will manage Customer class, and Operating class not manage the order of entering Restaurant
We use sort to arrange the order of entering Res most recently in both GRes and SRes
update 03.12: recently finished HuffTree, need rotate as AVLTree
mini task: implement KOKUSEN and HAND func, more than is implement Heap class
update 05.12: task unfinish are: rotate HuffTree, permutation of G_KOKUSEN, HEAP class(build heap &&KEITEIKEN, CLEAVE)
*/
class Customer{
	public:
		int result;
		int timeIn;
	public:
		Customer(int rs = -1, int ti = -1){
			result = rs;
			timeIn = ti;
		}
		~Customer(){}
		void print(){
			cout<<result<<endl;
		}
		int getResult(){
			return this->result;
		}
		int getTimeIn(){
			return this->timeIn;
		}
};

/*=========== BUILD HuffTree ===========*/
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
    public:
        int time;
        class Node;
        // Constructor 
        HuffTree(): root(nullptr){}
        HuffTree(const E& val, int freq, int ti) { root = new LeafNode<E>(val, freq); time = ti; }
        HuffTree(HuffTree<E>* l, HuffTree<E>* r, int ti) { root = new IntlNode<E>(l->Root(), r->Root()); time = ti; }
        ~HuffTree(){}

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
		string binEncypt(string str) {
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
		int binaryToDecimal(string binaryString) {
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
/*=========== END CLASS HuffTree ===========*/

/*=========== BUILD HashTable ===========*/
template<class T>
class BST
{
public:
    class Node;
private:
    Node* root;
public:
    BST() : root(nullptr){}
    ~BST() {}
    void insert(T value){
        root = addRec(root, value);
    }
    Node* addRec(Node* root, T value) {
        if (root == nullptr) return new Node(value);
        if (value < root->value) root->pLeft = addRec(root->pLeft, value);
        else root->pRight = addRec(root->pRight, value);
        return root;
    }

    void deleteKey(T value) {
        root = deleteNodeRec (root, value); 
    }
    Node* minValueNode(Node* root) {   //sp to deleteNodeRec func
        Node* current = root;
        while (current && current->pLeft != nullptr)
            current = current->pLeft;
        return current;
    }
    Node* deleteNodeRec(Node* root, T value) {
        if (root == nullptr) return root;
        if (value < root->value) root->pLeft = deleteNodeRec(root->pLeft, value);
        else if (value > root->value) root->pRight = deleteNodeRec(root->pRight, value);
        else {
            if (root->pLeft == nullptr) {
                Node* temp = root->pRight;
                delete root;
                return temp;
            }
            else if (root->pRight == nullptr) {
                Node* temp = root->pLeft;
                delete root;
                return temp;
            }
            Node* temp = minValueNode(root->pRight);
            root->value = temp->value;
            root->pRight = deleteNodeRec(root->pRight, temp->value);
        }
        return root;
    }    

    void inOrderTraversal(Node* root) {
        if (root != nullptr) {
            inOrderTraversal(root->pLeft);
            root->value->print();
            inOrderTraversal(root->pRight);
        }
    }
    void displayInOrder() {
        inOrderTraversal(root);
    }

	void postOrderTraversal(Node* r, vector<T>& arr){
		if (r != nullptr) {
            postOrderTraversal(r->pLeft, arr);
            postOrderTraversal(r->pRight, arr);
            arr.push_back(r->value);
        }
	}

	vector<T> postOrder(){
		vector<T> arr;
		postOrderTraversal(this->root, arr);
		return arr;
	}
    
    class Node
    {
    private:
        T value;
        Node* pLeft, * pRight;
        friend class BST<T>;
    public:
        Node(T value) : value(value), pLeft(NULL), pRight(NULL) {}
        ~Node() {}
    };
};

template<class T>
class HashTable {
public:
	class InfRegion;
private:
	std::unordered_map<int, BST<T>> hashedBSTs;
	std::vector<InfRegion* > regionManage; 
	int timeUseRegion = 0;
public:
	HashTable(){}
	~HashTable(){}
	void insertToRegion(int region, T value) {
		hashedBSTs[region].insert(value);
		bool isAppeared = false;
		for(auto reg : regionManage){
			if(reg->region == region) {
				reg->timeUseRecently = timeUseRegion++;
				reg->cusNumber += 1;
				isAppeared = true;
				break;
			}
		}
		if(!isAppeared){
			InfRegion* reg = new InfRegion(region, 1, timeUseRegion++);
			regionManage.push_back(reg);
		}
	}
	void deleteFromRegion(int region, T value) { 
		hashedBSTs[region].deleteKey(value);
	}

	bool compareCustomers(Customer* a, Customer* b) {
		return a->getTimeIn() < b->getTimeIn();
	}
	vector<T> listCusAtRegion(int region){  // traverse = post-order and arrange timeIn
 		vector<T> arr = hashedBSTs[region].postOrder();
		// sort in order time ascending
		auto compare = [this](Customer*& a, Customer*& b) {
			return this->compareCustomers(a, b);
		};
		std::sort(arr.begin(), arr.end(), compare);

		return arr;
	}

	/* support for KOKUSEN func */
	void deleteAllRegion(){
		//B1: convert BST tree -> array in order post-order
		for(auto reg : regionManage){
			vector<T> listCus = listCusAtRegion(reg->region);
			cout<<"list Cus in BST and arranged timeIn at Region: "<<reg->region<<endl;
			for(T x : listCus) {
				cout<<"result: "<<x->getResult()<<" timeIn: "<<x->getTimeIn()<<endl;
			} 

		//B2: calculate num of permutation(hoán vị) of array 
			vector<int> arr;   // save result of listCus in BST in order time ascending
			for(T x : listCus){
				arr.push_back(x->getResult());
			}
			int N = arr.size();
			int fact[N];
			calculateFact(fact, N);
			int Y = countWays(arr, fact);
			Y %= MAXSIZE;
			cout<<"Y :"<<Y<<endl;

		//B3: delete Y cus in order FIFO
			for(int i =0; i< Y && i <N; i++){
				cout<<"delete Cus: "<<listCus[i]->getResult()<<endl;
				deleteFromRegion(reg->region, listCus[i]);
			}
		}
	}
	void calculateFact(int fact[], int N){
		fact[0] = 1;
		for (long long int i = 1; i < N; i++) {
			fact[i] = fact[i - 1] * i;
		}
	}
		// Function to get the value of nCr
	int nCr(int fact[], int N, int R){
		if (R > N)
			return 0;
	
		// nCr= fact(n)/(fact(r)*fact(n-r))
		int res = fact[N] / fact[R];
		res /= fact[N - R];
	
		return res;
	}
		// Function to count the number of ways
		// to rearrange the array to obtain same BST
	int countWays(vector<int>& arr, int fact[]){
		// Store the size of the array
		int N = arr.size();
	
		// Base case
		if (N <= 2) {
			return 1;
		}
	
		// Store the elements of the
		// left subtree of BST
		vector<int> leftSubTree;
	
		// Store the elements of the
		// right subtree of BST
		vector<int> rightSubTree;
	
		// Store the root node
		int root = arr[0];
	
		for (int i = 1; i < N; i++) {
	
			// Push all the elements
			// of the left subtree
			if (arr[i] < root) {
				leftSubTree.push_back(
					arr[i]);
			}
	
			// Push all the elements
			// of the right subtree
			else {
				rightSubTree.push_back(
					arr[i]);
			}
		}
	
		// Store the size of leftSubTree
		int N1 = leftSubTree.size();
	
		// Store the size of rightSubTree
		int N2 = rightSubTree.size();
	
		// Recurrence relation
		int countLeft
			= countWays(leftSubTree,
						fact);
		int countRight
			= countWays(rightSubTree,
						fact);
	
		return nCr(fact, N - 1, N1)
			* countLeft * countRight;
	}
	/* end support for KOKUSEN func */

	void displayRegion(int region) { 
		cout << "=========Region " << region << " - BST in-order: =========="<<endl;
		hashedBSTs[region].displayInOrder();
		cout<<" ================END DISPLAY================"<<endl;
	}        
	void displayAllRegion(){
		for(auto reg: regionManage){
			displayRegion(reg->region);
		}
	}
public:
	class InfRegion {
		int region;
		int timeUseRecently; // timeUseRecently large then is use recently
		int cusNumber;
	public:
		InfRegion(int re, int cN, int ti): region(re), cusNumber(cN), timeUseRecently(ti){}
		friend class HashTable;
	};
};
/*=========== END HashTable ===========*/

/*=========== BUILD minHeap ===========*/
template<class T>
class Region{
	std::vector<T> cusManange;
	int number; // Region at number ?
	int time; // time use region recently, min = 0
	int countNumber;
public:
	Region(int n, T val, int ti){
		number = n;
		cusManange.push_back(val);
		time = ti;
		countNumber = 1;
	}
	~Region(){
		for(T c: cusManange){
			delete c;
		}
		cusManange.clear();
	}
	void addCus(T newCus){
		cusManange.push_back(newCus);
		countNumber++;
	}
	int size(){
		return int(cusManange.size()); 
	}
	void updateTime(int ti){
		this->time = ti;
	}
	int regionNumber(){
		return number;
	}
};
template<class T>
struct Compare {
    bool operator()(Region<T>* a, Region<T>* b) {
        return a->size() > b->size();
    }
};

template<class T>
class Heap{
	// manage Customer class
	private:
		int timeUseRegion;
		std::priority_queue<Region<T>*, vector<Region<T>*>, Compare<T>> minHeap;
		vector<Region<T>*> regionManage; // num Region is created
	public:
		Heap(): timeUseRegion(0){}
		~Heap(){}
		void insertToRegion(int region, T val){
			// Step 1: if region uncreated, then create region
			bool isCreated = false;
			for(auto r : regionManage){ 
				if(r->regionNumber() == region){
					r->addCus(val);
					isCreated = true;
					break;
				}
			}
			if(!isCreated){
				Region<T>* reg = new Region<T>(region, val, timeUseRegion++);
				minHeap.push(reg);
				regionManage.push_back(reg);
			}
		}
};
/*=========== END minHeap ===========*/

/*=========== CLASS RESTAURANT ===========*/
class GRes{
	private:
		HashTable<Customer*> *hash;
		int timeInRes;
	public:
		GRes(){
			timeInRes = 0;
			hash = new HashTable<Customer* >();
		};
		~GRes(){};
		void G_KOKUSEN(){ //delete Y cus in each of area
			hash->deleteAllRegion();
		}
		void G_LIMITLESS(int num) {
			// print BST tree at num area in order in-order
			hash->displayRegion(num);
		}

		void insert(int region, int result){
			Customer* cus = new Customer(result, timeInRes++);
			hash->insertToRegion(region, cus);
		}
		void displayAllRegion(){
			hash->displayAllRegion();
		}
};

class SRes{
	private:
		Heap<Customer*> *heap;
		int timeInRes; // min = 0
	public:
		SRes(){
			timeInRes = 0;
			heap = new Heap<Customer* >();
		};
		~SRes(){};
		void S_KEITEIKEN(int num){
			// delete num cus, num = (1, MAXSIZE) in order FIFO
			//B1: choose area has not been used the longest and has the fewest visitors
			//B2: delete each of cus in order FIFO and print cus infor
		};
		void S_CLEAVE (int num) {
			// print infor of num cus in order LIFO
		};	
		
		void insert(int region, int result){
			Customer* cus = new Customer(result, timeInRes++);
			heap->insertToRegion(region, cus);
		}
};
/*=========== END RESTAURANT ===========*/

class Caesar{
	private:
		map<char, int> myMap;
	public:
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
		
		string encryptCaesar(string na){ //function: encypt string and Incremental string
			map<char, int> newMap;
			string out = "";
			for(char c: na){
				char tempCh = spEncypt(c, this->myMap[c]);
				bool isAdded = false;
				for(auto& m : newMap){
					if(m.first == tempCh) {
						m.second += myMap[c];
						isAdded = true;
						break;
					}
				}
				if(!isAdded) {
					out+= tempCh;
					newMap.insert({tempCh, myMap[c]});
				}
			}
			this->myMap = newMap;
			return out;
		}
		char spEncypt(char ch, int shift){
			if (std::isalpha(ch)) {
				char base = std::isupper(ch) ? 'A' : 'a';
				return static_cast<char>((ch - base + shift + 26) % 26 + base);
			} 
			else return ch;  // if ch is not character then ignore
		}
		string buildMap(string na){
			std::map<char, int> list;
			char x ;
			int count =0;
			string out = "";
			for(char c : na){
				if(c == na[0]){
					x = c;
					count++;
				}
				else if(c == x) count++;
				else {
					list.insert({x, count});
					out+= x ;
					x=c; 
					count =1; 
				}
			}   
			list.insert({x, count});
			out += x;
			this->myMap = list; 
			return out;
		}
		std::map<char, int> getMap(){ return myMap; }
		string sortByFreq(string na){
			// Tạo vector từ map để dễ dàng sắp xếp
			std::vector<std::pair<char, int>> charFreqVector(myMap.begin(),myMap.end());
			// Sắp xếp vector theo giá trị freq tăng dần
			std::sort(charFreqVector.begin(), charFreqVector.end(), [](const auto& a, const auto& b) {
				if(a.second == b.second){
					auto x = a.first;
					auto y = b.first;
					if(islower(x)) x -= 58;
					if(islower(y)) y -=58;
					return x < y;
				} 
				return (a.second == b.second) ? a.first < b.first : a.second < b.second;
			});

			// Tạo chuỗi kết quả dựa trên vector đã sắp xếp
			std::string result;
			for (const auto& pair : charFreqVector) {
				result += pair.first;  // Lặp qua giá trị freq và thêm kí tự vào chuỗi
			}
			return result;
		}
};
class Operating {
	private:
		HuffTree<char>* huffTree;
		GRes* G;
		SRes* S;
	public:
		Operating() {
			G= new GRes();
			S= new SRes();
			huffTree = nullptr;
		};
		~Operating() {
			delete G;
			delete S;
			if(huffTree) delete huffTree;
		};
		/* internal method */
		void LAPSE(string name){
			//B1: new Caesar and return result
			Caesar* ca = new Caesar();
			bool isGreaterThan3ch = false;
			string nameRearranged = ca->rearrangeName(name, isGreaterThan3ch);
			if(!isGreaterThan3ch) return ;    //note: name cus must contain from 3 character or more

			string listAfterBuildMap = ca->buildMap(nameRearranged);
			string nameAfterEncypt = ca->encryptCaesar(listAfterBuildMap);
			string XList = ca->sortByFreq(nameAfterEncypt);
			map<char, int> freq = ca->getMap();

			cout<<"XList: "<<XList<<endl;
			cout<<"list map:\n";
			for(auto m : freq){
				cout<< m.first <<m.second<<" ";
			} cout<<endl;

			//B2: build Huffman tree and rotate in "Customer class"
			if(huffTree != nullptr) delete huffTree;    // have to implement HuffTree's Destructor
			huffTree = nullptr;
			this->huffTree = buildHuff<char>(XList, freq);
			string binEncode = huffTree->binEncypt(XList);
			//B3: Calculate Result
			int Result = huffTree->binaryToDecimal(binEncode);
			//B4: choose Restaurant
			int ID = Result % MAXSIZE + 1;  //Note: Each of Res has Maxsize area, each of area is unlimited cus
			cout<<"ID: "<<ID<<" Result: "<<Result<<endl;
			if(Result% 2 ==0) S->insert(ID, Result);
			else G->insert(ID, Result);
		}
		void HAND(){
			//print value of Huffman tree of cus came most recently in order from top to bottom, from left to right
			huffTree->drawTree();
			//huffTree->H_HAND();
		}
		void LIGHT() {}
		/* sp to GRes method */
		void KOKUSEN(){ G->G_KOKUSEN(); }
		void LIMITLESS(int num){ 
			G->displayAllRegion(); 
			//G->G_LIMITLESS(num);
		} 
		/* sp to SRes method */
		void KEITEIKEN(int num){ S->S_KEITEIKEN(num); }
		void CLEAVE (int num){ S->S_CLEAVE(num); }
};

void simulate(string filename)
{
	ifstream ss(filename);
	string str, name, num;
	Operating* r= new Operating();

	while(ss >> str)
	{ 
		if(str == "LAPSE")
		{
			ss >> name;
			cout<<"===LAPSE "<<endl;
			r->LAPSE(name);
    	}
        else if(str == "KOKUSEN") 
        {
			cout<<"===KOKUSEN\n";
            r->KOKUSEN();
    	}
    	else if(str == "KEITEIKEN") 
    	{
            ss >> num;
			cout<<"===KEITEIKEN "<<num<<endl;
			r->KEITEIKEN(stoi(num));
		}
    	else if(str == "HAND") 
    	{
			cout<<"===HAND\n";
			r->HAND();
		}
		else if(str == "LIMITLESS") 
		{
			ss >> num;
			cout<<"===LIMITLESS "<<num<<endl;
			r->LIMITLESS(stoi(num));
		}
    	else if(str == "CLEAVE")
     	{   	
			ss >> num;
			cout<<"===CLEAVE "<<num<<endl;
			r->CLEAVE(stoi(num));
    	}
		else ss >> MAXSIZE;
    }
	delete r;
}