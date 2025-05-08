#include<bits/stdc++.h>
#define st first
#define nd second
#define x first
#define y second
using namespace std;

int n;
pair<int,int> points[500010];
pair<int,int> points2[500010];
pair<int,int> tmpp[500010];

long long squa_dist(pair<int,int> a, pair<int,int> b){
    return (long long)(a.x - b.x) * (a.x - b.x) + (long long)(a.y - b.y) * (a.y - b.y); 
}

int dist(pair<int,int> a, pair<int,int> b){
    return sqrt(squa_dist(a,b)) + 1;
}
double exact_dist(pair<int,int> a, pair<int,int> b){
    return sqrt(squa_dist(a,b));
}

bool compare_dist(pair<pair<int,int>,pair<int,int>> a, pair<pair<int,int>,pair<int,int>> b){
    return squa_dist(a.st,a.nd) > squa_dist(b.st,b.nd);
}

struct Triangle{
    pair<int,int> a,b,c;
    long double d;
    void count_d(){
        d = exact_dist(a,b) + exact_dist(a,c) + exact_dist(c,b);
    }
    void print(){
        cout << "( (" << a.st << " - " << a.nd << ") (" << b.st << " - " << b.nd << ") (" << c.st << " - " << c.nd << ") " << d << ")\n"; 
    }
    void sortp(){
        vector<pair<int,int>> xd = {a,b,c};
        sort(xd.begin(),xd.end());
        a = xd[0];
        b = xd[1];
        c = xd[2];
    }
};

int main(){
    cin >> n;
    Triangle tans = {{1e9,1e9},{-1e9,-1e9},{-1e9,1e9},1e9};
    Triangle ans = tans;

    for(int i = 0; i < n; i++){
        cin >> points[i].first >> points[i].second;
    }

    for(int i = 0; i < n; i++){
        tans.a = points[i];
        for(int j = i + 1; j < n; j++){
            tans.b = points[j];
            for(int k = j + 1; k < n; k++){
                tans.c = points[k];
                tans.count_d();
                if(tans.d < ans.d){
                    ans = tans;
                }
            }
        }
    }
    ans.sortp();
    cout << ans.a.x << " " << ans.a.y << "\n" << ans.b.x << " " << ans.b.y << "\n" << ans.c.x << " " << ans.c.y << "\n";
}