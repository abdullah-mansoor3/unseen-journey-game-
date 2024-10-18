#include<iostream>

using namespace std;

int main(){
    time_t t = time(nullptr);

    int i;
    cin>>i;


    cout<<difftime(time(nullptr), t);
}