#include "main.h"

int MAXSIZE;
/*
NOTE: BST and Heap class will manage Customer class, and Operating class not manage the order of entering Restaurant
We use sort to arrange the order of entering Res most recently in both GRes and SRes
update 03.12: recently finished HuffTree, need rotate as AVLTree
mini task: implement KOKUSEN and HAND func, more than is implement Heap class
update 05.12: task unfinish are: rotate HuffTree, permutation of G_KOKUSEN, HEAP class(build heap &&KEITEIKEN, CLEAVE)
update 09.12: heap finished, no memory leak, please build rotate HuffTree
update 10.12: is rotating, task: 2 subtree is balance, but tree unbalance, how to fix? what is this case?
*/
class Customer{
	private:
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
		void set(int res, int ti){
			this->result = res;
			this->timeIn = ti;
		}
		int getResult(){
			return this->result;
		}
		int getTimeIn(){
			return this->timeIn;
		}
};

/*============== BUILD HuffTree ============*/
enum BalanceValue
{
	LL = -2,
    LH = -1,
    EH = 0,
    RH = 1,
	RR = 2
};  
template<typename E> 
class HuffNode {
    protected:
		E it;
		int wgt;
		int time;
		BalanceValue balance;
		HuffNode<E>* lc; //left child
        HuffNode<E>* rc; //right child
	public:
        virtual ~HuffNode(){}
        virtual int weight() { return this->wgt;}
		virtual E val() 	 {return this->it;}
		virtual int Time()   { return time;}
		virtual void setTime(int ti) {time = ti;}
        virtual void print() {cout<<it<<wgt;}
		virtual void setLeft(HuffNode<E>* b){ lc = b; }
        virtual void setRight(HuffNode<E>* b) { rc = b; }

        virtual bool isLeaf()= 0;
        virtual HuffNode<E>* left() =0; 
        virtual HuffNode<E>* right() =0;

	public: /* Rotate method */
		int getHeight(HuffNode<E>* r){
			if (r == NULL)
                return 0;
            int lh = this->getHeight(r->left());
            int rh = this->getHeight(r->right());
            return (lh > rh ? lh : rh) + 1;
		}
		int getMinHeight(HuffNode<E>* r){
			if (r == NULL)
                return 0;
            int lh = this->getHeight(r->left());
            int rh = this->getHeight(r->right());
            return (lh > rh ? rh : lh) + 1;
		}
        HuffNode<E> *rotateLeft(HuffNode<E> *oldRoot){
			if(!oldRoot) return nullptr;
			if(!oldRoot->rc) return oldRoot;
            HuffNode<E> *newRoot = oldRoot->right();
            oldRoot->rc = newRoot->left();
            newRoot->lc = oldRoot;
            return newRoot;
        }
        HuffNode<E> *rotateRight(HuffNode<E> *oldRoot){
			if(!oldRoot) return nullptr;
			if(!oldRoot->lc) return oldRoot;
            HuffNode<E> *newRoot = oldRoot->left();
            oldRoot->lc = newRoot->right();
            newRoot->rc = oldRoot;
            return newRoot;
        }
        
