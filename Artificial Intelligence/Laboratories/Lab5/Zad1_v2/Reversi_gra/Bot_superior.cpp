#ifndef BOT_SUPERIOR_CPP
#define BOT_SUPERIOR_CPP

#include "Bot_template.cpp"
#include "Functions.h"

class Superior: public Bot{
    public:
    Superior(vector<int> sp) : Bot(sp){}

    pair<int,int> choose_move(){
        // cerr << "SUPERITO WYBIERA RUCH -------------------------------------------------------------------------------------------------------------------\n";
        // vector<int> answer = minmax(board,3,true);
        // answer.pop_back();
        // return answer;
        int ans = INT_MIN;
        int tans;
        pair<int,int> best_move = emptyP;
        
        Board tbrd;

        for(int row = 0; row < MAX_N; row++){
            for(int column = 0; column < MAX_N; column++){
                if(board.correct_move({row, column}, player)){
                    //possible_moves.push_back({row, column});
                    //tbrd = board;
                    //tbrd.make_move({row, column}, player);    
                    tans = minmax(board,{row, column},MAX_DEPTH-1,false);
                    // cerr << "RUCH: " << row << " - " << column << "  okresil na:  " << tans << "\n";
                    if(tans > ans){
                        // cerr << "PRZYJELO SIE\n";
                        ans = tans;
                        best_move = {row,column};
                    }
                }
            }
        }
        return best_move;
    }

    void reset(){
        setup();
    }

    int minmax(Board brd,pair<int,int> opp_move, int dep, bool max_plr){
        if(max_plr) brd.make_move(opp_move,opponent);
        else brd.make_move(opp_move,player); 

        if(brd.end_state()){
            if(brd.result(player) > 0) return (INT_MAX - 1);
            if(brd.result(player) < 0) return (INT_MIN + 1);
            return 0;
        }
        if(dep == 0){
            int tmp = brd.evaluate(player);
            // cerr << "KONIEC MINMAXA\n";
            // brd.print_board();
            // cerr << "OCENA NA: " << tmp << "\n";
            return tmp;
        }

        Board tbrd;

        if(max_plr){
            int max_ans = INT_MIN;
            for(int row = 0; row < MAX_N; row++){
                for(int column = 0; column < MAX_N; column++){
                    if(brd.correct_move({row, column}, player)){
                        //possible_moves.push_back({row, column});
                        //tbrd = brd;
                        //tbrd.make_move({row, column}, player);    
                        max_ans = max(max_ans,minmax(brd,{row, column},dep-1,false));
                    }
                }
            }
            return max_ans;
        } else {
            int min_ans = INT_MAX;
            for(int row = 0; row < MAX_N; row++){
                for(int column = 0; column < MAX_N; column++){
                    if(brd.correct_move({row, column}, opponent)){
                        //possible_moves.push_back({row, column});
                        //tbrd = brd;
                        //tbrd.make_move({row, column}, opponent);    
                        // tans = minmax(tbrd,dep-1,true);
                        min_ans = min(min_ans,minmax(brd,{row, column},dep-1,true));
                    }
                }
            }
            return min_ans;
        }
    }
};

#endif