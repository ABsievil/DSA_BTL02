#include <iostream>
#include <math.h>
#include <queue>
using namespace std;
#define SEPARATOR "#<ab@17943918#@>#"

enum BalanceValue
{
    LH = -1,
    EH = 0,
    RH = 1
};

void printNSpace(int n)
{
    for (int i = 0; i < n - 1; i++)
        cout << " ";
}

void printInteger(int &n)
{
    cout << n << " ";
}

template <class T>
class AVLTree
{
public:
    class Node;

private:
    Node *root;

protected:
    int getHeightRec(Node *node)
    {
        if (node == NULL)
            return 0;
        int lh = this->getHeightRec(node->pLeft);
        int rh = this->getHeightRec(node->pRight);
        return (lh > rh ? lh : rh) + 1;
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() {}

    class Node
    {
    private:
        T data;
        Node *pLeft, *pRight;
        BalanceValue balance;
        friend class AVLTree<T>;

    public:
        Node(T value) : data(value), pLeft(NULL), pRight(NULL), balance(EH) {}
        ~Node() {}
    };

    Node *minValueNode(Node *node)
    {
        while (node->pLeft)
            node = node->pLeft;
        return node;
    }

    Node *maxValueNode(Node *node)
    {
        while (node->pRight)
            node = node->pRight;
        return node;
    }

    int getHeight()
    {
        return this->getHeightRec(this->root);
    }

    void printTreeStructure()
    {
        int height = this->getHeight();
        if (this->root == NULL)
        {
            cout << "NULL\n";
            return;
        }
        queue<Node *> q;
        q.push(root);
        Node *temp;
        int count = 0;
        int maxNode = 1;
        int level = 0;
        int space = pow(2, height);
        printNSpace(space / 2);
        while (!q.empty())
        {
            temp = q.front();
            q.pop();
            if (temp == NULL)
            {
                cout << " ";
                q.push(NULL);
                q.push(NULL);
            }
            else
            {
                cout << temp->data;
                switch (temp->balance)
                {
                case EH:
                    cout << "e";
                    break;
                case LH:
                    cout << "l";
                    break;
                case RH:
                    cout << "r";
                    break;
                }
                q.push(temp->pLeft);
                q.push(temp->pRight);
            }
            printNSpace(space);
            count++;
            if (count == maxNode)
            {
                cout << endl;
                count = 0;
                maxNode *= 2;
                level++;
                space /= 2;
                printNSpace(space / 2);
            }
            if (level == height)
                return;
        }
    }

    Node *rotateLeft(Node *subroot)
    {
        Node *upNode = subroot->pRight;
        subroot->pRight = upNode->pLeft;
        upNode->pLeft = subroot;
        return upNode;
    };

    Node *rotateRight(Node *subroot)
    {
        Node *upNode = subroot->pLeft;
        subroot->pLeft = upNode->pRight;
        upNode->pRight = subroot;
        return upNode;
    };

    bool righTallerBalance(Node *&node)
    {
        switch (node->balance)
        {
        case LH:
            node->balance = EH;
            return false;
        case EH:
            node->balance = RH;
            return true;
        case RH:
            switch (node->pRight->balance)
            {
            case RH:
                node->balance = EH;
                node->pRight->balance = EH;
                node = rotateLeft(node);
                break;
            case EH:
                cout << "detect error!!";
                break;
            case LH:
                Node *subTree = node->pRight->pLeft;
                switch (subTree->balance)
                {
                case EH:
                    node->balance = EH;
                    node->pRight->balance = EH;
                    break;
                case LH:
                    node->balance = EH;
                    node->pRight->balance = RH;
                    break;
                case RH:
                    node->balance = LH;
                    node->pRight->balance = EH;
                    break;
                }
                subTree->balance = EH;
                node->pRight = rotateRight(node->pRight);
                node = rotateLeft(node);
                break;
            }
            return false;
        }
        return false;
    }

    bool leftTallerBalance(Node *&node)
    {
        switch (node->balance)
        {
        case RH:
            node->balance = EH;
            return false;
        case EH:
            node->balance = LH;
            return true;
            break;
        case LH:
            switch (node->pLeft->balance)
            {
            case LH:
                node->balance = EH;
                node->pLeft->balance = EH;
                node = rotateRight(node);
                break;
            case EH:
                cout << "detect error!!";
                break;
            case RH:
                Node *subTree = node->pLeft->pRight;
                switch (subTree->balance)
                {
                case EH:
                    node->balance = EH;
                    node->pLeft->balance = EH;
                    break;
                case RH:
                    node->balance = EH;
                    node->pLeft->balance = LH;
                    break;
                case LH:
                    node->balance = RH;
                    node->pLeft->balance = EH;
                    break;
                }
                subTree->balance = EH;
                node->pLeft = rotateLeft(node->pLeft);
                node = rotateRight(node);
                break;
            }
            return false;
        }
        return false;
    }

    bool insert(Node *&node, const T &value)
    {
        if (!node){
            node = new Node(value);
            return true;
        }
        if (value < node->data){
            bool taller = insert(node->pLeft, value);
            if (taller) return leftTallerBalance(node);
        }
        else{
            bool taller = insert(node->pRight, value);
            if (taller) return righTallerBalance(node);
        }
        return false;
    }

    void insert(const T &value)
    {
        insert(root, value);
    }

    bool rightShorterBalance(Node *&node)
    {
        switch (node->balance)
        {
        case RH:
            node->balance = EH;
            return true;
        case EH:
            node->balance = LH;
            return false;
        case LH:
            switch (node->pLeft->balance)
            {
            case LH:
                node->balance = EH;
                node->pLeft->balance = EH;
                node = rotateRight(node);
                return true;
            case EH:
                node->pLeft->balance = RH;
                node = rotateRight(node);
                return false;
            case RH:
                Node *subTree = node->pLeft->pRight;
                switch (subTree->balance)
                {
                case LH:
                    node->balance = EH;
                    node->pLeft->balance = RH;
                    break;
                case EH:
                    node->balance = EH;
                    node->pLeft->balance = EH;
                    break;
                case RH:
                    node->balance = LH;
                    node->pLeft->balance = EH;
                    break;
                }
                subTree->balance = EH;
                node->pLeft = rotateLeft(node->pLeft);
                node = rotateRight(node);
                return true;
            }
        }
        return false;
    }

    bool leftShorterBalance(Node *&node)
    {
        switch (node->balance)
        {
        case LH:
            node->balance = EH;
            return true;
        case EH:
            node->balance = RH;
            return false;
        case RH:
            switch (node->pRight->balance)
            {
            case RH:
                node->balance = EH;
                node->pRight->balance = EH;
                node = rotateLeft(node);
                return true;
            case EH:
                node->pRight->balance = LH;
                node = rotateLeft(node);
                return false;
            case LH:
                Node *subTree = node->pRight->pLeft;
                switch (subTree->balance)
                {
                case RH:
                    node->balance = LH;
                    node->pRight->balance = EH;
                    break;
                case EH:
                    node->balance = EH;
                    node->pRight->balance = EH;
                    break;
                case LH:
                    node->balance = EH;
                    node->pRight->balance = RH;
                    break;
                }
                subTree->balance = EH;
                node->pRight = rotateRight(node->pRight);
                node = rotateLeft(node);
                return true;
            }
        }
        return false;
    }

    bool remove(Node *&node, const T &value)
    {
        if (!node)
            return false;
        if (value < node->data)
        {
            bool shorter = remove(node->pLeft, value);
            if (shorter)
                return leftShorterBalance(node);
        }
        else if (value > node->data)
        {
            bool shorter = remove(node->pRight, value);
            if (shorter)
                return rightShorterBalance(node);
        }
        else
        {
            if (!node->pLeft)
            {
                Node *rightTree = node->pRight;
                delete node;
                node = rightTree;
                return true;
            }
            else if (!node->pRight)
            {
                Node *leftTree = node->pLeft;
                delete node;
                node = leftTree;
                return true;
            }
            else
            {
                Node *maxOfLeftTree = maxValueNode(node->pLeft);
                node->data = maxOfLeftTree->data;
                bool shorter = remove(node->pLeft, node->data);
                if (shorter)
                    return leftShorterBalance(node);
            }
        }
        return false;
    }

    void remove(const T &value)
    {
        remove(root, value);
    }

    int getBalance(Node *subroot)
    {
        if (!subroot)
            return 0;
        return getHeightRec(subroot->pLeft) - getHeightRec(subroot->pRight);
    }

    void inOrder(Node *node)
    {
        if (!node)
            return;
        inOrder(node->pLeft);
        cout << node->data << " ";
        inOrder(node->pRight);
    }

    void printInorder()
    {
        inOrder(root);
    }

    bool search(Node *node, const T &value)
    {
        if (!node)
            return 0;
        if (node->data == value)
            return 1;
        return search(node->pLeft, value) || search(node->pRight, value);
    }

    bool search(const T &value)
    {
        return search(root, value);
    }
};

int main()
{
    AVLTree<int> avl;
    // int arr[] = {20, 10, 40, 5, 7, 42, 2, 6, 15, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int arr[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    for (int i = 0; i < 19; i++)
    {
        avl.insert(arr[i]);
        avl.printTreeStructure();
        cout << endl;
    }
    // avl.printTreeStructure();
    // avl.remove(40);
    // avl.printTreeStructure();
    // avl.remove(20);
    // avl.printTreeStructure();
    // avl.remove(6);
    // avl.printTreeStructure();
    // avl.remove(10);
    // avl.printTreeStructure();
    // avl.remove(42);
    // avl.printTreeStructure();
    // avl.remove(15);
    // avl.printTreeStructure();
}