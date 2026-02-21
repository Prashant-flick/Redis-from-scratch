#include<bits/stdc++.h>

using namespace std;

struct A { 
    char a;
    int c;
    char b;
};

int main() {
    // const char *names[] = {"hello", "world", "hehe"};
    // const char **ptr = names;

    // cout << names[0] <<  " " << ptr[0] << endl;

    // char c = 'A';
    // char *p = &c;

    // char **pp = &p;
    // char ***cpp = &pp;
    // cout << &c << " " << p << " " << *pp << " " << **cpp << endl;

    // cout << c << " " << p << " " << pp << endl;
    // cout << c << " " << p <<  " " << *pp << endl;
    // cout << c << " " << *p << " " << **pp << endl;
    // cout << c << " " << *p << " " << **pp << " " << ***cpp << endl;

    int arr[] = {1,2,3};
    cout << *(arr + 1) << " " << arr[1] << " " << 1[arr] << " " << *(1 + arr) << endl;

    cout << sizeof(A) << endl; 
}