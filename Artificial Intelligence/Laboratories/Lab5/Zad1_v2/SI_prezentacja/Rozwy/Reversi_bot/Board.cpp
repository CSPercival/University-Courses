#ifndef BOARD_CPP
#define BOARD_CPP

#include "Libraries_and_Macros.h"
#include "Functions.h"

class Board{
    vector <vector <int>> board;
    vector <int> numof_discs;
    vector <int> weighted_discs;
    //int numof_dark,numof_light;
    public:
    Board(){
        //setup();
        numof_discs.resize(3);
        weighted_discs.resize(3);
    }

    void setup(){
        board = vector <vector <int>> (MAX_N, vector <int> (MAX_N, 0));
        board[3][3] = 2;
        board[3][4] = 1;
        board[4][3] = 1;
        board[4][4] = 2;

        //numof_discs.resize(3);
        numof_discs[0] = 0;
        numof_discs[1] = 2;
        numof_discs[2] = 2;

        weighted_discs[0] = 0;
        weighted_discs[1] = 2;
        weighted_discs[2] = 2;
    }

    int get_val(pair<int,int> coord){ return board[coord.st][coord.nd]; }
    void set_val(pair<int,int> coord, int val){ board[coord.st][coord.nd] = val; }

    int evaluate(int player){
        int opponent = (player & 1) + 1;
        vector< vector <int>> vis(MAX_N, vector <int> (MAX_N, 0));
        vector< pair<int,int>> possplr,possopp;

        for(int row = 0; row < MAX_N; row++){
            for(int column = 0; column < MAX_N; column++){
                if(correct_move({row, column}, player)) possplr.push_back({row,column});
                if(correct_move({row, column}, opponent)) possopp.push_back({row,column});
            }
        }
        if(possplr.empty() && possopp.empty()){
            if(result(player) > 0) return INT_MAX - 1;
            if(result(player) < 0) return INT_MIN + 1;
            return 0;
        }

        vector<int> res(3,0);
        int limit;
        for(int plr = 1; plr <=2; plr++){
            limit = MAX_N;
            for(int row = 0; row < MAX_N; row++){
                if(get_val({row,0}) != plr) break;
                int column;
                for(column = 0; column < limit; column++){
                    if(get_val({row,column}) != plr) break;
                    if(vis[row][column] == plr) continue;
                    vis[row][column] = plr;
                    res[plr] += stable_disc_weight;
                }
                limit = column;
            }
            limit = -1;
            for(int row = 0; row < MAX_N; row++){
                if(get_val({row,MAX_N-1}) != plr) break;
                int column;
                for(column = MAX_N-1; column > limit; column--){
                    if(get_val({row,column}) != plr) break;
                    if(vis[row][column] == plr) continue;
                    vis[row][column] = plr;
                    res[plr] += stable_disc_weight;
                }
                limit = column;
            }
            limit = MAX_N;
            for(int row = MAX_N-1; row >= 0; row--){
                if(get_val({row,0}) != plr) break;
                int column;
                for(column = 0; column < limit; column++){
                    if(get_val({row,column}) != plr) break;
                    if(vis[row][column] == plr) continue;
                    vis[row][column] = plr;
                    res[plr] += stable_disc_weight;
                }
                limit = column;
            }
            limit = -1;
            for(int row = MAX_N-1; row >= 0; row--){
                if(get_val({row,MAX_N-1}) != plr) break;
                int column;
                for(column = MAX_N-1; column > limit; column--){
                    if(get_val({row,column}) != plr) break;
                    if(vis[row][column] == plr) continue;
                    vis[row][column] = plr;
                    res[plr] += stable_disc_weight;
                }
                limit = column;
            }
        }
        for(int row = 0; row < MAX_N; row++){
            for(int column = 0; column < MAX_N; column++){
                if(vis[row][column] || get_val({row,column}) == 0) continue;
                res[get_val({row,column})] += weights[row][column];
            }
        }

        return (res[player] - res[opponent]) + 2 * (possplr.size() - possopp.size());
    }

    // int dfs(pair<int,int> coord, vector< vector <int>> *vis, int plr, pair<int,int> cov_dir){
    //     if(!correct_coord(coord) || (*vis)[coord.st][coord.nd] == plr || 
    //     get_val(coord) != plr  || get_val(coord-dxy[cov_dir.st]) != plr || get_val(coord-dxy[cov_dir.st]) != plr) return 0;
    //     int ans = stable_disc_weight - weights[coord.st][coord.nd];
    //     (*vis)[coord.st][coord.nd] = plr;
    //     for(int dir = 0; dir < 8; dir++){
    //         coord = coord + dxy[dir];
    //         if(correct_coord(coord) && get_val(coord) == plr && (*vis)[coord.st][coord.nd] != plr)
    //             ans += dfs(coord, vis, plr);
    //         coord = coord - dxy[dir];
    //     }
    //     return ans;
    // }

    int result(int player){
        int opponent = (player & 1) + 1;
        return numof_discs[player] - numof_discs[opponent];
    }

    bool correct_coord(pair<int,int> coord){
        if(coord.st < 0 || coord.st >= MAX_N || coord.nd < 0 || coord.nd >= MAX_N) return false;
        return true;
    }

    bool correct_move(pair<int, int> coord, int player){
        int opponent = (player & 1) + 1;
        pair<int,int> new_coord;
        if(!correct_coord(coord) || get_val(coord)) return false;
        for(int dir = 0; dir < 8; dir++){
            new_coord = coord + dxy[dir];
            if(correct_coord(new_coord) && get_val(new_coord) == opponent){
                while(correct_coord(new_coord) && get_val(new_coord) == opponent)
                    new_coord = new_coord + dxy[dir];
                
                if(correct_coord(new_coord) && get_val(new_coord) == player) return true;
            }
        }
        return false;
    }

    void make_move(pair<int, int> coord, int player){
        //if(!correct_move(coord, player)) return;
        if(!correct_coord(coord) || get_val(coord)) return;
        
        int opponent = (player & 1) + 1;
        pair<int,int> new_coord;
        set_val(coord, player);
        numof_discs[player]++;
        weighted_discs[player] += weights[coord.st][coord.nd];

        for(int dir = 0; dir < 8; dir++){
            new_coord = coord + dxy[dir];
            while(correct_coord(new_coord) && get_val(new_coord) == opponent)
                new_coord = new_coord + dxy[dir];

            if(correct_coord(new_coord) && get_val(new_coord) == player){
                new_coord = new_coord - dxy[dir];
                while(correct_coord(new_coord) && get_val(new_coord) == opponent){
                    set_val(new_coord,player);
                    numof_discs[player]++;
                    numof_discs[opponent]--;

                    weighted_discs[player] += weights[new_coord.st][new_coord.nd];
                    weighted_discs[opponent] -= weights[new_coord.st][new_coord.nd];
                    new_coord = new_coord - dxy[dir];
                }
            }
        }
        //numof_discs[0] = numof_discs[1] - numof_discs[2];
    }

    bool end_state(){
        for(int row = 0; row < MAX_N; row++){
            for(int column = 0; column < MAX_N; column++){
                if(correct_move({row, column}, 1) || correct_move({row, column}, 2)) return false;
            }
        }
        return true;
    }

    void print_board(){
        for(auto row : board){
            for(auto field : row){
                switch(field){
                    case 0:
                        cerr << '.';
                        break;
                    case 1:
                        cerr << 'o';
                        break;
                    case 2:
                        cerr << '#';
                        break;
                }
            }
            cerr << "\n";
        }
    }
};

#endif
