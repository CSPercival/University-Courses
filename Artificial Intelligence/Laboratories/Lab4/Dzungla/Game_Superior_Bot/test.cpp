#include <bits/stdc++.h>
//#include <assert.h>
//#include <thread>

//#define wait(x) this_thread::sleep_for((x)*1ms);
#define st first
#define nd second

using namespace std;

const int MAX_N = 8;
const string UGO_MESS =     "UGO";
const string HEDID_MESS =   "HEDID";
const string ONEMORE_MESS = "ONEMORE";
const string BYE_MESS =     "BYE";
const string RDY_MESS =     "RDY";
const string IDO_MESS =     "IDO";
const vector<int> emptyV;

const vector <pair <int,int>> dxy{{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};

pair<int,int> operator+(pair<int,int> const& p1, pair<int,int> const& p2){
    return {p1.st+p2.st, p1.nd+p2.nd};
}

pair<int,int> operator-(pair<int,int> const& p1, pair<int,int> const& p2){
    return {p1.st-p2.st, p1.nd-p2.nd};
}

int random_int(int l, int r){
    return (rand()%(r-l))+l;
}











class Board{
    vector <vector <int>> board;
    vector <int> numof_discs;
    //int numof_dark,numof_light;
    public:
    Board(){
        setup();
    }

    void setup(){
        board = vector <vector <int>> (MAX_N, vector <int> (MAX_N, 0));
        board[3][3] = 2;
        board[3][4] = 1;
        board[4][3] = 1;
        board[4][4] = 2;

        numof_discs.resize(3);
        numof_discs[0] = 0;
        numof_discs[1] = 2;
        numof_discs[2] = 2;
    }

    int get_val(pair<int,int> coord){ return board[coord.st][coord.nd]; }
    void set_val(pair<int,int> coord, int val){ board[coord.st][coord.nd] = val; }

    int evaluate(int player){ 
        if(player == 1) return numof_discs[0];
        return numof_discs[0]*-1; 
    }

    bool correct_coord(pair<int,int> coord){
        if(coord.st < 0 || coord.st >= MAX_N || coord.nd < 0 || coord.nd >= MAX_N) return false;
        return true;
    }

    bool correct_move(pair<int, int> coord, int player){
        int opponent = (player & 1) + 1;
        pair<int,int> new_coord;
        if(!correct_coord(coord) || get_val(coord)) return false;
        for(int dir = 0; dir < 8; dir++){
            new_coord = coord + dxy[dir];
            if(correct_coord(new_coord) && get_val(new_coord) == opponent){
                while(correct_coord(new_coord) && get_val(new_coord) == opponent)
                    new_coord = new_coord + dxy[dir];
                
                if(correct_coord(new_coord) && get_val(new_coord) == player) return true;
            }
        }
        return false;
    }

    void make_move(pair<int, int> coord, int player){
        if(!correct_move(coord, player)) return;
        
        int opponent = (player & 1) + 1;
        pair<int,int> new_coord;
        set_val(coord, player);
        numof_discs[player]++;

        for(int dir = 0; dir < 8; dir++){
            new_coord = coord + dxy[dir];
            while(correct_coord(new_coord) && get_val(new_coord) == opponent)
                new_coord = new_coord + dxy[dir];

            if(correct_coord(new_coord) && get_val(new_coord) == player){
                new_coord = new_coord - dxy[dir];
                while(correct_coord(new_coord) && get_val(new_coord) == opponent){
                    set_val(new_coord,player);
                    numof_discs[player]++;
                    numof_discs[opponent]--;
                    new_coord = new_coord - dxy[dir];
                }
            }
        }
        numof_discs[0] = numof_discs[1] - numof_discs[2];
    }

    bool end_state(){
        for(int row = 0; row < MAX_N; row++){
            for(int column = 0; column < MAX_N; column++){
                if(correct_move({row, column}, 1) || correct_move({row, column}, 2)) return false;
            }
        }
        return true;
    }

    void print_board(){
        for(auto row : board){
            for(auto field : row){
                switch(field){
                    case 0:
                        printf(".");
                        break;
                    case 1:
                        printf("o");
                        break;
                    case 2:
                        printf("#");
                        break;
                }
            }
            printf("\n");
        }
    }
};

