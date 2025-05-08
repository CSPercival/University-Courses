#include<iostream>

using namespace std;

const int N = 1e6 + 10;

int n;
int per[N];
int deg[N];
int ptr[N];
int graph[N];
pair<int,int> timetable[N];

void get_input(){
    for(int i = 2; i <= n; i++){
        cin >> per[i];
        deg[per[i]]++;
    }
    for(int i = 1; i <= n; i++){
        deg[i] += deg[i - 1];
    }
    for(int i = n; i >= 0; i--){
        // deg[i] += deg[i - 1];
        deg[i + 1] = deg[i];
    }
    for(int i = 2; i <= n; i++){
        graph[deg[per[i]] + ptr[per[i]]] = i;
        ptr[per[i]]++;
    }
    for(int i = 1; i <= n; i++){
        ptr[i] = 0;
    }
}

void dfs(){
    int time = 0;
    int dfs_ptr = 0;
    int parent,child;
    // per -> dfs stack;
    per[1] = 1;
    dfs_ptr = 1;
    timetable[1].first = time++;
    while(dfs_ptr != 0){
        parent = per[dfs_ptr];
        if(deg[parent] + ptr[parent] != deg[parent + 1]){
            child = graph[deg[parent] + ptr[parent]++];
            per[++dfs_ptr] = child;
            timetable[child].first = time++;
        } else {
            per[dfs_ptr--] = 0;
            timetable[parent].second = time++;
        }
    }
    // for(int i = 0; i <= n; i++){
    //     printf("Time of ver %d - (%d,%d)\n",i,timetable[i].first,timetable[i].second);
    // }
}

bool ancestor(int a, int b){
    if(timetable[a].first < timetable[b].first && timetable[a].second > timetable[b].second) return true;
    return false;
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int q;
    cin >> n >> q;
    get_input();
    dfs();
    int a,b;
    while(q--){
        cin >> a >> b;
        if(ancestor(a,b)){
            cout << "TAK\n";
        } else {
            cout << "NIE\n";
        }
    }
    return 0;
}