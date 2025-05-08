#include<bits/stdc++.h>
#define st first
#define nd second

using namespace std;

struct Coin{
    int val;
    int wei;
    int idx;

    bool operator <(const Coin& a){
        if(wei == a.wei) return idx < a.idx;
        return wei < a.wei;
    }
};

Coin mon[110];

long long infi = (long long)1 << 60;
long long min_val[1000010];
long long max_val[1000010];
int ans_min[110];
int ans_max[110];

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int F; cin >> F;
    int C; cin >> C;
    for(int i = 0; i < C; i++){
        cin >> mon[i].val >> mon[i].wei;
        mon[i].idx = i;
    }
    sort(mon,mon + C);
    for(int i = 0; i <= F; i++){
        min_val[i] = infi;
        max_val[i] = -1;
    }
    min_val[0] = 0;
    max_val[0] = 0;

    for(int i = 0; i < F; i++){
        if(max_val[i] == -1) continue;
        for(int j = 0; j < C; j++){
            if(i + mon[j].wei > F) break;
            min_val[i + mon[j].wei] = min(min_val[i + mon[j].wei], min_val[i] + mon[j].val);
            max_val[i + mon[j].wei] = max(max_val[i + mon[j].wei], max_val[i] + mon[j].val);
        }
    }
    if(max_val[F] == -1){
        cout << "NIE\n";
        return 0;
    }

    int tmp_F = F;
    for(int i = 0; i < C; i++){
        while(tmp_F >= mon[i].wei && min_val[tmp_F] == min_val[tmp_F - mon[i].wei] + mon[i].val){
            tmp_F -= mon[i].wei;
            ans_min[mon[i].idx]++;
        }
    }

    tmp_F = F;
    for(int i = 0; i < C; i++){
        while(tmp_F >= mon[i].wei && max_val[tmp_F] == max_val[tmp_F - mon[i].wei] + mon[i].val){
            tmp_F -= mon[i].wei;
            ans_max[mon[i].idx]++;
        }
    }

    cout << "TAK\n";

    cout << min_val[F] << "\n";
    for(int i = 0; i < C; i++){
        cout << ans_min[i] << " ";
    }
    cout << "\n";

    cout << max_val[F] << "\n";
    for(int i = 0; i < C; i++){
        cout << ans_max[i] << " ";
    }
    cout << "\n";
}