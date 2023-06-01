#include<iostream>
#include<string>
#include<sstream>
using namespace std;

int main() {
    stringstream ss;
    string scores = "Luigi 70 100 90";
    ss.str("");
    ss.clear();

    ss << scores;

    string name = "";
    int total = 0, count = 0, average = 0;
    int score;
    ss >> name;

    while (ss>>score)
    {
        count++;
        total += score;
    }
    if (count > 0)
    {
        average = total / count;
    }

    ss.clear();
    ss.str("");
    ss << "Name: "<< name << " / Average: " << average;

    cout << ss.str() << endl;
    return 0;
}