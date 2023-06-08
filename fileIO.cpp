#include<iostream>
#include<fstream>
#include<iomanip>

using namespace std;

int main() {
    ifstream inputStream;
    inputStream.open("C:\\Users\\gsmin2020\\Desktop\\cpptetbase-1\\input.txt");
    if (inputStream.fail()) {
        cout<<"Fail";
        return 1;
    }
    double n;
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(2);
    cout.width(4);
    while ((inputStream>>n)) {
        
        cout <<setprecision(5) << n << ' ';
    }
    cout<<'\n';
    return 0;
}