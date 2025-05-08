#include<bits/stdc++.h>
#include <time.h>

using namespace std;

const int MAX_N=100;
const string PATH_NOT_FOUND="NOT_FOUND";
const int DEGOF_NON_OPTIMALITY=3;

ifstream input_file;
ofstream output_file;

int numof_rows,numof_columns;
string board[MAX_N];
pair<int,pair<int,int>> closest_toG[MAX_N][MAX_N];
vector<pair<int,int>> S;
vector<pair<int,int>> G;
//int dx[]={-1,0,1,0};
//int dy[]={0,1,0,-1};
pair<int,int> dxy[]={{-1,0},{0,1},{1,0},{0,-1}};
string sxy[]={"U","R","D","L"};
char rev_sxy[]={'D','L','U','R'};

struct State{
    string path;
    vector<pair<int,int>> S_fields;
};

struct CmpS {
    bool operator()(State a, State b){
        return a.S_fields<b.S_fields;
    }
};

class CmpPiS {
public:
    bool operator()(pair<int,State> a, pair<int,State> b){
        if(a.first==b.first){
            if(a.second.S_fields.size()==b.second.S_fields.size()){
                return a.second.path>b.second.path;
            }
            return a.second.S_fields.size()>b.second.S_fields.size();
        }
        return a.first>b.first;
    }
};

int random_number(int l, int r){
    return (rand()%(r-l))+l;
}

