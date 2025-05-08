#ifndef NODE_MCTS_CPP
#define NODE_MCTS_CPP

#include "Libraries_and_Macros.h"
#include "Functions.h"
#include "Board.cpp"

class Node_mcts{
    private:
    int node_value;
    double C_const;
    int numof_vis;
    int player;
    int opponent;
    Node_mcts* parent;
    vector<Node_mcts> childs;
    Board board;

    public:
    pair<int,int> move_that_get_me_here;

    Node_mcts(double c, Board* brd, pair<int,int> move, int plr, Node_mcts* par){
        node_value = 0;
        C_const = sqrt(c);
        numof_vis = 0;

        parent = par;
        childs.clear();
        
        opponent = plr;
        player = (opponent & 1) + 1;

        board = (*brd);
        board.make_move(move,opponent);
        move_that_get_me_here = move;
    }

    double eval(int t){
        if(numof_vis == 0) {
            return 1000.0;
        }
        assert((t > 0));
        assert((numof_vis > 0));
        return (double)node_value/(double)numof_vis + C_const * sqrt(log(t)/(double)numof_vis);
    }

    Node_mcts* create_childs(){
        for(int row = 0; row < MAX_N; row++){
            for(int column = 0; column < MAX_N; column++){
                if(board.correct_move({row, column}, player)){
                    Node_mcts child_new(C_const,(&board),{row,column},player,this);
                    childs.push_back(child_new);
                }
            }
        }
        if(childs.empty()){
            Node_mcts empty_child(C_const,(&board),{-1,-1},player,this);
            childs.push_back(empty_child);
            // return (&empty_child);
        }
        return (&(childs[0]));
    }

    Node_mcts* select_node(){
        // if(board.end_state()){
        //     return this;
        // }
        // if(numof_vis == 0){
        //     return this;
        // }
        if(childs.empty()){
            if(numof_vis == 0 || board.end_state()) return this;
            return create_childs();
        }
        Node_mcts* ans_node = (&(childs[0]));
        for(int i = 0; i < childs.size(); i++){
            if((*ans_node).eval(numof_vis) < childs[i].eval(numof_vis)){
                ans_node = (&childs[i]);
            }
        }
        return (*ans_node).select_node();
    }

    // Node_mcts* best_node(){
    //     assert((!childs.empty()));
    //     Node_mcts* ans_node = (&(childs[0]));
    //     for(auto node : childs){
    //         if((*ans_node).eval(numof_vis) < node.eval(numof_vis)){
    //             ans_node = (&node);
    //         }
    //     }
    //     return ans_node;
    // }

    pair<int,int> best_move(){
        if(childs.empty()) return {-1,-1};
        assert((!childs.empty()));
        Node_mcts* ans_node = (&(childs[0]));
        for(auto node : childs){
            if((*ans_node).eval(numof_vis) < node.eval(numof_vis)){
                ans_node = (&node);
            }
        }
        return (*ans_node).move_that_get_me_here;
    }

    int simulate(int plr){
        Board tbrd = board;
        return simulate_aux((&tbrd),player,plr);
    }

    int simulate_aux(Board* tbrd, int plr, int res_plr){
        if((*tbrd).end_state()){
            int res = (*tbrd).result(res_plr);
            if(res > 0) return 1;
            if(res < 0) return 0;
            if(res == 0) return 0;
        }
        vector<pair<int,int>> correct_moves;
        for(int row = 0; row < MAX_N; row++){
            for(int column = 0; column < MAX_N; column++){
                if((*tbrd).correct_move({row, column}, plr)){
                    correct_moves.push_back({row,column});
                }
            }
        }
        if(correct_moves.empty()){
            correct_moves.push_back({-1,-1});
        }
        (*tbrd).make_move(correct_moves[random_int(0,correct_moves.size())],plr);
        return simulate_aux(tbrd,(plr & 1) + 1,res_plr);
    }

    void backpropagation(int res){
        // cerr << "BACKPROPAG: " << move_that_get_me_here.st << " - " << move_that_get_me_here.nd << "\n";
        // node_value *= numof_vis;
        // node_value += res;
        // numof_vis++;
        // node_value /= (double)numof_vis;
        node_value += res;
        numof_vis++;
        if(parent == nullptr) return;
        (*parent).backpropagation(res);
        return;
    }

};

#endif