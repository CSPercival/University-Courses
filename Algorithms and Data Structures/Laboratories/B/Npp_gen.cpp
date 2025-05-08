#include<bits/stdc++.h>
#define st first
#define nd second
#define x first
#define y second
using namespace std;

int RA(int l, int r){
    return (rand() % (r - l + 1)) + l;
}


int main(int argc, char* argv[]){
	int name = atoi(argv[1]);
    srand(name);
    int n = RA(3,500000);
    cout << n << "\n";
    for(int i = 0; i < n; i++){
        cout << RA(0,20000000) << " " << RA(0, 20000000) << "\n";
    }

}