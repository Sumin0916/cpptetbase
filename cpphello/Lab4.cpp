#include<iostream>
#include<vector>
using namespace std;

//Display 19.2
int main() {
    vector<char> container;

    container.push_back('A');
    container.push_back('B');
    container.push_back('C');
    container.push_back('D');

    for (int i = 0; i < 4; i++) cout << "container[" << i << "] == " << container[i] << '\n'; 

    vector<char>::iterator p = container.begin();
    cout << "The third entry is " << container[2] << '\n';
    cout << "The third entry is " << p[2] << '\n';
    cout << "The third entry is " << *(p + 2) << '\n';

    cout << "Back to container[0].\n";
    p = container.begin();
    cout << "which has value" << *p << '\n';

    cout << "Two step forward and one step back:\n";
    p++;
    cout << *p << endl;

    cout << "Using auto\n";
    for (auto i : container) cout << i << ' ';
    cout << '\n';
    for (auto i = container.end()-1; i >= container.begin(); i--) cout << *i << ' ';
    cout << '\n';
    return 0;
}