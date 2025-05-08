#ifndef BOT_SUPERIOR_CPP
#define BOT_SUPERIOR_CPP

#include "Bot_template.cpp"
#include "Functions.h"

class Superior: public Bot{
    public:
    Superior() : Bot(){}

    pair< pair<int,int>, pair<int,int>> choose_move(){
        cerr << "SUPERIOR WYBIERA\n";
        vector< pair< pair<int,int>, pair<int,int>> > possible_moves = board.possible_moves(player);
        if(possible_moves.size() == 0){
            return {{-1,-1},{-1,-1}};
        }

        int ans = INT_MIN;
        pair< pair<int,int>, pair<int,int>> best_move = {{-1,-1},{-1,-1}};
        for(auto move : possible_moves){
            int tmp = minmax(board,move, MAX_DEPTH - 1, false);
            if(ans < tmp){
                ans = tmp;
                best_move = move;
            }
        }
        return best_move;
    }

    void reset(){
        setup();
    }

    int minmax(Board brd,pair< pair<int,int>, pair<int,int>> opp_move, int dep, bool max_plr){
        if(max_plr) brd.make_move(opp_move.st,opp_move.nd,opponent);
        else brd.make_move(opp_move.st,opp_move.nd,player); 

        if(brd.end_state()){
            //cerr << "KONIEC1\n";
            if(brd.result(player) > 0) return BEST_SCENARIO;
            if(brd.result(player) < 0) return WORST_SCENARIO;
            return 0;
        }
        if(dep == 0){
            //cerr << "KONIEC2\n";
            //int tmp = brd.evaluate(player);
            // cerr << "KONIEC MINMAXA\n";
            // brd.print_board();
            // cerr << "OCENA NA: " << tmp << "\n";
            return brd.evaluate(player);
        }

        if(max_plr){
            int max_ans = INT_MIN;
            vector< pair< pair<int,int>, pair<int,int>> > possible_moves = board.possible_moves(player);
            if(possible_moves.size() == 0){
                possible_moves.push_back({{-1,-1},{-1,-1}});
            }
            for(auto move : possible_moves){
                max_ans = max(max_ans, minmax(brd, move, dep - 1, false));
            }

            return max_ans;
        } else {
            int min_ans = INT_MAX;
            vector< pair< pair<int,int>, pair<int,int>> > possible_moves = board.possible_moves(opponent);
            if(possible_moves.size() == 0){
                possible_moves.push_back({{-1,-1},{-1,-1}});
            }
            for(auto move : possible_moves){
                min_ans = min(min_ans, minmax(brd, move, dep - 1, true));
            }
            return min_ans;
        }
    }
};

#endif