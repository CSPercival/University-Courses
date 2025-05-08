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

pair<pair<int,int>,pair<int,int>> f1(int l, int r){
    pair<pair<int,int>,pair<int,int>> pans = {{1e9,1e9},{-1e9,-1e9}};
    // cout << l << " - " << r << endl;
    if(r - l < 4){
        // cout << "ok5" << endl;
        for(int i = l; i < r; i++){
            for(int j = i + 1; j < r; j++){
                if(compare_dist(pans,{points[i],points[j]})){
                    pans = {points[i],points[j]};
                }
            }
        }
        sorcik_y(l,r);

        // for(int i = l; i < r; i++){
        //     cout << "( " << points[i].st << " - " << points[i].nd << ") ";
        // }
        // cout << "\n";
        return pans;
    }
    int stick = points[(r+l)/2].x;
    auto pans_l = f1(l,(r+l)/2), pans_r = f1((r+l)/2, r);
    // cout << "ok3" << endl;
    if(compare_dist(pans_l,pans_r)){
        pans = pans_r;
    } else {
        pans = pans_l;
    }
    // cout << pans.st.x << " " << pans.st.y << " ---- " << pans.nd.x << " " << pans.nd.y << "\n";
    int d = dist(pans.st,pans.nd);
    merge(l,r);

    // for(int i = l; i < r; i++){
    //     cout << "( " << points[i].st << " - " << points[i].nd << ") ";
    // }
    // cout << "\n";
    
    int it1 = l;
    for(int i = l; i < r; i++){
        if(stick - d <= points[i].x && points[i].x <= stick + d){
            tmpp[it1++] = points[i];
        }
    }
    // cout << "ok4 " << it1 << endl; 
    for(int i = l; i < it1; i++){
        int j = i + 1;
        while(j < it1 && tmpp[j].y - tmpp[i].y <= d){
            if(compare_dist(pans,{tmpp[i],tmpp[j]}))
                pans = {tmpp[i],tmpp[j]};
            j++;
        }
    }
    return pans;
}

int main(){
    // int q; cin >> q;
    cin >> n;
    // while(n){
        for(int i = 0; i < n; i++){
            cin >> points[i].first >> points[i].second;
            points2[i] = points[i];
        }
        sort(points, points + n);
        // cout << "ok1" << endl;
        pair<pair<int,int>, pair<int,int>> ans = f1(0,n);
        int idx1 = -1, idx2 = -1;
        for(int i = 0; i < n; i++){
            if(points2[i] == ans.st && idx1 == -1){
                idx1 = i;
                continue;            
            }
            if(points2[i] == ans.nd && idx2 == -1){
                idx2 = i;
                continue;            
            }
        }
        // cout << "ok2" << endl;
        // cout << ans.st.x << " " << ans.st.y << "\n" << ans.nd.x << " " << ans.nd.y << "\n";
        double d = exact_dist(ans.st,ans.nd);
        cout << fixed << setprecision(6) << min(idx1,idx2) << " " << max(idx1,idx2) << " " << d << "\n";
        
        // if(d >= 10000){
        //     cout << "INFINITY\n";
        // } else {
        //     cout << d << "\n";
        // }
        // cin >> n;
    // }
    // cout << ans.st.x << " " << ans.st.y << "\n" << ans.nd.x << " " << ans.nd.y << "\n";

}