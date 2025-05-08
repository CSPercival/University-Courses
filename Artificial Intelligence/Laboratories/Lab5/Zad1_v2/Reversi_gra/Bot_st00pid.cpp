#ifndef BOT_ST00PID_CPP
#define BOT_ST00PID_CPP

#include "Bot_template.cpp"
#include "Functions.h"

class St00pid: public Bot{
    public:
    St00pid(vector<int> sp) : Bot(sp){}

    pair<int,int> choose_move(){
        vector< pair<int,int> > possible_moves;
        for(int row = 0; row < MAX_N; row++){
            for(int column = 0; column < MAX_N; column++){
                if(board.correct_move({row, column}, player)){
                    possible_moves.push_back({row, column});
                }
            }
        }
        //possible_moves.push_back({-1,-1});
        if(possible_moves.size() == 0){
            possible_moves.push_back({-1,-1});
        }
        pair<int,int> my_move = possible_moves[random_int(0,possible_moves.size())];

        //board.make_move(my_move,player);
        
        return my_move; 
    }

    void reset(){
        setup();
    }
};

#endif