pair<int, int> operator+ (pair<int, int> a, pair<int, int> b) {
    return {a.first+b.first,a.second+b.second};
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

void find_SG(){
    for(int i=0;i<numof_rows;i++){
        for(int j=0;j<numof_columns;j++){
            switch(board[i][j]){
                case 'G':
                    G.push_back({i,j});
                    break;
                case 'S':
                    S.push_back({i,j});
                    break;
                case 'B':
                    G.push_back({i,j});
                    S.push_back({i,j});
                    break;
            }
        }
    }
}

int check_field(pair<int,int> coord){
    switch(board[coord.first][coord.second]){
        case '#':
            return 0;
        case 'G':
        case 'B':
            return 1;
        default:
            return 2;
    }
}

pair<int,int> make_move(pair<int,int> coord, int direction){
    if(check_field(coord+dxy[direction])){
        return coord+dxy[direction];
    }
    return coord;
}

State generate_move(State* state, int direction){
    State new_state,tmp_state;
    tmp_state.S_fields.push_back({-1,-1});
    for(auto pos_S : (*state).S_fields){
        tmp_state.S_fields.push_back(make_move(pos_S,direction));
    }
    sort(tmp_state.S_fields.begin(),tmp_state.S_fields.end());
    for(int i=1;i<tmp_state.S_fields.size();i++){
        if(tmp_state.S_fields[i]!=tmp_state.S_fields[i-1]){
            new_state.S_fields.push_back(tmp_state.S_fields[i]);
        }
    }
    new_state.path=(*state).path+sxy[direction];
    return new_state;
}

bool check_if_answer(vector<pair<int,int>>* states){
    for(auto state : (*states)){
        if(check_field(state)!=1) return false;
    }
    return true;
}

void find_paths_toG(){
    for(int row=0;row<numof_rows;row++){
        for(int column=0;column<numof_columns;column++){
            closest_toG[row][column]={INT_MAX,{0,0}};   
        }
    }

    queue<pair<int,int>> q;
    pair<int,int> act_pos,new_pos;
    
    for(auto pos : G){
        closest_toG[pos.first][pos.second]={0,pos};
        q.push(pos);
    }
    while(!q.empty()){
        act_pos=q.front();
        q.pop();
        for(int direction=0;direction<4;direction++){
            new_pos=make_move(act_pos,direction);
            if(closest_toG[new_pos.first][new_pos.second].first>closest_toG[act_pos.first][act_pos.second].first+1){
                closest_toG[new_pos.first][new_pos.second].first=closest_toG[act_pos.first][act_pos.second].first+1;
                closest_toG[new_pos.first][new_pos.second].second=act_pos;
                q.push(new_pos);
            }
        }
    }
    // for(int i=0;i<numof_rows;i++){
    //     for(int j=0;j<numof_columns;j++){
    //         if(closest_toG[i][j].first==INT_MAX){
    //             cout << "X";
    //             continue;
    //         }
    //         cout << closest_toG[i][j].first << " ";
    //     }
    //     cout << "\n";
    // }
}

int eval(State* state){
    int heura=0;
    for(auto pos_S : (*state).S_fields){
        heura=max(heura,closest_toG[pos_S.first][pos_S.second].first);
    }
    return (*state).path.size()+heura;
}

int eval2(State* state,int degof_notopt){
    int heura=0;
    vector<int> distances;
    for(auto pos_S : (*state).S_fields){
        //heura=max(heura,closest_toG[pos_S.first][pos_S.second].first);
        distances.push_back(closest_toG[pos_S.first][pos_S.second].first);
    }
    sort(distances.begin(),distances.end(),greater<int>());
    for(int i=0;i<min(degof_notopt,(int)distances.size());i++){
        heura+=distances[i];
    }
    return (*state).path.size()+heura;
}

string A_Star(State start){
    if(check_if_answer(&start.S_fields)) return start.path;

    set<vector<pair<int,int>>> states;
    priority_queue<pair<int,State>,vector<pair<int,State>>,CmpPiS> pq;
    State new_state,act_state;

    string new_path;
    string ans=PATH_NOT_FOUND;
    bool ans_found=false;

    sort(start.S_fields.begin(),start.S_fields.end());
    //states.insert(start.S_fields);
    pq.push({0,start});
    while(!pq.empty() && !ans_found){
        act_state=pq.top().second;
        //cout << states[state] << endl;
        pq.pop();
        if(states.find(act_state.S_fields)!=states.end()) continue;
        states.insert(act_state.S_fields);
        for(int direction=0;direction<4;direction++){
            new_state=generate_move(&act_state,direction);
            //new_path=states[state]+sxy[direction];
            if(check_if_answer(&new_state.S_fields)){
                // if(ans==PATH_NOT_FOUND || ans.size()>new_state.path.size()){
                //     ans=new_state.path;
                //     break;
                // }
                ans=new_state.path;
                ans_found=true;
                break;
            }
            if(states.find(new_state.S_fields)==states.end()){
                //states.insert(new_state.S_fields);
                //pq.push({eval(&new_state),new_state});
                pq.push({eval2(&new_state,DEGOF_NON_OPTIMALITY),new_state});
            }
        }
    }
    //if(ans==PATH_NOT_FOUND) return PATH_NOT_FOUND;
    return start.path+ans;
}

int main(){
    srand(time(0));
    input_file.open("zad_input.txt");
    output_file.open("zad_output.txt");

    read_input();
    //read_input_debug();
    find_SG();
    find_paths_toG();

    State start;
    start.path="";
    start.S_fields=S;
    string ans_path=A_Star(start);
    //cout << ans_path << endl;
    output_file << ans_path << "\n";
    input_file.close();
    output_file.close();
}
/*
######################
# G   G              #
#    #       #S      #
#  S #       #       #
######### ############
#    G#           G  #
##   ##    ###########
#    #       S       #
# ## ###########     #
#    #            S  #
#S         ####      #
######################

ULLLULLLLLLLLLURURUDRRRRRRRURR
LLUULLULLLLURRULUUUULLLLLLLLRRRRR

######################
#SSSSSSSS#SSS##SSSSSS#
#SSSSSSSSSSSS##SSSSSS#
#SSSSSS###SSSSSSSSS#B#
#SSSSSS###SSSSSSSSSSS#
#SSSSSSSSSSSSSSSSSSSS#
#####SSSSSSSSSSSSSSSS#
#SSSSSSSSSSSSSSSSSSSS#
######################

#####
#S S#
# G #
#S S#
#####

#####
#G# #
# S #
#  G#
#S# #
#   #
#SSS#
#####
RRURRUULDDR
RRUUUDLLU
LLUUURR
*/