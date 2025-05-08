#include<iostream>
#include<vector>
#define BOOST cin.tie(0);ios_base::sync_with_stdio(0)
#define st first
#define nd second

using namespace std;

long long infi = (long long)1e18 + 7;
pair<long long, int> dp[110][10010];    // dp[i][j] = {a,b}, a = najtansza komorka o i klawiszach i j pierwszych literkach alfabetu, b = indeks tz. a = dp[i - 1][b] + C(b,j)
long long cost[10010];                  // cost[i] = koszt i pierwszych literek na jednym klawiszu 
long long pre[10010];                   // pre[i] = suma wystapien i pierwszych literek 
int fi[10010];                          // fi[i] = ile razy i-ta ta literka jest w pamflecie

long long C(int i, int j){              // koszt literek (j, i] na jednym klawiszu
    // j = max(0,i - j);
    return cost[i] - cost[j] - j * (pre[i] - pre[j]);
}

void calculate(int k, int L, int R, int optL, int optR){
    if(L > R) return;

    int mid = (L + R)/2;
    long long ans = infi;
    int min_idx = 1e6, max_idx = -1;
    long long tans;
    for(int i = optL; i <= min(optR,mid); i++){
        tans = C(mid,i) + dp[k - 1][i].st;
        if(tans == ans) max_idx = i;
        if(tans < ans){
            ans = tans;
            max_idx = i;
            min_idx = i;
        }
    }

    dp[k][mid] = {ans,min_idx};

    calculate(k, L, mid - 1, optL, max_idx);
    calculate(k, mid + 1, R, max_idx, optR);
}

int main(){
    BOOST;

    int K,L; cin >> K >> L;
    for(int i = 1; i <= L; i++){
        cin >> fi[i];
        pre[i] = pre[i - 1] + fi[i];
    }


    for(int i = 1; i <= L; i++){
        cost[i] = cost[i - 1] + i * fi[i];
        dp[1][i] = {cost[i],0};
    }

    for(int i = 2; i <= K; i++){
        calculate(i,1,L,1,L);
    }

    // for(int i = 0; i <= K; i++){
    //     for(int j = 0; j <= L; j++){
    //         cout << dp[i][j] << " ";
    //     }
    //     cout << "\n";
    // }

    cout << dp[K][L].st << "\n";
    vector<int> tiles(K);
    int idx = L;
    for(int i = K; i > 0; i--){
        tiles[i - 1] = (idx - dp[i][idx].nd);
        idx = dp[i][idx].nd;
    }
    for(int i : tiles){
        cout << i << " ";
    }
    cout << "\n";
    return 0;
}