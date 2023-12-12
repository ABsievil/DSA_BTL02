// C++ program to implement
// the above approach

#include <bits/stdc++.h>
using namespace std;

// Function to precompute the
// factorial of 1 to N
void calculateFact(int fact[], int N)
{
	fact[0] = 1;
	for (long long int i = 1; i < N; i++) {
		fact[i] = fact[i - 1] * i;
	}
}

// Function to get the value of nCr
int nCr(int fact[], int N, int R)
{
	if (R > N)
		return 0;

	// nCr= fact(n)/(fact(r)*fact(n-r))
	int res = fact[N] / fact[R];
	res /= fact[N - R];

	return res;
}

// Function to count the number of ways
// to rearrange the array to obtain same BST
int countWays(vector<int>& arr, int fact[])
{
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

// Driver Code
int main()
{

	vector<int> arr;
	arr = {3};
    vector<int> arr2 = { 5,2, 2 , 6};

	// Store the size of arr
	int N = arr.size();
int N2 = arr2.size();
	// Store the factorial up to N
	int fact[N];
int fact2[N2];
	// Precompute the factorial up to N
	calculateFact(fact, N);
calculateFact(fact2, N2);
	cout << countWays(arr, fact)<<endl;
cout << countWays(arr2, fact2);
	return 0;
}



// nếu des của A là virtual: thì sẽ gọi des của B sau đó gọi des của A
// nếu des của A ko phải là virtual: thì chỉ gọi des của A