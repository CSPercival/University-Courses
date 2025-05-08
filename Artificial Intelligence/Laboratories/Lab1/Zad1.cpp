#include<bits/stdc++.h>
#define x first
#define y second

using namespace std;

bool visited[2000010];

struct Position{
    int player;             // player that has a move now (0 - white, 1 - black)
    pair<int,int> kingw;    // white king x and y
    pair<int,int> rookw;    // white rook x and y
    pair<int,int> kingb;    // black king x and y
    int num_of_moves;
};

int hash_fun(Position pos){
    int ans=pos.player;
    ans*=10;
    ans+=pos.kingw.x;
    ans*=10;
    ans+=pos.kingw.y;
    ans*=10;
    ans+=pos.rookw.x;
    ans*=10;
    ans+=pos.rookw.y;
    ans*=10;
    ans+=pos.kingb.x;
    ans*=10;
    ans+=pos.kingb.y;
    return ans;
}

Position input(){
    Position start;

    string player; cin >> player;
    if(player=="white"){
        start.player=0;
    } else {
        start.player=1;
    }
    start.num_of_moves=0;
    
    string tmp_pos;
    cin >> tmp_pos;
    start.kingw={tmp_pos[1]-'0',tmp_pos[0]-'a'+1};

    cin >> tmp_pos;
    start.rookw={tmp_pos[1]-'0',tmp_pos[0]-'a'+1};

    cin >> tmp_pos;
    start.kingb={tmp_pos[1]-'0',tmp_pos[0]-'a'+1};
    // start.wky=tmp_pos[0]-'a'+1;
    // start.wkx=tmp_pos[1]-'0';
    
    // cin >> tmp_pos;
    // start.wry=tmp_pos[0]-'a'+1;
    // start.wrx=tmp_pos[1]-'0';

    // cin >> tmp_pos;
    // start.bky=tmp_pos[0]-'a'+1;
    // start.bkx=tmp_pos[1]-'0';
    
    return start; 
}

void move_black(Position curr_pos, queue<Position> *ptr_BFS){
    
}

void move_white(Position curr_pos, queue<Position> *ptr_BFS){
    
}

void make_move(Position curr_pos, queue<Position> *ptr_BFS){
    if(curr_pos.player){
        move_black(curr_pos,ptr_BFS);
    } else {
        move_white(curr_pos,ptr_BFS);
    }
}


int main(){
    Position curr_pos;
    queue<Position> BFS;
    while(true){
        curr_pos=input();
        while(!BFS.empty()) BFS.pop(); //clearing queue
        
        BFS.push(curr_pos);
        while(!BFS.empty()){
            curr_pos=BFS.front();
            BFS.pop();

            if(visited[hash_fun(curr_pos)]) continue;
            visited[hash_fun(curr_pos)]=true;
            make_move(curr_pos,(&BFS));
        }
    }
}