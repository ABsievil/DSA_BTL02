#include<bits/stdc++.h> 
#include<string>
using namespace std;

template<class T>
class BST
{
public:
    class Node;
private:
    Node* root;
public:
    BST() : root(nullptr) {}
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
            std::cout << root->value << " ";
            inOrderTraversal(root->pRight);
        }
    }
    void displayInOrder() {
        inOrderTraversal(root);
        std::cout << std::endl;
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

// Định nghĩa lớp quản lý bảng băm của các cây BST
class HashTable {
private:
    std::unordered_map<int, BST<int>> hashedBSTs;

public:
    // Thêm giá trị vào một khu vực bằng cách sử dụng cây BST tương ứng
    void insertValueIntoRegion(int region, int value) {
        hashedBSTs[region].insert(value);
    }

    // Xóa giá trị khỏi một khu vực bằng cách sử dụng cây BST tương ứng
    void deleteValueFromRegion(int region, int value) {
        hashedBSTs[region].deleteKey(value);
    }

    // Hiển thị toàn bộ cây BST của một khu vực
    void displayRegion(int region) {
        std::cout << "Region " << region << " - BST in-order: ";
        hashedBSTs[region].displayInOrder();
    }
};

int main() {
    HashTable gojoManager;

    // Thêm giá trị vào khu vực 1
    gojoManager.insertValueIntoRegion(1, 50);
    gojoManager.insertValueIntoRegion(1, 30);
    gojoManager.insertValueIntoRegion(1, 70);
    gojoManager.insertValueIntoRegion(3, 75);
    gojoManager.insertValueIntoRegion(3, 40);
    gojoManager.insertValueIntoRegion(2, 8);
    gojoManager.insertValueIntoRegion(2, 15);

    // Hiển thị cây BST của khu vực 1
    gojoManager.displayRegion(1);

    // Xóa giá trị khỏi khu vực 1
    gojoManager.deleteValueFromRegion(1, 30);

    // Hiển thị cây BST của khu vực 1 sau khi xóa
    gojoManager.displayRegion(1);
    gojoManager.displayRegion(3);
    gojoManager.displayRegion(2);

    return 0;
}