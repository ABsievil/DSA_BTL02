#include "main.h"

int MAXSIZE;
/*
NOTE: BST and Heap class will manage Customer class, and Operating class not manage the order of entering Restaurant
We use sort to arrange the order of entering Res most recently in both GRes and SRes
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
};
class HuffmanTree{
	public:
		HuffmanTree(){};
		~HuffmanTree(){};
};
class BST{
	// manage Customer class
};
class HashTable{
	//manage BST
};
class Heap{
	// manage Customer class
	private:
		int timeUseRegion;
};
class Caesar{
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
		char spEncypt(char ch, int shift){
			if (std::isalpha(ch)) {
				char base = std::isupper(ch) ? 'A' : 'a';
				return static_cast<char>((ch - base + shift + 26) % 26 + base);
			} 
			else return ch;  // if ch is not character then ignore
		}
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
};
class GRes{
	private:
		HashTable* hash;
	public:
		GRes(){};
		~GRes(){};
		void G_KOKUSEN(){ //delete Y cus in each of area
			//B1: convert BST tree -> array in order post-order
			//B2: calculate num of permutation(hoán vị) of array 
			//B3: delete Y cus in order FIFO
		}
		void G_LIMITLESS(int num) {
			// print BST tree at num area in order in-order
		}
		void G_PRINT(){}

		/* sp to G_KOKUSEN*/
		string convertToPostOrder(){ return ""; }  //can change type of return = string
		int numOfPermutation(string str) { return -1; }
};
class SRes{
	private:
		Heap* heap;
	public:
		SRes(){};
		~SRes(){};
		void S_KEITEIKEN(int num){
			// delete num cus, num = (1, MAXSIZE) in order FIFO
			//B1: choose area has not been used the longest and has the fewest visitors
			//B2: delete each of cus in order FIFO and print cus infor
		};
		void S_CLEAVE (int num) {
			// print infor of num cus in order LIFO
		};	
		void S_PRINT(){}
};
class Operating {
	private:
		HuffmanTree* huffTree;
		GRes* G;
		SRes* S;
	public:
		Operating() {
			G= new GRes();
			S= new SRes();
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
			//note: name cus must contain from 3 character or more
			if(!isGreaterThan3ch) return ;
			cout<<"name: "<<nameRearranged<<endl;
			map<char, int> freq = ca->convertToMap(nameRearranged);
			string Xlist = ca->encryptCaesar(nameRearranged);
			cout<<"Xlist: "<<Xlist<<endl;
			
			//B2: build Huffman tree and rotate in "Customer class"

			//B3: convert name cus to binary value, after convert bin to dec(Result)
			/* choose Restaurant*/
			//B4: if Result Odd then choose GRes, else choose SRes
			//Note:  Each of Res has Maxsize area, each of area is unlimited cus
			/* choose area*/
			//B5: calculate ID = Result % MAXSIZE + 1

		}
		void HAND(){
			//print value of Huffman tree of cus came most recently in order from top to bottom, from left to right
		}
		void LIGHT() {}
		/* sp to GRes method */
		void KOKUSEN(){ G->G_KOKUSEN(); }
		void LIMITLESS(int num){G->G_LIMITLESS(num);}
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
			cout<<"LAPSE "<<name<<endl;
			r->LAPSE(name);
    	}
        else if(str == "KOKUSEN") 
        {
			cout<<"KOKUSEN\n";
            r->KOKUSEN();
    	}
    	else if(str == "KEITEIKEN") 
    	{
            ss >> num;
			cout<<"KEITEIKEN "<<num<<endl;
			r->KEITEIKEN(stoi(num));
		}
    	else if(str == "HAND") 
    	{
			cout<<"HAND\n";
			r->HAND();
		}
		else if(str == "LIMITLESS") 
		{
			ss >> num;
			cout<<"LIMITLESS "<<num<<endl;
			r->LIMITLESS(stoi(num));
		}
    	else if(str == "CLEAVE")
     	{   	
			ss >> num;
			cout<<"CLEAVE "<<num<<endl;
			r->CLEAVE(stoi(num));
    	}
		else ss >> MAXSIZE;
    }
	delete r;
}