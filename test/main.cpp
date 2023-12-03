#include <iostream>
using namespace std;

int test() {
    int y = 5;
}

int main() {
    int *r;
    {
        int x = 5;
        r = &x;
    }
    cout << *r ;
    return 0;
}
