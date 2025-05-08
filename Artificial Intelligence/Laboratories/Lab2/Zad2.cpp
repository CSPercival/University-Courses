#include<bits/stdc++.h>
#include <time.h>

using namespace std;

const int MAX_N=100;
const string PATH_NOT_FOUND="NOT_FOUND";
const int DEGOF_NON_OPTIMALITY=2;

ifstream input_file;
ofstream output_file;

int numof_rows,numof_columns;
string board[MAX_N];
pair<int,pair<int,int>> closest_toG[MAX_N][MAX_N];
set<pair<int,int>> B;
vector<pair<int,int>> G;
pair<int,int> K;
//int dx[]={-1,0,1,0};
//int dy[]={0,1,0,-1};
pair<int,int> dxy[]={{-1,0},{0,1},{1,0},{0,-1}};
string sxy[]={"U","R","D","L"};
char rev_sxy[]={'D','L','U','R'};

struct State{
    string path;
    pair<int,int> K_field;
    set<pair<int,int>> B_fields;
};

// struct CmpS {
//     bool operator()(State a, State b){
//         return a.S_fields<b.S_fields;
//     }
// };

// class CmpPiS {
// public:
//     bool operator()(pair<int,State> a, pair<int,State> b){
//         if(a.first==b.first){
//             if(a.second.S_fields.size()==b.second.S_fields.size()){
//                 return a.second.path>b.second.path;
//             }
//             return a.second.S_fields.size()>b.second.S_fields.size();
//         }
//         return a.first>b.first;
//     }
// };

int random_number(int l, int r){
    return (rand()%(r-l))+l;
}

pair<int, int> operator+ (pair<int, int> a, pair<int, int> b) {
    return {a.first+b.first,a.second+b.second};
}

pair<int, int> operator- (pair<int, int> a, pair<int, int> b) {
    return {a.first-b.first,a.second-b.second};
}

void read_input(){
    int ctr1=0;
    while(getline(input_file,board[ctr1])){
        ctr1++;
    }
    numof_rows=ctr1;
    numof_columns=board[0].size();
    return;
}

void read_input_debug(){
    int ctr1=0;
    while(getline(cin,board[ctr1])){
        ctr1++;
    }
    numof_rows=ctr1;
    numof_columns=board[0].size();
    return;
}

void find_BKG(){
    for(int i=0;i<numof_rows;i++){
        for(int j=0;j<numof_columns;j++){
            switch(board[i][j]){
                case 'G':
                    G.push_back({i,j});
                    break;
                case 'B':
                    B.insert({i,j});
                    board[i][j]='.';
                    break;
                case '*':
                    B.insert({i,j});
                    G.push_back({i,j});
                    board[i][j]='G';
                    break;
                case 'K':
                    K={i,j};
                    board[i][j]='.';
                    break;
                case '+':
                    K={i,j};
                    board[i][j]='G';
                    G.push_back({i,j});
                    break;
            }
        }
    }
}

int check_field(pair<int,int> coord){
    switch(board[coord.first][coord.second]){
        case 'W':
            return 0;
        case 'G':
            return 1;
        default:
            return 2;
    }
}

// pair<int,int> make_move(pair<int,int> coord, int direction){
//     // if(check_field(coord+dxy[direction])){
//     //     return coord+dxy[direction];
//     // }
//     // return coord;

// }

