#include <iostream>
#include <unordered_map>

using namespace std;

// Định nghĩa lớp Node
class Node {
public:
    int key;
    Node* left;
    Node* right;

    Node(int value) {
        key = value;
        left = right = nullptr;
    }
};

// Định nghĩa lớp BST
class BST {
private:
    Node* root;

    Node* insert(Node* root, int key) {
        if (root == nullptr) {
            return new Node(key);
        }

        if (key < root->key) {
            root->left = insert(root->left, key);
        } else if (key >= root->key) {
            root->right = insert(root->right, key);
        }

        return root;
    }

    Node* findMinValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    Node* deleteNode(Node* root, int key) {
        if (root == nullptr) {
            return root;
        }

        if (key < root->key) {
            root->left = deleteNode(root->left, key);
        } else if (key > root->key) {
            root->right = deleteNode(root->right, key);
        } else {
            if (root->left == nullptr) {
                Node* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                Node* temp = root->left;
                delete root;
                return temp;
            }

            Node* temp = findMinValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }

        return root;
    }

    void inOrderTraversal(Node* root) {
        if (root != nullptr) {
            inOrderTraversal(root->left);
            std::cout << root->key << " ";
            inOrderTraversal(root->right);
        }
    }

public:
    BST() {
        root = nullptr;
    }

    void insert(int key) {
        root = insert(root, key);
    }

    void deleteKey(int key) {
        root = deleteNode(root, key);
    }

    void displayInOrder() {
        inOrderTraversal(root);
        std::cout << std::endl;
    }
};

// Định nghĩa lớp quản lý bảng băm của các cây BST
class HashedBSTManager {
private:
    std::unordered_map<int, BST> hashedBSTs;

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
    HashedBSTManager gojoManager;

    // Thêm giá trị vào khu vực 1
    gojoManager.insertValueIntoRegion(1, 50);
    gojoManager.insertValueIntoRegion(1, 30);
    gojoManager.insertValueIntoRegion(1, 70);

    // Hiển thị cây BST của khu vực 1
    gojoManager.displayRegion(1);

    // Xóa giá trị khỏi khu vực 1
    gojoManager.deleteValueFromRegion(1, 30);

    // Hiển thị cây BST của khu vực 1 sau khi xóa
    gojoManager.displayRegion(1);

    return 0;
}