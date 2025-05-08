#ifndef BOT_TEMPLATE_CPP
#define BOT_TEMPLATE_CPP

#include "Board.cpp"

class Bot{
    protected:
    Board board;
    int player,opponent;
    vector<int> special_values;
    
    public:
    Bot(vector<int> sp){
        //setup();
        special_values = sp;
    }

    void setup(){
        board.setup();
        player = 2;
        opponent = 1;
    }

    void change_colors(){ swap(player,opponent); }

    pair<string,pair<int,int>> perform(string message,pair<int,int> args){
        if(message == UGO_MESS){
            change_colors();
            
            pair<int,int> my_move = choose_move();
            board.make_move(my_move,player);

            return {IDO_MESS,my_move};
        }
        if(message == HEDID_MESS){
            board.make_move(args,opponent);
            
            pair<int,int> my_move = choose_move();
            board.make_move(my_move,player);

            return {IDO_MESS,my_move};
        }
        if(message == ONEMORE_MESS){
            reset();
            return {RDY_MESS,emptyP};
        }
        if(message == BYE_MESS){
            return {BYE_MESS,emptyP};
        }
        if(message == RESULT_MESS){
            return {RESULT_MESS,{board.result(player),0}};
        }
        return {ERROR_MESS,emptyP};
    }

    //virtual pair<string, vector<int>> perform(string,vector<int>) = 0;
    virtual pair<int,int> choose_move() = 0;
    virtual void reset() = 0;
};

#endif