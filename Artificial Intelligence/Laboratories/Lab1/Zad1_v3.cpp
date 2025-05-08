#include<bits/stdc++.h>
#define x first
#define y second

using namespace std;

string MODE="debug";       //wsad, debug

int visited[2000010];
int king_dx[]={-1,-1,0,1,1,1,0,-1};
int king_dy[]={0,1,1,1,0,-1,-1,-1};
int rook_dx[]={-1,0,1,0};
int rook_dy[]={0,1,0,-1};
pair<int,int> OUT_OF_GAME={9,9};

struct Position{
    int player;             // player that has a move now (0 - white, 1 - black)
    pair<int,int> pos[3];   // 0 - white_king, 1 - white_rook, 2 - black_king
    int num_of_moves;
};

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
    for(int i=0;i<3;i++){
        cin >> tmp_pos;
        start.pos[i]={tmp_pos[1]-'0',tmp_pos[0]-'a'+1};
    }
    return start; 
}

Position unhash_fun(int hash){
    Position ans;
    for(int i=2;i>=0;i--){
        ans.pos[i]={(hash%100)/10,hash%10};
        hash/=100;
    }
    ans.player=hash;
    ans.num_of_moves=0;
    return ans;
}

int hash_fun(Position curr_pos){
    int ans=curr_pos.player;
    for(int i=0;i<3;i++){
        ans*=100;
        ans+=curr_pos.pos[i].x*10+curr_pos.pos[i].y;
    }
    return ans;
}

bool valid_pos(Position curr_pos){
    if(curr_pos.player!=0 && curr_pos.player!=1) return false;
    for(int i=0;i<3;i++){
        if(i==1 && curr_pos.pos[1]==OUT_OF_GAME) continue;      // in that case rook is off the board
        if(curr_pos.pos[i].x<1 || curr_pos.pos[i].x>8 || curr_pos.pos[i].y<1 || curr_pos.pos[i].y>8) return false;
    }
    for(int i=0;i<2;i++){
        for(int j=i+1;j<3;j++){
            if(curr_pos.pos[i]==curr_pos.pos[j]) return false;
        }
    }
    return true;
}

bool valid_move(Position curr_pos){
    int dx=abs(curr_pos.pos[0].x-curr_pos.pos[2].x);
    int dy=abs(curr_pos.pos[0].y-curr_pos.pos[2].y);
    if(dx<2 && dy<2) return false;
    return true;
}

bool check(Position curr_pos){
    if(curr_pos.pos[1].x==curr_pos.pos[2].x){
        if(curr_pos.pos[0].x!=curr_pos.pos[2].x) return true;
        if(curr_pos.pos[0].y<min(curr_pos.pos[1].y,curr_pos.pos[2].y) || curr_pos.pos[0].y>max(curr_pos.pos[1].y,curr_pos.pos[2].y)) return true;
    }

    if(curr_pos.pos[1].y==curr_pos.pos[2].y){
        if(curr_pos.pos[0].y!=curr_pos.pos[2].y) return true;
        if(curr_pos.pos[0].x<min(curr_pos.pos[1].x,curr_pos.pos[2].x) || curr_pos.pos[0].x>max(curr_pos.pos[1].x,curr_pos.pos[2].x)) return true;
    }
    return false;
}

int move_black_king(Position curr_pos, queue<Position> *ptr_BFS){
    Position new_pos=curr_pos;
    new_pos.num_of_moves++;
    new_pos.player^=1;
    
    int move_exist=-1;
    for(int i=0;i<8;i++){
        new_pos.pos[2]={curr_pos.pos[2].x+king_dx[i],curr_pos.pos[2].y+king_dy[i]};
        if(new_pos.pos[2]==new_pos.pos[1]) new_pos.pos[1]=OUT_OF_GAME;
        if(!valid_pos(new_pos) || !valid_move(new_pos) || check(new_pos)) continue;
        move_exist=1;
        if(visited[hash_fun(new_pos)]!=0) continue;
        visited[hash_fun(new_pos)]=hash_fun(curr_pos);
        (*ptr_BFS).push(new_pos);
    }
    return move_exist;
}

