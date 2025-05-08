#include<bits/stdc++.h>
#define BOOST cin.tie(0);ios_base::sync_with_stdio(false)

using namespace std;

string pattern[210];
string text[2010];

long long alfa = 31;
long long mod = 1e9 + 9;

long long hs[2010];
vector<pair<int,int>> pomatch;

bool check(pair<int,int> start, int a, int b){
    int si = start.first;
    int sj = start.second;
    for(int i = 0; i < a; i++){
        for(int j = 0; j < b; j++){
            if(pattern[i][j] != text[i + si][j + sj]) return false;
        }
    }
    return true;
}

int main(){
    BOOST;
    int a,b,c,d;
    cin >> a >> b >> c >> d;
    for(int i = 0; i < a; i++){
        cin >> pattern[i];
    }
    for(int i = 0; i < c; i++){
        cin >> text[i];
    }

    long long phash = 0;
    for(int i = 0; i < a; i++){
        for(int j = 0; j < b; j++){
            phash *= alfa;
            phash += pattern[i][j] - '@';
            phash %= mod;
        }
    }
    long long a1balfa = 1;
    long long balfa = 1;
    long long b1alfa = 1;
    for(int i = 0; i < (a - 1) * b; i++) a1balfa = (a1balfa * alfa) % mod;
    for(int i = 0; i < b - 1; i++) b1alfa = (b1alfa * alfa) % mod;
    balfa = (b1alfa * alfa) % mod;


    for(int i = 0; i < a; i++){
        for(int j = 0; j < b; j++){
            hs[0] *= alfa;
            hs[0] += text[i][j] - '@';
            hs[0] %= mod;
        }
    }
    long long lhash = 0;
    long long rhash = 0;
    for(int i = 1; i <= d - b; i++){
        lhash = 0;
        rhash = 0;
        for(int j = 0; j < a; j++){
            lhash *= balfa;
            lhash += text[j][i - 1] - '@';
            lhash %= mod;
            
            rhash *= balfa;
            rhash += text[j][i - 1 + b] - '@';
            rhash %= mod;
        }
        hs[i] = hs[i - 1] - lhash * b1alfa;
        hs[i] %= mod;
        hs[i] += (hs[i] < 0 ? mod : 0);
        hs[i] *= alfa;
        hs[i] += rhash;
        hs[i] %= mod;
    }
    for(int i = 0; i <= d - b; i++){
        if(phash == hs[i]) pomatch.push_back({0,i});
    }

    long long uhash = 0;
    long long dhash = 0;
    for(int i = 1; i <= c - a; i++){
        uhash = 0;
        dhash = 0;
        for(int j = 0; j < b; j++){
            uhash *= alfa;
            uhash += text[i - 1][j] - '@';
            uhash %= mod;
            
            dhash *= alfa;
            dhash += text[i - 1 + a][j] - '@';
            dhash %= mod;
        }
        // cout << uhash << " hh " << dhash << endl;
        // for(int j = 0; j <= d - b; j++){
        //     cout << hs[j] << " ";
        // }
        // cout << endl;
        for(int j = 0; j <= d - b; j++){
            hs[j] -= uhash * a1balfa;
            hs[j] %= mod;
            hs[j] += (hs[j] < 0 ? mod : 0);
            hs[j] *= balfa; 
            hs[j] += dhash;
            hs[j] %= mod;

            if(phash == hs[j]) pomatch.push_back({i,j});

            uhash -= (text[i - 1][j] - '@') * b1alfa;
            uhash %= mod;
            uhash += (uhash < 0 ? mod : 0);
            uhash *= alfa;
            uhash += (text[i - 1][j + b] - '@');
            uhash %= mod;

            dhash -= (text[i - 1 + a][j] - '@') * b1alfa;
            dhash %= mod;
            dhash += (dhash < 0 ? mod : 0);
            dhash *= alfa;
            dhash += text[i - 1 + a][j + b] -'@';
            dhash %= mod;
        }
    }

    int ans = 0;
    // cout << pomatch.size() << "\n";
    for(auto i : pomatch){
        // cout << i.first << " " << i.second << "\n";
        if(check(i,a,b)) ans++;
    }
    cout << ans << "\n";
    return 0;
}