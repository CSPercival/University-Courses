#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<iomanip>
#define st first 
#define nd second
using namespace std;


vector<pair<int, int>> points, tmp;

double dist(pair<double, double> a, pair<double, double> b) {
    return sqrt((a.st-b.st)*(a.st-b.st) + (a.nd-b.nd)*(a.nd-b.nd));
}

double triangle(pair<double, double>a, pair<double, double> b, pair<double, double> c) {
    return dist(a, b) + dist(b, c) + dist(c, a);
}

pair<double, pair<pair<pair<int, int>, pair<int, int>>, pair<int, int>>> solve(int l, int r) {
    // cout << l << ' ' << r << endl;
    if (r-l == 1) return {1e18, {{{0, 0}, {0, 0}}, {0, 0}}};
    int mid = (l+r)/2;
    double x = points[mid].st;
    pair<double, pair<pair<pair<int, int>, pair<int, int>>, pair<int, int>>> left = solve(l, mid), right = solve(mid, r);
    pair<double, pair<pair<pair<int, int>, pair<int, int>>, pair<int, int>>> d = left;
    if (d.st > right.st) d = right;
    vector<pair<int, int>> A, B;
    
    for (int i = l; i < mid; i++) {
        if ((double)points[i].first >= x-(d.st/2.0)) A.push_back(points[i]);
    }
    for (int i = mid; i < r; i++) {
        if ((double)points[i].first <= x+(d.st/2.0)) B.push_back(points[i]);
    }
    int p = 0, q = 0;
    for (int i = 0; i < A.size(); i++) {
        while (q < B.size() && B[q].nd <= (double)A[i].nd+d.st) q++;
        while (p < B.size() && B[p].nd < (double)A[i].nd-d.st) p++; 
        for (int j = p; j < q; j++) {
            for (int k = j+1; k < q; k++) {
                if (triangle(A[i], B[j], B[k]) < d.st) {
                    d.st = triangle(A[i], B[j], B[k]);
                    d.nd = {{A[i], B[j]}, B[k]};
                }
            }
        }
    }
    p = 0, q = 0;
    for (int i = 0; i < B.size(); i++) {
        while (q < A.size() && A[q].nd <= (double)B[i].nd+d.st) q++;
        while (p < A.size() &&( A[p].nd < (double)B[i].nd-d.st)) p++; 
        for (int j = p; j < q; j++) {
            for (int k = j+1; k < q; k++) {
                if (triangle(B[i], A[j], A[k]) < d.st) {
                    d.st = triangle(B[i], A[j], A[k]);
                    d.nd = {{B[i], A[j]}, A[k]};
                }
            }
        }
    }
    // for (int i = l; i < r; i++) cout << "[" << points[i].st << ' ' << points[i].nd << "] ";
    // cout << endl;
    int it1=l, it2=mid;
    // cout << it1 << ' ' << it2 << endl;
    while ((it1 < mid) || (it2 < r)) {
        if (it1 == mid) {
            tmp[it1+it2-mid] = points[it2];
            it2++;
        }
        else if (it2 == r) {
            tmp[it1+it2-mid] = points[it1];
            it1++;
        }
        else if (points[it1].nd < points[it2].nd || ((points[it1].nd == points[it2].nd) && (points[it1].st < points[it2].st))) {
            tmp[it1+it2-mid] = points[it1];
            it1++;
        }
        else {
            tmp[it1+it2-mid] = points[it2];
            it2++;
        }
    }
    for (int i = l; i < r; i++) points[i] = tmp[i];
    
    // for (int i = l; i < r; i++) cout << "[" << points[i].st << ' ' << points[i].nd << "] ";
    // cout << endl;
    return d;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    points.resize(n);
    tmp.resize(n);
    // answer.resize(3);
    for (int i = 0; i < n; i++) {
        cin >> points[i].st >> points[i].nd;
    }
    sort(points.begin(), points.end());
    auto result = solve(0, n);
    // for (int i = 0; i < 3; i++) cout << answer[i].st << ' ' << answer[i].nd << '\n';
    // cout << result.nd.st.st.st << ' ' << result.nd.st.st.nd << '\n';
    // cout << result.nd.st.nd.st << ' ' << result.nd.st.nd.nd << '\n';
    // cout << result.nd.nd.st << ' ' << result.nd.nd.nd << '\n';
    cout << fixed << setprecision(6) << result.st << "\n";
}