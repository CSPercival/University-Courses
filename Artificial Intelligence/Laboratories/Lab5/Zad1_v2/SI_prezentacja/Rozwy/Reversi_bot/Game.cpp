#ifndef GAME_CPP
#define GAME_CPP

#include "Libraries_and_Macros.h"
#include "Functions.h"
#include "Board.cpp"
#include "Bot_template.cpp"

class Game{
    Board board;
    Bot *player1;
    Bot *player2;

    public:
    Game(Bot* player1, Bot* player2){
        this->player1 = player1;
        this->player2 = player2;
        //setup();
    }

    void setup(){
        board.setup();

        pair<string,pair<int,int>> ans;
        
        ans = player1->perform(ONEMORE_MESS,emptyP);
        if(ans.st != RDY_MESS){
            cerr << "PLAYER 1 IS NOT READY\n";
        }

        ans = player2->perform(ONEMORE_MESS,emptyP);
        if(ans.st != RDY_MESS){
            cerr << "PLAYER 2 IS NOT READY\n";
        }
    }

    int play(){
        int turn = 1;
        bool pass1 = false, pass2 = false;
        pair<string, pair<int,int>> answer;
        pair<int,int> move;

        /// ER cerr << "G -> P1: " << UGO_MESS << "\n"; 
        
        answer = player1->perform(UGO_MESS,emptyP);

        /// ER cerr << "P1 -> G: " << answer.st << " ";
        // for(auto arg : answer.nd) cerr << arg << " ";
        /// ER cerr << answer.nd.st << " " << answer.nd.nd;
        /// ER cerr << "\n";

        // if(answer.st == IDO_MESS){
        //     //move = {answer.nd[0],answer.nd[1]};
        //     if(board.correct_move(answer.nd,turn)){
        //         board.make_move(answer.nd,turn);
        //     } else {
        //         /// ER cerr << "INVALID MOVE BY PLAYER: " << turn << "\n";
        //     }
        // } else {
        //     cerr << "RECEIVE UNKNOWN MESSAGE AFTER PLAYER " << turn << " MOVE\n";
        // }
        if(answer.nd == emptyP)
            pass1 = true;
        turn = 2;
        /// ER board.print_board();

        while(!pass1 || !pass2){
            if(turn == 1){
                /// ER cerr << "G -> P1: " << HEDID_MESS << " ";
                // for(auto arg : answer.nd) cerr << arg << " ";
                /// ER cerr << answer.nd.st << " " << answer.nd.nd;
                /// ER cerr << "\n"; 
                
                answer = player1->perform(HEDID_MESS,answer.nd);
                if(answer.nd == emptyP)
                    pass1 = true;
                else
                    pass1 = false;
                
                /// ER cerr << "P1 -> G: " << answer.st << " ";
                // for(auto arg : answer.nd) cerr << arg << " ";
                /// ER cerr << answer.nd.st << " " << answer.nd.nd;
                /// ER cerr << "\n";

                // if(answer.st == IDO_MESS){
                //     if(board.correct_move(answer.nd,turn)){
                //         board.make_move(answer.nd,turn);
                //     } else {
                //         /// ER cerr << "INVALID MOVE BY PLAYER 1\n";
                //     }
                // } else {
                //     cerr << "RECEIVE UNKNOWN MESSAGE AFTER PLAYER 1'S MOVE\n";
                // }

                turn = 2;
                /// ER board.print_board();
                continue;
            }
            if(turn == 2){
                /// ER cerr << "G -> P2: " << HEDID_MESS << " ";
                // for(auto arg : answer.nd) cerr << arg << " ";
                /// ER cerr << answer.nd.st << " " << answer.nd.nd;
                /// ER cerr << "\n"; 
                
                answer = player2->perform(HEDID_MESS,answer.nd);
                if(answer.nd == emptyP)
                    pass2 = true;
                else
                    pass2 = false;
                /// ER cerr << "P2 -> G: " << answer.st << " ";
                // for(auto arg : answer.nd) cerr << arg << " ";
                /// ER cerr << answer.nd.st << " " << answer.nd.nd;
                /// ER cerr << "\n";

                // if(answer.st == IDO_MESS){
                //     if(board.correct_move(answer.nd,turn)){
                //         board.make_move(answer.nd,turn);
                //     } else {
                //         /// ER cerr << "INVALID MOVE BY PLAYER 2\n";
                //     }
                // } else {
                //     cerr << "RECEIVE UNKNOWN MESSAGE AFTER PLAYER 2'S MOVE\n";
                // }

                turn = 1;
                /// ER board.print_board();
                continue;
            }
        }

        /// ER cerr << "Game over\n";
        /// ER cerr << "Score: " << board.result(1) << "\n";
        return (player1->perform(RESULT_MESS,emptyP)).nd.st;
    }
};

#endif