State generate_move(State* state, int direction){
    State new_state,tmp_state;
    //new_state=(*state);
    //new_state.K_field+=dxy[direction];
    new_state=(*state);
    new_state.K_field=new_state.K_field+dxy[direction];
    new_state.path=new_state.path+sxy[direction];
    if((*state).B_fields.find(new_state.K_field)==(*state).B_fields.end()){
        // na nastepnym polu nie ma skrzynki
        if(!check_field(new_state.K_field)){
            // na nastepnym jest wall
            new_state.K_field=new_state.K_field-dxy[direction];
            new_state.path.pop_back();
            //new_state.K_field=new_state.K_field+dxy[direction];
        }
    } else {
        //na nastepnym polu jest skrzynka;
        if((*state).B_fields.find(new_state.K_field+dxy[direction])==(*state).B_fields.end()){
            // na nastepnym nastepnym polu nie ma skrzynki
            if(check_field(new_state.K_field)){
                //na nastepnym nastepnym nie ma tez walla
                //new_state.K_field=new_state.K_field+dxy[direction];
                new_state.B_fields.erase(new_state.K_field);
                new_state.B_fields.insert(new_state.K_field+dxy[direction]);
            } else {
                //na nastepnym nastepym jest wall
                new_state.K_field=new_state.K_field-dxy[direction];
                new_state.path.pop_back();
            }
        } else {
            //na nastepnym nastepnym jest skrzynia
            new_state.K_field=new_state.K_field-dxy[direction];
            new_state.path.pop_back();
        }
    }
    // tmp_state.S_fields.push_back({-1,-1});
    // for(auto pos_S : (*state).S_fields){
    //     tmp_state.S_fields.push_back(make_move(pos_S,direction));
    // }
    // sort(tmp_state.S_fields.begin(),tmp_state.S_fields.end());
    // for(int i=1;i<tmp_state.S_fields.size();i++){
    //     if(tmp_state.S_fields[i]!=tmp_state.S_fields[i-1]){
    //         new_state.S_fields.push_back(tmp_state.S_fields[i]);
    //     }
    // }
    // new_state.path=(*state).path+sxy[direction];
    return new_state;
}

bool check_if_answer(set<pair<int,int>>* states){
    for(auto state : (*states)){
        if(check_field(state)!=1) return false;
    }
    return true;
}

bool dead_box(pair<int,int> pos){
    if(board[pos.first][pos.second]=='G') return false;
    //pair<int,int> new_pos;
    for(int dir=0;dir<4;dir++){
        //new_pos=pos+dxy[dir];
        if(!check_field(pos+dxy[dir]) && !check_field(pos+dxy[(dir+1)%4])) return true;
    }
    return false;
}

bool dead_state(State* act_state){
    for(auto pos_B : (*act_state).B_fields){
        if(dead_box(pos_B)) return true;
    }
    return false;
}

string BFS(State start){
    if(check_if_answer(&start.B_fields)) return start.path;

    set<pair<set<pair<int,int>>,pair<int,int>>> states;
    queue<State> q;
    State new_state,act_state;
    string ans=PATH_NOT_FOUND;
    bool ans_found=false;

    //sort(start.B_fields.begin(),start.B_fields.end());
    states.insert({start.B_fields,start.K_field});
    q.push(start);
    while(!q.empty() && !ans_found){
        act_state=q.front();
        //cout << states[state] << endl;
        q.pop();
        for(int direction=0;direction<4;direction++){
            new_state=generate_move(&act_state,direction);
            //new_path=states[state]+sxy[direction];
            if(check_if_answer(&new_state.B_fields)){
                ans=new_state.path;
                ans_found=true;
                break;
            }
            if(states.find({new_state.B_fields,new_state.K_field})==states.end()){
                states.insert({new_state.B_fields,new_state.K_field});
                q.push(new_state);
                //pq.push({eval(&new_state),new_state});
                //pq.push({eval2(&new_state,DEGOF_NON_OPTIMALITY),new_state});
            }
        }
    }
    if(ans==PATH_NOT_FOUND) return PATH_NOT_FOUND;
    return ans;
}

int main(){
    srand(time(0));
    input_file.open("zad_input.txt");
    output_file.open("zad_output.txt");

    read_input();
    //read_input_debug();
    find_BKG();
    State start;
    start.B_fields=B;
    start.K_field=K;
    start.path="";
    
    string ans_path;
    ans_path=BFS(start);

    //cout << ans_path << endl;
    output_file << ans_path << "\n";
    input_file.close();
    output_file.close();
}
/*
WWWWWW
W.GWWW
W..WWW
W*K..W
W..B.W
W..WWW
WWWWWW

DLURRRDLULLDDRULURUULDRDDRRULDLUU
DLURRRDLULLDDRULURUULDRDDRRULDLUU
 && !dead_state(&new_state)
*/