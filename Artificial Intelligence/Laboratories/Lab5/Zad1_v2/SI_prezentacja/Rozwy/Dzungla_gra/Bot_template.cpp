#ifndef BOT_TEMPLATE_CPP
#define BOT_TEMPLATE_CPP

#include "Board.cpp"

class Bot{
    protected:
    Board board;
    int player,opponent;
    
    public:
    Bot(){
        //setup();
    }

    void setup(){
        board.setup();
        player = 2;
        opponent = 1;
    }

    void change_colors(){ swap(player,opponent); }

    pair<string,pair< pair<int,int>, pair<int,int>>> perform(string message,pair< pair<int,int>, pair<int,int>> args){
        if(message == UGO_MESS){
            change_colors();
            
            pair< pair<int,int>, pair<int,int>> my_move = choose_move();
            board.make_move(my_move.st,my_move.nd,player);

            return {IDO_MESS,my_move};
        }
        if(message == HEDID_MESS){
            board.make_move(args.st,args.nd,opponent);
            
            pair< pair<int,int>, pair<int,int>> my_move = choose_move();
            board.make_move(my_move.st,my_move.nd,player);

            return {IDO_MESS,my_move};
        }
        if(message == ONEMORE_MESS){
            reset();
            return {RDY_MESS,emptyPP};
        }
        if(message == BYE_MESS){
            return {BYE_MESS,emptyPP};
        }
        return {ERROR_MESS,emptyPP};
    }

    //virtual pair<string, vector<int>> perform(string,vector<int>) = 0;
    virtual pair< pair<int,int>, pair<int,int>> choose_move() = 0;
    virtual void reset() = 0;
};

#endif