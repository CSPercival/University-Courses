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

void sorcik_y(int l, int r){
    for(int i = r - 1; i >= l; i--){
        for(int j = l; j < i; j++){
            if(points[j].y > points[j + 1].y){
                swap(points[j],points[j + 1]);
            }
        }
    }
}

void merge(int l, int r){
    int mid = (l+r)/2;
    int it1 = l, it2 = mid;
    int it3 = l;
    while(it1 < mid && it2 < r){
        if(points[it1].y < points[it2].y){
            tmpp[it3] = points[it1];
            it1++;
        } else {
            tmpp[it3] = points[it2];
            it2++;
        }
        it3++;
    }
    while(it1 < mid) tmpp[it3++] = points[it1++];
    while(it2 < r) tmpp[it3++] = points[it2++];
    for(int i = l; i < r; i++) {
        points[i] = tmpp[i];
    }
}

Triangle f1(int l, int r){
    Triangle ans = {{1e9,1e9},{-1e9,-1e9},{-1e9,1e9},0};
    Triangle tans;
    ans.count_d();
    if(r - l < 6){
        for(int i = l; i < r; i++){
            tans.a = points[i];
            for(int j = i + 1; j < r; j++){
                tans.b = points[j];
                for(int k = j + 1; k < r; k++){
                    tans.c = points[k];
                    tans.count_d();
                    if(tans.d < ans.d){
                        ans = tans;
                    }
                }
            }
        }
        sorcik_y(l,r);

        return ans;
    }
    int stick = points[(r+l)/2].x;
    auto tans_l = f1(l,(r+l)/2), tans_r = f1((r+l)/2, r);
    if(tans_l.d < tans_r.d){
        ans = tans_l;
    } else {
        ans = tans_r;
    }
    int d = ans.d/2 + 1;
    merge(l,r);
    
    int it1 = l;
    for(int i = l; i < r; i++){
        if(stick - d <= points[i].x && points[i].x <= stick + d){
            tmpp[it1++] = points[i];
        }
    }
    for(int i = l; i < it1; i++){
        tans.a = tmpp[i];
        int j = i + 1;
        while(j < it1 && tmpp[j].y - tmpp[i].y <= d){
            tans.b = tmpp[j];
            int k = j + 1;
            while(k < it1 && tmpp[k].y - tmpp[i].y <= d){
                tans.c = tmpp[k];
                tans.count_d();
                if(tans.d < ans.d){
                    ans = tans;
                }
                k++;
            }
            j++;
        }
    }
    return ans;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cin >> n;
    for(int i = 0; i < n; i++){
        cin >> points[i].first >> points[i].second;
        points2[i] = points[i];
    }
    sort(points, points + n);
    Triangle ans = f1(0,n);
    // ans.sortp();
    // cout << ans.a.x << " " << ans.a.y << "\n" << ans.b.x << " " << ans.b.y << "\n" << ans.c.x << " " << ans.c.y << "\n";
    cout << fixed << setprecision(6) << ans.d << "\n";
}