		int getBalance(HuffNode<E> *subroot){
			if (!subroot) return 0;
			return getHeight(subroot->left()) - getHeight(subroot->right());
		}
		void setBalance(int key){
			if(key ==0) this->balance = EH;
			else if(key == -1) this->balance = RH;
			else if(key == 1) this->balance = LH;
			else if(key > 1) this->balance = LL;
			else this->balance = RR;
		}
		HuffNode<E>* balanceRight(HuffNode<E>* root){
			if(root == nullptr) return nullptr;
			int key = getBalance(root);
			if(key >=-1 && key <=1) return root;
			root = rotateRight(root);
			key = getBalance(root);
			int keyR = getBalance(root->rc);
			if(key >=-1 && key <=1 && keyR >=-1 && keyR <=1) return root;
			root->rc = balanceRight(root->right());
			return root;
		}
		HuffNode<E>* balanceLeft(HuffNode<E>* root){
			if(root == nullptr) return nullptr;
			int key = getBalance(root);
			if(key >=-1 && key <=1) return root;
			root = rotateLeft(root);
			key = getBalance(root);
			int keyL = getBalance(root->lc);
			if(key >=-1 && key <=1 && keyL >=-1 && keyL <=1) return root;
			root->rc = balanceLeft(root->left());
			return root;
		}
		void setBalanceValueAllNode(HuffNode<E> * root){
			if(root == nullptr) return ;
			if(root->rc) setBalanceValueAllNode(root->rc);
			if(root->lc) setBalanceValueAllNode(root->lc);
			int key = getBalance(root);
			root->setBalance(key);
		}
		HuffNode<E>* balanceTree(HuffNode<E>* root) {
			if(root == nullptr) return nullptr;
			int key = getBalance(root);
			root->setBalance(key);    // set temporary balance for root
			// B1: check subTree is perfect tree
			int keyL = getBalance(root->left());
			int keyR = getBalance(root->right());
			if(keyL == 0 && keyR ==0) {
				if(key >=-1 && key <=1) return root;  // temporary balance is official balance
				int HLeftTree = this->getHeight(root->left());
				int HRightTree = this->getHeight(root->right());
				
				if(HLeftTree > HRightTree) {
					HuffNode<E>* newRoot = this->rotateRight(root);
					newRoot->rc = balanceRight(newRoot->rc);
					int newBalance = getBalance(newRoot);
					newRoot->setBalance(newBalance);
					setBalanceValueAllNode(newRoot);
					key = getBalance(newRoot);
					if(key >= -1 && key <=1) return newRoot;
					else return balanceTree(newRoot);
				}
				else{
					HuffNode<E>* newRoot = this->rotateLeft(root);
					newRoot->lc = balanceLeft(newRoot->lc);
					int newBalance = getBalance(newRoot);
					newRoot->setBalance(newBalance);
					key = getBalance(newRoot);
					if(key >= -1 && key <=1) return newRoot;
					else return balanceTree(newRoot);
				}
			}
			else if(keyL ==0) { 
				int HLeftTree = this->getHeight(root->left());
				int HRightTree = this->getHeight(root->right());
				int HRightMinTree = this->getMinHeight(root->right());
				if(key >=-1 && key <=1) { //check if tree is balance
					if(std::abs(HLeftTree - HRightTree) <= 1 && std::abs(HLeftTree - HRightMinTree) <= 1)
						return root;  // temporary balance is official balance
				}

				if(HLeftTree > HRightTree) {
					HuffNode<E>* newRoot = this->rotateRight(root);
					newRoot->rc = balanceRight(newRoot->rc);
					int newBalance = getBalance(newRoot);
					newRoot->setBalance(newBalance);
					setBalanceValueAllNode(newRoot);
					key = getBalance(newRoot);
					if(key >= -1 && key <=1) return newRoot;
					else return balanceTree(newRoot);
				}
				else{
					HuffNode<E>* newRoot = this->rotateLeft(root);
					newRoot->lc = balanceLeft(newRoot->lc);
					int newBalance = getBalance(newRoot);
					newRoot->setBalance(newBalance);
					setBalanceValueAllNode(newRoot);
					key = getBalance(newRoot);
					if(key >= -1 && key <=1) return newRoot;
					else return balanceTree(newRoot);
				}
			}
			else if(keyR ==0){ 
				int HLeftTree = this->getHeight(root->left());
				int HLeftMinTree = this->getMinHeight(root->left());
				int HRightTree = this->getHeight(root->right());
				if(key >=-1 && key <=1) { //check if tree is balance
					if(std::abs(HLeftTree - HRightTree) <= 1 && std::abs(HLeftMinTree - HRightTree) <= 1)
						return root;  // temporary balance is official balance
				}

				if(HLeftTree > HRightTree) {
					HuffNode<E>* newRoot = this->rotateRight(root);
					newRoot->rc = balanceRight(newRoot->rc);
					int newBalance = getBalance(newRoot);
					newRoot->setBalance(newBalance);
					setBalanceValueAllNode(newRoot);
					key = getBalance(newRoot);
					if(key >= -1 && key <=1) return newRoot;
					else return balanceTree(newRoot);
				}
				else{
					HuffNode<E>* newRoot = this->rotateLeft(root);
					newRoot->lc = balanceLeft(newRoot->lc);
					int newBalance = getBalance(newRoot);
					newRoot->setBalance(newBalance);
					setBalanceValueAllNode(newRoot);
					key = getBalance(newRoot);
					if(key >= -1 && key <=1) return newRoot;
					else return balanceTree(newRoot);
				}
			}
			//B2: check case 2 subTree is balance and tree balance

			// B3: check case normal
			// Left Left Case  
			if(key >= 1 && keyL >=1)
				return rotateRight(root);

			// Right Right Case  
			if(key <= -1 && keyR <= -1)
				return rotateLeft(root);

			// Left Right Case  
			if(key >= 1 && keyL <= -1){
				root->lc = rotateLeft(root->lc);
				return rotateRight(root);
			}

			// Right Left Case  
			if(key <= -1 && keyR >= 1){
				root->rc = rotateRight(root->rc);
				return rotateLeft(root);
			}
			return root;
			throw out_of_range("detect error! rotate unexpected");
		}
};
template<typename E>
class LeafNode: public HuffNode<E>{
    public:
       LeafNode(const E& val, int freq, int ti){
			this->it = val;
			this->wgt = freq;
			this->time = ti;
			this->balance = EH;
			this->lc = nullptr;
			this->rc = nullptr;
	   }
	   ~LeafNode(){}