void move_white_king(Position curr_pos, queue<Position> *ptr_BFS){
    Position new_pos=curr_pos;
    new_pos.num_of_moves++;
    new_pos.player^=1;

    for(int i=0;i<8;i++){
        new_pos.pos[0]={curr_pos.pos[0].x+king_dx[i],curr_pos.pos[0].y+king_dy[i]};
        if(!valid_pos(new_pos) || !valid_move(new_pos)) continue;
        if(visited[hash_fun(new_pos)]!=0) continue;
        visited[hash_fun(new_pos)]=hash_fun(curr_pos);
        (*ptr_BFS).push(new_pos);
    }
}

void move_white_rook(Position curr_pos, queue<Position> *ptr_BFS){
    Position new_pos;
    for(int i=0;i<4;i++){
        new_pos=curr_pos;
        new_pos.num_of_moves++;
        new_pos.player^=1;

        while(true){
            new_pos.pos[1].x+=rook_dx[i];
            new_pos.pos[1].y+=rook_dy[i];
            if(!valid_pos(new_pos)) break;
            if(visited[hash_fun(new_pos)]!=0) continue;
            visited[hash_fun(new_pos)]=hash_fun(curr_pos);
            (*ptr_BFS).push(new_pos);
        }
    }
}

int make_move(Position curr_pos, queue<Position> *ptr_BFS){
    int result;
    if(curr_pos.player){
        result=move_black_king(curr_pos,ptr_BFS);
        if(result==-1){
            if(check(curr_pos)){
                //checkmate
                return 0;
            } else {
                //stalemate
                return -1;
            }
        }
    } else {
        move_white_king(curr_pos,ptr_BFS);
        if(curr_pos.pos[1]!=OUT_OF_GAME){
            move_white_rook(curr_pos,ptr_BFS);
        }
    }
    return 1;
}

void show(Position curr_pos){
    cout << "VVVVVVVVVVVVVVVVVVVVVVVV\n";

    cout << curr_pos.player << endl;
    for(int i=0;i<3;i++){
        cout << curr_pos.pos[i].x << " " << curr_pos.pos[i].y << endl;
    }
    cout << curr_pos.num_of_moves << endl;
}

void show2(Position curr_pos){
    if(curr_pos.player==0){
        cout << "White ";
    } else {
        cout << "Black ";
    }
    for(int i=0;i<3;i++){
        cout << (char)(curr_pos.pos[i].y+'a'-1) << curr_pos.pos[i].x << " ";
    }
    cout << "\n";
}

void print(Position ending){
    vector<Position> moves;
    Position tmp_pos=ending;
    moves.push_back(tmp_pos);
    while(visited[hash_fun(tmp_pos)]!=-1){
        tmp_pos=unhash_fun(visited[hash_fun(tmp_pos)]);
        moves.push_back(tmp_pos);
    }
    reverse(moves.begin(),moves.end());
    for(auto i : moves){
        show2(i);
    }
} 

int main(){
    Position curr_pos,ans;
    queue<Position> BFS;
    int result;
    while(true){
        curr_pos=input();
        while(!BFS.empty()) BFS.pop(); //clearing queue
        for(int i=0;i<2000000;i++) visited[i]=0;
        ans.num_of_moves=-1;
        
        visited[hash_fun(curr_pos)]=-1;
        BFS.push(curr_pos);
        while(!BFS.empty()){
            curr_pos=BFS.front();
            BFS.pop();
            result=make_move(curr_pos,(&BFS));
            if(result==0){
                ans=curr_pos;
                break;
            }
        }
        if(MODE=="wsad"){
            cout << ans.num_of_moves << "\n";
        }
        if(MODE=="debug"){
            print(ans);
        }
    }
}