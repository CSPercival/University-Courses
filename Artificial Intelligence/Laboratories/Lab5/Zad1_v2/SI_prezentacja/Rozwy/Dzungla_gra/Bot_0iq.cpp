#ifndef BOT_0IQ_CPP
#define BOT_0IQ_CPP

#include "Bot_template.cpp"
#include "Functions.h"

class OIQ: public Bot{
    public:
    OIQ() : Bot(){}

    pair< pair<int,int>, pair<int,int>> choose_move(){
        vector< pair< pair<int,int>, pair<int,int>> > possible_moves = board.possible_moves(player);
        //possible_moves.push_back({-1,-1});
        if(possible_moves.size() == 0){
            //possible_moves.push_back({{-1,-1},{-1,-1}});
            return {{-1,-1},{-1,-1}};
        }
        pair< pair<int,int>, pair<int,int>> best_move = possible_moves[random_int(0,possible_moves.size())];
        return best_move; 
    }

    void reset(){
        setup();
    }

};

#endif