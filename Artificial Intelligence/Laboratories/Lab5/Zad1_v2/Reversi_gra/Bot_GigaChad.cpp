#ifndef BOT_GIGACHAD_CPP
#define BOT_GIGACHAD_CPP

#include "Bot_template.cpp"
#include "Functions.h"
#include "Node_mcts.cpp"

class Gigachad: public Bot{
    public:
    Gigachad(vector<int> sp) : Bot(sp){}

    pair<int,int> choose_move(){
        // cerr << "GIGACHAD WYBIERA\n";
        Node_mcts root(special_values[0],(&board),{-1,-1},opponent,nullptr);
        Node_mcts* selected_node;
        int result;
        int ctr1 = 0;
        // cerr << "URUCHOMIC MCTS\n";
        while(ctr1 < special_values[1]){
            // cerr << "DZIALAM1\n";
            selected_node = root.select_node();
            //assert((selected_node != nullptr));
            // cerr << "WYBRANO CHWILOWO: " << (*selected_node).move_that_get_me_here.st << " - " << (*selected_node).move_that_get_me_here.nd << "\n";
            result = (*selected_node).simulate(player);
            // cerr << "GRA ZAKONCZONA WYNIKIEM: " << result << endl;
            (*selected_node).backpropagation(result);
            ctr1++;
            // cerr << "DZIALAM2\n";
        }
        pair<int,int> best_move = root.best_move();
        return best_move;
        //Node_mcts* ans_node = root.best_node();
        // cerr << "WYBRANO : " << (*ans_node).move_that_get_me_here.st << " - " << (*ans_node).move_that_get_me_here.nd << "\n";
        //return (*ans_node).move_that_get_me_here;
    }

    void reset(){
        setup();
    }
};

#endif