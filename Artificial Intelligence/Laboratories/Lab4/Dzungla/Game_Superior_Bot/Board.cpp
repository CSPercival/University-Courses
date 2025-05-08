#ifndef BOARD_CPP
#define BOARD_CPP

#include "Libraries_and_Macros.h"
#include "Functions.h"

class Board{
    vector <vector <pair <int,int>>> board;
    
    public:
    Board(){
        
    }

    void setup(){
        board = start_pos;
    }

    pair<int,int> get_field(pair<int,int> coord){ return board[coord.st][coord.nd]; }
    void set_field(pair<int,int> coord, pair<int,int> val){ board[coord.st][coord.nd] = val; }

    int evaluate(int player){
        
    }

    int result(int player){
        
    }

    bool correct_coord(pair<int,int> coord){
        if(coord.st < 0 || coord.st >= MAX_N || coord.nd < 0 || coord.nd >= MAX_M) return false;
        return true;
    }

    bool correct_move(pair<int, int> coord1, pair<int, int> coord2, int player){
        int opponent = (player & 1) + 1;
        pair<int,int> start, finish;
        if(!correct_coord(coord1) || !correct_coord(coord2)) return false;
        start = get_field(coord1);
        finish = get_field(coord2);
        if(start == emptyP) return false;
        if(start.nd != player || start == make_pair(0,1) || finish.nd == player) return false;
        
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
