#ifndef BOARD_CPP
#define BOARD_CPP

#include "Libraries_and_Macros.h"
#include "Functions.h"

class Board{
    vector <vector <pair <int,int>>> board;
    map<pair<int,int>,pair<int,int>> pieces;
    
    public:
    Board(){

    }

    void setup(){
        board = start_pos;
        // for(int plr = 1; plr <= 2; plr++){
        //     for(int piec = 0; piec <= 8; piec++){
        //         pieces[plr].insert({piec,plr});
        //     }
        // }
        pieces.clear();
        for(int row = 0; row < MAX_N; row++){
            for(int column = 0; column < MAX_M; column++){
                pair<int,int> tmp = get_field({row,column});
                if(tmp != emptyP){
                    pieces[tmp] = {row,column};
                }
            }
        }
    }

    pair<int,int> get_field(pair<int,int> coord){ return board[coord.st][coord.nd]; }
    void set_field(pair<int,int> coord, pair<int,int> val){ board[coord.st][coord.nd] = val; }

    vector<pair<int,int>> pieces_positions(int player){
        vector<pair<int,int>> ans;
        for(auto it : pieces){
            if(it.st.nd == player && it.st.st != 0){
                ans.push_back(it.nd);
            }
        }
        return ans;
    }

    vector< pair< pair<int,int>, pair<int,int>> > possible_moves(int player){
        vector< pair< pair<int,int>, pair<int,int>> > poss_mov;
        vector< pair<int,int> > piec_pos = pieces_positions(player);
        pair<int,int> new_coord;
        for(auto it : piec_pos){
            if(it.st == 6 || it.st == 7){
                for(int dir = 0; dir < 4; dir++){
                    new_coord = it;
                    while(correct_coord(new_coord + dxy[dir])){
                        new_coord = new_coord + dxy[dir];
                        if(correct_move(it,new_coord, player)){
                            poss_mov.push_back({it,new_coord});
                        }
                    }
                }
            } else {
                for(int dir = 0; dir < 4; dir++){
                    if(correct_move(it,it + dxy[dir], player)){
                        poss_mov.push_back({it,it + dxy[dir]});
                    }
                }
            }
            // for(int row = 0; row < MAX_N; row++){
            //     for(int column = 0; column < MAX_M; column++){
            //         if(correct_move(it,{row, column}, player)){
            //             poss_mov.push_back({it,{row, column}});
            //         }
            //     }
            // }
        }
        return poss_mov;
    }

    int evaluate(int player){
        int opponent = (player & 1) + 1;
        vector<int> res(3,0);
        for(auto it : pieces){
            res[it.st.nd] += (it.st.st+2)*(it.st.st+2);
        }
        vector<int> res2(3,0);
        vector<pair<int,int>> ppp,ppo;
        ppp = pieces_positions(player);
        ppo = pieces_positions(opponent);

        for(auto coord : ppp){
            pair<int,int> tmp = coord - pieces[make_pair(0,opponent)];
            res2[player] += abs(tmp.st) + abs(tmp.nd);
        }

        for(auto coord : ppo){
            pair<int,int> tmp = coord - pieces[make_pair(0,player)];
            res2[opponent] += abs(tmp.st) + abs(tmp.nd);
        }

        if(ppp.size() == 0) return INT_MIN + 1;
        if(ppo.size() == 0) return INT_MAX - 1 - res2[player];

        res2[player] *= 10;
        res2[player] /= ppp.size();
        res2[opponent] *= 10;
        res2[opponent] /= ppo.size();

        return res[player] - res[opponent] + res2[opponent] - res2[player]; 
        
    }

    int result(int player){
        int opponent = (player & 1) + 1;
        if(!pieces.count({0,player})) return -1;
        if(!pieces.count({0,opponent})) return 1;
        return 0;
    }

    bool correct_coord(pair<int,int> coord){
        if(coord.st < 0 || coord.st >= MAX_N || coord.nd < 0 || coord.nd >= MAX_M) return false;
        return true;
    }

