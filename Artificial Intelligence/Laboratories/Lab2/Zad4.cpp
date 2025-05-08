#include<bits/stdc++.h>
#include <time.h>

using namespace std;

const string PATH_NOT_FOUND="NOT_FOUND";
ifstream input_file;
ofstream output_file;

int numof_rows,numof_columns;
string board[100];
vector<pair<int,int>> S;
//int dx[]={-1,0,1,0};
//int dy[]={0,1,0,-1};
pair<int,int> dxy[]={{-1,0},{0,1},{1,0},{0,-1}};
string sxy[]={"U","R","D","L"};
//set<pair<int,int>> G;

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
    // cout << ctr1 << "\n";
    // for(int i=0;i<ctr1;i++){
    //     cout << board[i] << "\n";
    // }
    numof_rows=ctr1;
    numof_columns=board[0].size();
    return;
}

void find_SG(){
    for(int i=0;i<numof_rows;i++){
        for(int j=0;j<numof_columns;j++){
            /*
            switch(board[i][j]){
                case 'G':
                    G.insert({i,j});
                    break;
                case 'S':
                    S.insert({i,j});
                    break;
                case 'B':
                    G.insert({i,j});
                    S.insert({i,j});
                    break;
            }
            */
            if(board[i][j]=='S' || board[i][j]=='B'){
                S.push_back({i,j});
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

vector<pair<int,int>> generate_move(vector<pair<int,int>> states, int direction){
    vector<pair<int,int>> new_states,tmp_states;
    tmp_states.push_back({-1,-1});
    for(auto state : states){
        tmp_states.push_back(make_move(state,direction));
    }
    sort(tmp_states.begin(),tmp_states.end());
    for(int i=1;i<tmp_states.size();i++){
        if(tmp_states[i]!=tmp_states[i-1]){
            new_states.push_back(tmp_states[i]);
        }
    }
    return new_states;
}

bool check_if_answer(vector<pair<int,int>> states){
    for(auto state : states){
        if(check_field(state)!=1) return false;
    }
    return true;
}

string BFS(vector<pair<int,int>> start, string pre_ans, int depth){
    if(check_if_answer(start)) return pre_ans;

    map<vector<pair<int,int>>,string> states;
    queue<vector<pair<int,int>>> q;
    vector<pair<int,int>> new_state,state;
    string new_path;
    string ans=PATH_NOT_FOUND;
    bool ans_found=false;

    sort(start.begin(),start.end());
    states[start]="";
    q.push(start);
    while(!q.empty() && states[q.front()].size()<depth && !ans_found){
        state=q.front();
        //cout << states[state] << endl;
        q.pop();
        for(int direction=0;direction<4;direction++){
            new_state=generate_move(state,direction);
            new_path=states[state]+sxy[direction];
            if(check_if_answer(new_state)){
                ans=new_path;
                ans_found=true;
                break;
            }
            if(states.find(new_state)==states.end()){
                states[new_state]=states[state]+sxy[direction];
                q.push(new_state);
            }
        }
    }
    if(ans==PATH_NOT_FOUND) return PATH_NOT_FOUND;
    return pre_ans+ans;
}

pair<vector<pair<int,int>>,string> generate_random_moves(vector<pair<int,int>> new_states,int numof_moves){
    string new_path="";
    int direction;
    for(int it1=0;it1<numof_moves;it1++){
        direction=random_number(0,4);
        new_states=generate_move(new_states,direction);
        new_path+=sxy[direction];
    }
    return {new_states,new_path};
}

int main(){
    srand(time(0));
    input_file.open("zad_input.txt");
    output_file.open("zad_output.txt");

    read_input();
    find_SG();

    vector<pair<int,int>> new_S;
    string tmp_ans_path;
    pair<vector<pair<int,int>>,string> part_ans;
    string ans_path=PATH_NOT_FOUND;
    int ctr1=0;
    while(ans_path==PATH_NOT_FOUND){
        //cout << ctr1++ << endl;
        do{
            part_ans=generate_random_moves(S,110);
        } while(part_ans.first.size()>3);
        cout << "FOUND IT\n";
        ans_path=BFS(part_ans.first,part_ans.second,40);
    }
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

######################
#SSSSSSSS#SSS##SSSSSS#
#SSSSSSSSSSSS##SSSSSS#
#SSSSSS###SSSSSSSSS#B#
#SSSSSS###SSSSSSSSSSS#
#SSSSSSSSSSSSSSSSSSSS#
#####SSSSSSSSSSSSSSSS#
#SSSSSSSSSSSSSSSSSSSS#
######################
*/