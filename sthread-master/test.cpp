#include <iostream>
#include<bits/stdc++.h>
#include<string>
using namespace std;

void disp(int s)
{
    cout<<"hi"<<endl;
}
int disp(int s)
{
    cout<<"hi"<<endl;
}
int disp(string s)
{
    cout<<"hi "<<s<<endl;
    return 0;
}
int main()
{
    disp(50);
    int a=disp("aman");
    return 0;
}