       bool isLeaf() { return true; } 
       HuffNode<E>* left() { return nullptr;}
       HuffNode<E>* right() { return nullptr;}
	   void setLeft(HuffNode<E>* b) override {}
	   void setRight(HuffNode<E>* b) override {}
};
template<typename E>
class IntlNode: public HuffNode<E>{
    public:
        IntlNode(HuffNode<E> *l, HuffNode<E> *r, int ti){
            this->lc = l; 
			this->rc = r;
			this->it = '$';
            this->wgt = l->weight() + r->weight();
			this->time = ti;
        }
		~IntlNode(){}

        bool isLeaf(){ return false; }
        HuffNode<E>* left() { return this->lc;}
        HuffNode<E>* right() { return this->rc;}
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
		HuffTree(HuffNode<E>*& r): root(r){}
		~HuffTree(){
			drawTree();
			//system("pause");
			clear(this->root);
		}
		void clear(HuffNode<E>* r){
			if(r == nullptr) return ;
			//cout<<"clear root: "; r->print(); cout<<endl;
			if(r->left()) clear(r->left());
			if(r->right()) clear(r->right());
			delete r;
			r = nullptr;
		}

        HuffNode<E> *Root(){ return root; }
		void setRoot(HuffNode<E> * r){root = r;}
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
			if(int(str.length()) == 1) return "0";
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
			while (!myQ.empty()) {
				for(int i=0 ; i< maxNode; i++){
					temp = myQ.front();
					myQ.pop();
					if(temp == NULL) {
						cout<<"_";
						printNSpace(space);
						continue;
					}
					//temp->print();
					if(temp->isLeaf()){
						// LeafNode<E>* leaf = static_cast<LeafNode<E>*>(temp);
						// string x = ""; x += leaf->val();  x += to_string(leaf->weight());
						// int len = x.length();
						// printNSpace(space - len + 2);
						//cout<<"space: "<<space<<" len: "<<len<<endl;
						temp->print();
						printNSpace(space);
					}
					else {
						// string x = to_string(temp->weight());
						// int len = x.length();
						// printNSpace(space - len + 3);
						cout<<" "; temp->print();
						printNSpace(space);
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
};

template <typename E>
class minTreeComp {
public:
    bool operator()(HuffNode<E>*& tree1, HuffNode<E>*& tree2) {  // func return true ->swap, return false ->nothing
        if(tree1->weight() == tree2->weight()){
            return tree1->Time() > tree2->Time();
        }
        return tree1->weight() > tree2->weight(); // So sánh theo trọng số, giả sử ưu tiên cây có trọng số nhỏ hơn
    }
};
template <typename E>
HuffTree<E>* buildHuff(const string& na, map<char, int>& freq) {
    priority_queue<HuffNode<E>*, vector<HuffNode<E>*>, minTreeComp<E>> forest;

    // Bước 1: Tạo một cây cho mỗi ký tự trong chuỗi 'na'
    int len = na.length();
    int i = 0;  // i = timeIn
    for (char ch : na) {
        HuffNode<E>* tree = new LeafNode<E>(ch, freq[ch], i++); // Tạo một cây Huffman với nút lá này và thêm vào hàng đợi
        forest.push(tree);
    }
    HuffTree<E> * topTree = new HuffTree<E>();
	HuffTree<E>* leftTree = new HuffTree<E>();
	HuffTree<E>* rightTree = new HuffTree<E>();
    // Bước 2: Xây dựng cây Huffman từ các cây con
    while (forest.size() > 1) {
        HuffNode<E>* temp1 = forest.top();
        forest.pop();
        HuffNode<E>* temp2 = forest.top();
        forest.pop();
		cout<<"pull node1: "; temp1->print(); cout<<" node2 :"; temp2->print(); cout<<endl;
		leftTree->setRoot(temp1); leftTree->drawTree();
		rightTree->setRoot(temp2); rightTree->drawTree();
		cout<<"--------------------";
        //Tạo một cây mới với nút nội và thêm vào hàng đợi
        HuffNode<E>* newTree = new IntlNode<E>(temp1, temp2, i++);

        // Thêm cân bằng cây Huffman dùng recursion
        newTree = newTree->balanceTree(newTree);

		topTree->setRoot(newTree);
		topTree->drawTree();
		cout<<"=======================";
		system("pause");
		newTree->setTime(i++);
        forest.push(newTree);
    }
    // Bước 3: Cây Huffman cuối cùng là cây duy nhất còn lại trong hàng đợi
    HuffNode<E>* rootTree = forest.top();
    return new HuffTree<E>(rootTree);
}
/*=========== END CLASS HuffTree ===========*/

/*============ BUILD HashTable =============*/
template<class T>
class BST
{
public:
    class Node;
private:
    Node* root;
public:
    BST() : root(nullptr){}
    ~BST() {
		clear(root);
	}
	void clear(Node*& r){
		if(!r) return ;
		if(r->pLeft) clear(r->pLeft);
		if(r->pRight) clear(r->pRight);
		delete r;
		r = nullptr;
	}
    void insert(T& value){
        root = addRec(root, value);
    }
    Node* addRec(Node*& root, T& value) {
        if (root == nullptr) return new Node(value);
        if (value->getResult() < root->value->getResult()) root->pLeft = addRec(root->pLeft, value);
        else root->pRight = addRec(root->pRight, value);
        return root;
    }

    void deleteKey(int res, int ti) {
        root = deleteNodeRec (root, res, ti); 
    }
    Node* minValueNode(Node*& root) {   //sp to deleteNodeRec func
        Node* current = root;
        while (current && current->pLeft != nullptr)
            current = current->pLeft;
        return current;
    }
    Node* deleteNodeRec(Node*& root, int res, int ti) {
        if (root == nullptr) return root;
        if (res < root->value->getResult()) root->pLeft = deleteNodeRec(root->pLeft, res, ti);
        else if (res > root->value->getResult()) root->pRight = deleteNodeRec(root->pRight, res, ti);
        else { //if result equal, check time
			if(ti != root->value->getTimeIn()) root->pRight = deleteNodeRec(root->pRight, res, ti);
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
				// find temp and save attributes of temp into root, after delete address of old temp
				Node* temp = minValueNode(root->pRight);
				root->value->set(temp->value->getResult(), temp->value->getTimeIn());
				root->pRight = deleteNodeRec(root->pRight, temp->value->getResult(), temp->value->getTimeIn());
			}
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
        ~Node() {
			delete value;
		}
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
	~HashTable(){
		int n = regionManage.size();
		for(int i =0; i<n; i++){
			delete regionManage[i];
		}
		regionManage.clear();
		// auto call Destructor of BST
	}
	void insertToRegion(int region, T& value) {
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
	
	//support for KOKUSEN func
	void deleteFromRegion(int region, int res, int ti) { 
		hashedBSTs[region].deleteKey(res, ti);
	}
	vector<T> listCusAtRegion(int region){  // traverse = post-order and arrange timeIn
 		vector<T> arr = hashedBSTs[region].postOrder();
		// sort in order time ascending
		auto compare = [](Customer*& a, Customer*& b) {
			return a->getTimeIn() < b->getTimeIn();
		};
		std::sort(arr.begin(), arr.end(), compare);

		return arr;
	}
	void deleteAllRegion(){
		//B1: convert BST tree -> array in order post-order
		for(auto reg : regionManage){
			vector<T> listCus = listCusAtRegion(reg->region);

			// cout<<"list Cus in BST and arranged timeIn at Region: "<<reg->region<<endl;
			// for(T x : listCus) {
			// 	cout<<"result: "<<x->getResult()<<" timeIn: "<<x->getTimeIn()<<endl;
			// } 

			vector<std::pair<int, int>> listPair;
			vector<int> arr;   // save result of listCus in BST in order time ascending
			for(T x : listCus) {
				listPair.push_back(std::make_pair(x->getResult(), x->getTimeIn()));
				arr.push_back(x->getResult());
			}
		//B2: calculate num of permutation(hoán vị) of array 
			int N = arr.size();
			int fact[N];
			calculateFact(fact, N);
			int Y = countWays(arr, fact);
			Y %= MAXSIZE;
			//cout<<"Y :"<<Y<<" at Region: "<<reg->region<<endl;

		//B3: delete Y cus in order FIFO
			for(int i =0; i< Y && i <N; i++){
				//cout<<"delete Cus: "<<listPair[i].first<<" timeIn: "<<listPair[i].second<<endl;
				deleteFromRegion(reg->region, listPair[i].first, listPair[i].second);
			}
		}
	}
	// Calculate permutation
	void calculateFact(int fact[], int N){
		fact[0] = 1;
		for (long long int i = 1; i < N; i++) {
			fact[i] = fact[i - 1] * i;
		}
	}
	int nCr(int fact[], int N, int R){ 
		// Function to get the value of nCr
		if (R > N)
			return 0;
	
		// nCr= fact(n)/(fact(r)*fact(n-r))
		int res = fact[N] / fact[R];
		res /= fact[N - R];
	
		return res;
	}
	int countWays(vector<int>& arr, int fact[]){
		// Function to count the number of ways
		// to rearrange the array to obtain same BST

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

	void displayRegion(int region) { 
		bool isAppeared = false;
		for(InfRegion* reg : regionManage){
			if(region == reg->region){
				isAppeared = true;
				break;
			}
		}
		if(!isAppeared) return ;
		cout << "=========Region " << region << " - BST in-order: =========="<<endl;
		hashedBSTs[region].displayInOrder();
		//cout<<"=================END DISPLAY================"<<endl;
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
/*============ END HashTable ===============*/

/*============ BUILD minHeap ===============*/
template<class T>
class Region{
	std::vector<T> cusManange;
	int ID; // ID = what is region number ?
	int time; // time use region recently, min = 0
	int timeStart;
public:
	Region(int id, T val, int ti){
		ID = id;
		cusManange.push_back(val);
		time = timeStart = ti;
	}
	~Region(){
		int n = NUM();
		for(int i =0; i< n; i++){
			T x = cusManange[i];
			delete x;
		}
		cusManange.clear();
	}

	void addCus(T newCus){
		cusManange.push_back(newCus);
	}
	void updateTime(int ti){
		this->time = ti;
	}
	int NUM(){  // current number of cus in region
		return int(cusManange.size());
	}
	int atRegion(){
		return ID;
	}
	int timeInitial(){
		return this->timeStart;
	}
	int timeUseRecently(){
		return this->time;
	}
	void updateTimeUse(int ti){
		this->time = ti;
	}
	void printLIFO(int num){
		if(num > this->NUM()) num = this->NUM();
		int cusCurrent = this->NUM() -1;
		int result;
		for(int i =0; i< num; i++){
			result = cusManange[cusCurrent--]->getResult();
			cout<<this->ID<<"-"<<result<<endl;
		} 
	}
	void printAndDeleteFIFO(int num){
		if(num > this->NUM()) num = this->NUM();
		int result;
		for(int i= 0; i< num; i++){
			result = cusManange[i]->getResult();
			cout<<result<<"-"<<this->ID<<endl;
		}
		for(int i=0 ; i< num; i++){
			T cus = cusManange[0];
			cusManange.erase(cusManange.begin());
			delete cus;
		}
	}
};
template<class T>
class Heap{
	// manage Customer class
	private:
		int timeUseRegion;
		int size;
		vector<Region<T>*> region; // num Region is created, there are heap tree
	public:
		Heap() {
			timeUseRegion = 0;
			size = MAXSIZE;
		}
		~Heap(){
			int n = this->Count();
			for(int i =0; i<n; i++){
				delete region[i];
			}
			region.clear();
		}

		void insertToRegion(int reg, T val){ 
			// Step 1: if region uncreated, then create region
			bool isCreated = false;
			int i = 0; // index of reg in region if region containning reg
			for(Region<T>*& r : region){ 
				if(r->atRegion() == reg){
					isCreated = true;
					r->addCus(val);
					r->updateTime(timeUseRegion++);
					reheapDown(region, this->Count(), i); 
					break;
				}
				else i++;
			}
			if(!isCreated){
				Region<T>* newReg = new Region<T>(reg, val, timeUseRegion++);
				region.push_back(newReg);
				reheapUp(this->Count() -1);
			}
		}
		void reheapUp(int position){  //using when new region is created
			if (position <= 0 || position >= this->Count()) return;
            int parent = (position - 1) / 2;
			if(region[parent]->NUM() > region[position]->NUM()){ 
				swap(region[parent], region[position]);  //is Swap func running?
				reheapUp(parent);
			}
            return;
		}
		void reheapDown(vector<Region<T>*>& minHeap, int numberOfElements, int index) {
            if(index >= numberOfElements || index<0)   return;
            int i= index;
            int smallest_e=0;
                if((2*i + 1) < numberOfElements){
                    int left_e= 2*i +1;        
                    if((2*i +2) < numberOfElements){
                        int right_e= 2*i +2;
                        if(minHeap[left_e]->NUM() < minHeap[right_e]->NUM())    smallest_e= left_e;
                        else if(minHeap[left_e]->NUM() == minHeap[right_e]->NUM()){  
							//choose the child with the smallest num cus
							int timeUseL = minHeap[left_e]->timeUseRecently();
							int timeUseR = minHeap[right_e]->timeUseRecently();
							if(timeUseL < timeUseR) smallest_e = left_e;
							else smallest_e = right_e;
						}
						else   smallest_e= right_e;
                    }
                    else    smallest_e= left_e;
                }
                else    return;
                if(minHeap[i]->NUM() > minHeap[smallest_e]->NUM()){ 
                    swap(minHeap[i], minHeap[smallest_e]);
                    reheapDown(minHeap, numberOfElements, smallest_e);
                }
				else if(minHeap[i]->NUM() == minHeap[smallest_e]->NUM()){ 
					int timeUseP = minHeap[i]->timeUseRecently();
					int timeUseC = minHeap[smallest_e]->timeUseRecently();
					if(timeUseC < timeUseP) swap(minHeap[i], minHeap[smallest_e]);
					reheapDown(minHeap, numberOfElements, smallest_e);
				}

            return;
        }

		// sp for KEITEINEN func
		vector<std::pair<Region<T>*, int>> listRegionSorted(int num){
			vector<std::pair<Region<T>*, int>> list;
			std::queue<int> indices;
			indices.push(0);
			while (!indices.empty()) {
				int current = indices.front();
				indices.pop();

				list.push_back(std::make_pair(region[current], current));
				if(int(list.size()) == num) break;

				int leftChild = 2 * current + 1;
				int rightChild = 2 * current + 2;

				if (leftChild < this->Count()) 
					indices.push(leftChild);

				if (rightChild < this->Count()) 
					indices.push(rightChild);
			} 
			std::sort(list.begin(), list.end(), 
			[](std::pair<Region<T>*, int>& a, std::pair<Region<T>*, int>& b){
				Region<T>* x = a.first;
				Region<T>* y = b.first;
				if(x->NUM() == y->NUM()){
					return x->timeUseRecently() < y->timeUseRecently();
				}
				return x->NUM() < y->NUM();
			});
			return list;
		}
		void removeSubRegFromRegion(Region<T>*& reg, int i){
			Region<T>* lastElement = region[this->Count() -1];
			region[i] = lastElement;
			region.erase(region.begin() + this->Count() -1);
			reheapDown(region, this->Count(), i);
			delete reg; 
			reg = nullptr;
		}
		void updateList(vector<std::pair<Region<T>*, int>>& list, int index){
			//update list.second from element at index to last element
			int n = this->Count();
			for(int i = index; i <n; i++){
				Region<T>* reg = list[i].first;
				for(int k = 0; k <n; k++){
					if(region[k]->timeInitial() == reg->timeInitial()){
						list[i].second = k;
						break;
					}
				}
			}
		}
		void deleteNumRegion(int num){
			if(this->Count() ==0) return ;
			// delete num cus, num = (1, MAXSIZE) in order FIFO
			//B1: choose area has not been used the longest and has the fewest visitors
			vector<std::pair<Region<T>*, int>> list = listRegionSorted(num);

			// cout<<"list region sorted: \n";
			// for(std::pair<Region<T>*, int> li : list){
			// 	Region<T>* reg = li.first;
			// 	cout<<"Region: "<<reg->atRegion()<<" NUM: "<<reg->NUM()<<" ti: "<<reg->timeUseRecently()
			// 	<<" current index: "<<li.second<<endl;
			// }

			//B2: delete each of cus in order FIFO and print cus infor
			for(int i =0; i< int(list.size()); i++){
				Region<T>* reg = list[i].first;
				reg->printAndDeleteFIFO(num);
				reg->updateTime(timeUseRegion++);
				if(reg->NUM() == 0) {
					cout<<"clear subregion: "<<reg->atRegion()<<endl;
					removeSubRegFromRegion(reg, list[i].second);
					updateList(list, i+1);
				} 
			}
		}

		int Count(){  //num region is using
			return int(region.size());
		}
		
		void displayPreOrder(int index, int num) { 
			if(index >= this->Count()) return ;

			cout << "=========Region " << region[index]->atRegion() <<" NUM: "<<region[index]->NUM()
			<< " - minHeap PreOrder: =========="<<endl;
			this->region[index]->printLIFO(num);
			//cout<<" ================END DISPLAY================"<<endl;

			displayPreOrder(2 * index + 1, num); // Left child
       		displayPreOrder(2 * index + 2, num); // Right child
		}        
};
/*============== END minHeap ===============*/

/*============ CLASS RESTAURANT ============*/
class GRes{
	private:
		HashTable<Customer*> *hash;
		int timeInRes;
	public:
		GRes(){
			timeInRes = 0;
			hash = new HashTable<Customer* >();
		};
		~GRes(){
			delete hash;
		};
		
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
		void displayAllRegion(){ //check nhớ xóa
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
		~SRes(){
			delete heap;
		};
		void S_KEITEIKEN(int num){
			heap->deleteNumRegion(num);
		};
		void S_CLEAVE (int num) {
			// print infor of num cus in order LIFO
			heap->displayPreOrder(0, num);
		};	
		void insert(int region, int result){
			Customer* cus = new Customer(result, timeInRes++);
			heap->insertToRegion(region, cus);
		}
};
/*============= END RESTAURANT =============*/

class Caesar{
	private:
		map<char, int> myMap;
	public:
		string rearrangeName(const string& na, bool& isGreaterThan3ch){
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
		
		void encryptCaesar(const string& na){ //function: encypt string and Incremental string
			map<char, int> newMap;
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
					newMap.insert({tempCh, myMap[c]});
				}
			}
			this->myMap = newMap;
		}
		char spEncypt(char ch, int shift){
			if (std::isalpha(ch)) {
				char base = std::isupper(ch) ? 'A' : 'a';
				return static_cast<char>((ch - base + shift + 26) % 26 + base);
			} 
			else return ch;  // if ch is not character then ignore
		}
		string buildMap(const string& na){
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
		string sortByFreq(){
			// Tạo vector từ map để dễ dàng sắp xếp
			std::vector<std::pair<char, int>> charFreqVector(myMap.begin(),myMap.end());
			// Sắp xếp vector theo giá trị freq tăng dần
			std::sort(charFreqVector.begin(), charFreqVector.end(), 
			[](const std::pair<char, int>& a, const std::pair<char, int>& b) {
				// return true-> not swap , return false -> swap
				if(a.second == b.second){
					auto x = a.first;
					auto y = b.first;
					if(islower(x)) x -= 58;
					if(islower(y)) y -=58;
					return x < y;   // ensure character 'z' < 'A' -> not swap
				} 
				return a.second < b.second;
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
		void LAPSE(const string& name){
			//B1: new Caesar and return result
			Caesar* ca = new Caesar();
			bool isGreaterThan3ch = false;
			string nameRearranged = ca->rearrangeName(name, isGreaterThan3ch);
			if(!isGreaterThan3ch) return ;    //note: name cus must contain from 3 character or more

			string listAfterBuildMap = ca->buildMap(nameRearranged);
			ca->encryptCaesar(listAfterBuildMap);
			string XList = ca->sortByFreq();
			map<char, int> freq = ca->getMap();

			// cout<<"XList: "<<XList<<endl;
			// cout<<"list map:\n";
			// for(auto m : freq){
			// 	cout<< m.first <<m.second<<" ";
			// } cout<<endl;

			//B2: build Huffman tree and rotate in "Customer class"
			if(huffTree != nullptr) delete huffTree;    // have to implement HuffTree's Destructor
			huffTree = nullptr;
			this->huffTree = buildHuff<char>(XList, freq);
			string binEncode = huffTree->binEncypt(XList);
			//B3: Calculate Result
			int Result = huffTree->binaryToDecimal(binEncode);
			//B4: choose Restaurant
			int ID = Result % MAXSIZE + 1;  //Note: Each of Res has Maxsize area, each of area is unlimited cus
			//cout<<"ID: "<<ID<<" Result: "<<Result<<endl;
			if(Result% 2 ==0) S->insert(ID, Result);
			else G->insert(ID, Result);

			delete ca;
		}
		void HAND(){
			//print value of Huffman tree of cus came most recently in order from top to bottom, from left to right
			if(!huffTree) return ;
			//huffTree->drawTree();
			huffTree->H_HAND();
		}
		/* sp to GRes method */
		void KOKUSEN(){ G->G_KOKUSEN(); }
		void LIMITLESS(int num){ 
			//G->displayAllRegion(); 
			G->G_LIMITLESS(num);
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