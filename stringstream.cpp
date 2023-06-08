#include<sstream>
#include<iostream>
#include<string>

using namespace std;

int main() {
    stringstream ss;
    string scores = "Luigi 70 100 90"; 
    ss.str("");
    ss.clear();
    ss<<scores;
    string name;
    ss>>name;
    int score,total=0,count=0,average=0;
    while (ss>>score) {
        ++count;
        total += score;
    }
    cout << name << ':' << total/count << '\n';
    cout<<ss.str()<<endl;
}