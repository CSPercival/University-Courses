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


int main(){
    cin >> n;
    pair<pair<int,int>,pair<int,int>> pans = {{1e9,1e9},{-1e9,-1e9}};
    for(int i = 0; i < n; i++){
        cin >> points[i].first >> points[i].second;
    }
    int idx1 = -1, idx2 = -1;
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            if(compare_dist(pans,{points[i],points[j]})){
                pans = {points[i],points[j]};
                idx1 = i;
                idx2 = j;
            }
        }
    }
    double d = exact_dist(pans.st,pans.nd);
    cout << fixed << setprecision(6) << idx1 << " " << idx2 << " " << d << "\n";
}