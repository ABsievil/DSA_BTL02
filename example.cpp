#include <iostream>
#include <string>
using namespace std;
 
int main() {
    string str = "Hello World!";
 
    string substr = str.substr(int(str.length()) - 10);
 
    cout << substr << endl;
    cin.get();
    return 0;
}
