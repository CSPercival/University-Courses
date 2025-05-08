#include<bits/stdc++.h>
#define BOOST cin.tie(0);ios_base::sync_with_stdio(0)

using namespace std;

set<long long> P;

int main(){
    BOOST;
    int n; cin >> n;
    char op;
    long long x;
    while(n--){
        cin >> op >> x;
        if(op == 'I'){
            P.insert(x);
        }
        if(op == 'D'){
            if(P.count(x)){
                P.erase(x);
                cout << "OK\n";
            } else {
                cout << "BRAK\n";
            }
        }
        if(op == 'U'){
            auto it = P.lower_bound(x);
            if(it == P.end()){
                cout << "BRAK\n";
            } else {
                cout << (*it) << "\n";
            }
        }
        if(op == 'L'){
            auto it = P.upper_bound(x);
            if(it == P.begin()){
                cout << "BRAK\n";
            } else {
                it--;
                cout << (*it) << "\n";
            }
        }
    }
}