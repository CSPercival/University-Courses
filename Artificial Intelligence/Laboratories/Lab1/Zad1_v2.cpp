#include<bits/stdc++.h>

using namespace std;

typedef vector<pair<int,int>> Position; 
set<Position> visited;

Position input(){
    Position start(4);

    string player; cin >> player;
    if(player=="white"){
        start[0]={0,0};
    } else {
        start[0]={1,1};;
    }
    
    string tmp_pos;
    for(int i=1;i<=3;i++){
        cin >> tmp_pos;
        start[i]={tmp_pos[1]-'0',tmp_pos[0]-'a'+1};
    }
    return start; 
}


int main(){
    Position current;
    while(true){
        current=input();
        
    }
}