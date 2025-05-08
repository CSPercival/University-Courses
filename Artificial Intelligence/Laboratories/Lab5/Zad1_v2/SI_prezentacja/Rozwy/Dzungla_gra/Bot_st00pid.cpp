#ifndef BOT_ST00PID_CPP
#define BOT_ST00PID_CPP

#include "Bot_template.cpp"
#include "Functions.h"

class St00pid: public Bot{
    public:
    St00pid() : Bot(){}

    pair< pair<int,int>, pair<int,int>> choose_move(){
        cerr << "STUPIDO WYBIERA\n"; 
        vector< pair< pair<int,int>, pair<int,int>> > possible_moves = board.possible_moves(player);
        //possible_moves.push_back({-1,-1});
        if(possible_moves.size() == 0){
            //possible_moves.push_back({{-1,-1},{-1,-1}});
            return {{-1,-1},{-1,-1}};
        }

        int ans = INT_MIN;
        pair< pair<int,int>, pair<int,int>> best_move = {{-1,-1},{-1,-1}};
        int smol_max_nmoves = MAX_NMOVES/possible_moves.size()+1;
        for(auto move : possible_moves){
            int tmp = check(board,move.st,move.nd, smol_max_nmoves);
            if(ans < tmp){
                ans = tmp;
                best_move = move;
            }
        }

        //pair<int,int> my_move = possible_moves[random_int(0,possible_moves.size())];
        //board.make_move(my_move,player);
        
        return best_move; 
    }

    void reset(){
        setup();
    }

    int check(Board brd, pair<int,int> coordS, pair<int,int> coordF, int limit){
        // cerr << "CHECK: " << coordS.st << " - " << coordS.nd << "   " << coordF.st << " - " << coordF.nd << "\n";
        brd.make_move(coordS,coordF,player);
        int turn = opponent;
        vector< pair< pair<int,int>, pair<int,int>> > possible_moves;
        int ans = 0;
        int ctr = 0;
        Board tbrd;
        if(brd.end_state()){
            if(brd.result(player) > 0) return INT_MAX - 1;
            if(brd.result(player) < 0) return INT_MIN + 1;
            return 0;
        }

        while(ctr < limit){
            turn = opponent;
            tbrd = brd;
            while(ctr < limit && !tbrd.end_state()){
                ctr++;
                possible_moves = tbrd.possible_moves(turn);
                if(possible_moves.size() == 0){
                    possible_moves.push_back({{-1,-1},{-1,-1}});
                }
                pair< pair<int,int>, pair<int,int>> move = possible_moves[random_int(0,possible_moves.size())];
                tbrd.make_move(move.st,move.nd,turn);
                turn = (turn & 1) + 1;
            }
            ans += tbrd.result(player);
        }
        return ans;
    }

};

#endif