    bool correct_move(pair<int, int> coordS, pair<int, int> coordF, int player){
        int opponent = (player & 1) + 1;
        pair<int,int> start, finish;
        if(!correct_coord(coordS) || !correct_coord(coordF)) return false;                      // koordynaty mieszcza sie w planszy

        if(coordS == coordF) return false;
        start = get_field(coordS);
        finish = get_field(coordF);
        if(start == emptyP) return false;
        if(start.nd != player || start == make_pair(0,1) || finish.nd == player) return false;  // Wiemy ze ruch jest bierka playera i idzie na pole gdzie playera nie ma ani playerowskiej jamy
        if(start.st != 1 && obstacle(coordF) == river) return false;                            // Tylko szczur moze wchodzic do wody
        bool close_move = false;
        for(int dir = 0; dir < 4; dir++){
            if(coordS + dxy[dir] == coordF){
                close_move = true;
                break;
            }
        }
        if(!close_move){
            if(start.st != 6 && start.st != 7) return false;                                    // ruchy musi byc gora,dol,prawo,lewo
            bool all_water = false;                                                             // chyba ze to lew i tygrys
            if(coordS.st == coordF.st){
                all_water = true;                                                               // skok nastepuje wzdluz osi X
                for(int i = min(coordS.nd,coordF.nd)+1; i <= max(coordS.nd,coordF.nd)-1; i++){
                    if(obstacle({coordS.st,i}) != river || get_field({coordS.st,i}) == make_pair(1,opponent)){
                        all_water = false;
                        break;
                    }
                }
                //if(!all_water) return false;                                  
            }
            if(coordS.nd == coordF.nd){
                all_water = true;                                                               // skok nastepuje wzdluz osi Y
                for(int i = min(coordS.st,coordF.st)+1; i <= max(coordS.st,coordF.st)-1; i++){
                    if(obstacle({i,coordS.nd}) != river || get_field({i,coordS.nd}) == make_pair(1,opponent)){
                        all_water = false;
                        break;
                    }
                }
                // if(all_water) return true;
            }
            if(!all_water) return false;                                                        // skok jest albo po ukosie, albo nad ladem, albo nad szczurem przeciwnika
        }

        if(finish.nd == opponent && finish.st != 0){                                                            //finish zawiera bierke opponenta
            //if(obstacle(coordS) == trap) return false;
            if(obstacle(coordF) != trap){                                                               //finish nie stoi na pulapce
                if(finish.st > start.st ){                                                              // finish jest mocniejszy
                    if(start.st != 1 || finish.st != 8) return false;                                               // wszystko oprocz pojedynku szczur-slon odpada
                } else {
                    if(start.st == 8 && finish.st == 1) return false;                                   // jesli start jest mocniejszy ale to pojedynek szczur-slon to koniec
                }
            }
            //if(finish.st < start.st && obstacle(coordF) != trap){} return false;                                  // jezeli finish nie jest pulapka to nie mozna bic slabszymi figurami mocniejszych
            if(start.st == 1 && obstacle(coordS) == river && obstacle(coordF) != river) return false;           // szczur nie moze atakowac z wody na lad
        }
        return true;
    }

    void make_move(pair<int, int> coordS, pair<int, int> coordF, int player){
        if(!correct_move(coordS,coordF, player)) return;
        
        int opponent = (player & 1) + 1;
        if(get_field(coordF).nd == opponent){
            pieces.erase(get_field(coordF));
        }
        pieces[get_field(coordS)] = coordF;
        set_field(coordF,get_field(coordS));
        set_field(coordS,emptyP);
    }

    bool end_state(){
        if(!pieces.count({0,1}) || !pieces.count({0,2})) return true;
        return false;
    }

    void print_board(){
        // for(auto row : board){
        //     for(auto field : row){
        //         if(field.st == 0){
        //             cerr << obstacle(field);
        //         } else {
        //             cerr << ntp[field.nd-1][field.st];
        //         }
        //     }
        //     cerr << "\n";
        // }
        for(int row = 0; row < MAX_N; row++){
            for(int column = 0; column < MAX_M; column++){
                auto field = get_field({row,column});
                if(field.st == 0){
                    cerr << obstacle({row,column});
                } else {
                    cerr << ntp[field.nd-1][field.st];
                }
            }
            cerr << "\n";
        }
    }
};

#endif
