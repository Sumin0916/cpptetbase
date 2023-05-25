#include<iostream>
#include<vector>
using namespace std;

//Display 19.1
int main() {
    vector<int> container;

    for (int i = 0; i < 4; i++) container.push_back(i);

    cout << "Here is what is in the container:\n";
    vector<int>::iterator p;
    for (p = container.begin(); p!=container.end(); p++) cout << *p << ' ';
    cout << '\n';
    cout << "Setting entries to 0\n";
    for (p = container.begin(); p!=container.end(); p++) *p = 0;

    cout << "Container now contains:\n";
    for (p = container.begin(); p!=container.end(); p++) cout << *p << ' ';
    cout << '\n';
    return 0;
}