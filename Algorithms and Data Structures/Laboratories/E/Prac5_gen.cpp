#include<bits/stdc++.h>

using namespace std;

set<long long> P;

int RA(int l, int r){
    return (rand() % (r - l + 1)) + l;
}

int main(int argc, char* argv[]){
    cin.tie(0);cout.tie(0);ios_base::sync_with_stdio(0);
    int name = atoi(argv[1]);
    srand(name);
    int n = 1e6; cout << n << "\n";
    char op;
    long long x;
    long long maxxi = 1e5;
    long long minxi = -1e5;
    while(n--){
        switch(RA(1,6)){
            case 1:
            case 2:
                cout << "I " << RA(minxi,maxxi) << "\n";
                break;
            case 3:
            case 4:
                cout << "D " << RA(minxi,maxxi) << "\n";
                break;
            case 5:
                cout << "U " << RA(minxi,maxxi) << "\n";
                break;
            case 6:
                cout << "L " << RA(minxi,maxxi) << "\n";
                break;
        }
    }
}