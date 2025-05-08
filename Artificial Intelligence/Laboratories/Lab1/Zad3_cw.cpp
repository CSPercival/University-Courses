#include<bits/stdc++.h>
#include <ctime>
#define war first
#define kol second

using namespace std;

struct Hand{
    pair<int,int> card[5];
};

int NUM_OF_TESTCASES;

vector<pair<int,int>> fig_deck;
vector<pair<int,int>> blot_deck;

bool if_straight_flush(Hand);
bool if_quads(Hand);
bool if_full_house(Hand);
bool if_flush(Hand);
bool if_straight(Hand);
bool if_three(Hand);
bool if_two_pair(Hand);
bool if_pair(Hand);

bool if_straight_flush(Hand curr_hand){
    return if_straight(curr_hand) && if_flush(curr_hand);
}

bool if_quads(Hand curr_hand){
    return (curr_hand.card[0].war==curr_hand.card[3].war) || (curr_hand.card[1].war==curr_hand.card[4].war); 
}

bool if_full_house(Hand curr_hand){
    return ((curr_hand.card[0].war==curr_hand.card[1].war) && (curr_hand.card[2].war==curr_hand.card[4].war)) || 
    ((curr_hand.card[0].war==curr_hand.card[2].war) && (curr_hand.card[3].war==curr_hand.card[4].war));
}

bool if_flush(Hand curr_hand){
    for(int i=1;i<5;i++){
        if(curr_hand.card[i].kol!=curr_hand.card[i-1].kol) return false;
    }
    return true;
}

bool if_straight(Hand curr_hand){
    for(int i=1;i<5;i++){
        if(curr_hand.card[i].war!=curr_hand.card[i-1].war+1) return false;
    }
    return true;
}

bool if_three(Hand curr_hand){
    for(int i=0;i<3;i++){
        if(curr_hand.card[i].war==curr_hand.card[i+2].war) return true;
    }
    return false;
}

bool if_two_pair(Hand curr_hand){
    int ctr1=0;
    for(int i=1;i<5;i++){
        if(curr_hand.card[i].war==curr_hand.card[i-1].war) ctr1++;
    }
    if(ctr1==2) return true;
    return false;
}

bool if_pair(Hand curr_hand){
    for(int i=1;i<5;i++){
        if(curr_hand.card[i].war==curr_hand.card[i-1].war) return true;
    }
    return false;
}

int rate(Hand curr_hand){
    if(if_straight_flush(curr_hand)) return 8;
    if(if_quads(curr_hand)) return 7;
    if(if_full_house(curr_hand)) return 6;
    if(if_flush(curr_hand)) return 5;
    if(if_straight(curr_hand)) return 4;
    if(if_three(curr_hand)) return 3;
    if(if_two_pair(curr_hand)) return 2;
    if(if_pair(curr_hand)) return 1;
    return 0;
} 

bool blot_wins(Hand fig, Hand blot){
    int fig_val,blot_val;
    fig_val=rate(fig);
    blot_val=rate(blot);
    return blot_val>fig_val;
}

void generate_deck(){
    for(int i=11;i<15;i++){
        for(int j=1;j<5;j++){
            fig_deck.push_back({i,j});
        }
    }

    for(int i=2;i<11;i++){
        for(int j=1;j<5;j++){
            blot_deck.push_back({i,j});
        }
    }
    //blot_deck.push_back({10,2});
}

Hand generate_hand(vector<pair<int,int>> *ptrdeck){
    Hand new_hand;
    if((*ptrdeck).size()<5){ 
        cout << "Error: Deck is too small\n";
        new_hand.card[0]={-1,-1};
        return new_hand;
    }
    vector<pair<int,int>> tmpv;
    for(int i=0;i<5;i++){
        tmpv.push_back((*ptrdeck)[i]);
    }
    sort(tmpv.begin(),tmpv.end());
    for(int i=0;i<5;i++){
        new_hand.card[i]=tmpv[i];
    }
    return new_hand;
}

int tab_ans[10][2];     //0 - blot, 1 - fig
int num_of_test[10][2];
Hand f_hand;
void f(vector<pair<int,int>> *ptrdeck, int idx_h,int idx_d, int plr){
    if(idx_h==5){
        tab_ans[rate(f_hand)][plr]++;
        return;
    }
    if(idx_d==(*ptrdeck).size()) return;
    f(ptrdeck,idx_h,idx_d+1,plr);
    f_hand.card[idx_h]=(*ptrdeck)[idx_d];
    f(ptrdeck,idx_h+1,idx_d+1,plr);
    f_hand.card[idx_h]={0,0};
}

int main(){
    srand(unsigned(time(0)));
    generate_deck();
    //if(generate_hand((&fig_deck)).card[0].first==-1 || generate_hand((&blot_deck)).card[0].first==-1) return 0;
    /*
    NUM_OF_TESTCASES=100000;
    int tmp_test=NUM_OF_TESTCASES;
    int ctr1=0;
    while(tmp_test--){
        random_shuffle(fig_deck.begin(),fig_deck.end());
        random_shuffle(fig_deck.begin(),fig_deck.end());
        random_shuffle(blot_deck.begin(),blot_deck.end());
        random_shuffle(blot_deck.begin(),blot_deck.end());
        Hand fig_hand=generate_hand((&fig_deck));
        Hand blot_hand=generate_hand((&blot_deck));
        if(blot_wins(fig_hand,blot_hand)) ctr1++;
    }
    long double ans=ctr1;
    ans/=NUM_OF_TESTCASES;
    cout << ans << "\n";
    */

    f((&blot_deck),0,0,0);
    f((&fig_deck),0,0,1);
    num_of_test[0][0]=0;
    num_of_test[0][1]=0;
    for(int i=0;i<9;i++){
        cout << "tab_ans " << i << ": " << tab_ans[i][0] << " " << tab_ans[i][1] << endl;
        num_of_test[i][0]=tab_ans[i][0]+num_of_test[max(0,i-1)][0];
        num_of_test[i][1]=tab_ans[i][1]+num_of_test[max(0,i-1)][1];
    }
    for(int i=0;i<9;i++){
        cout << "num_ans " << i << ": " << num_of_test[i][0] << " " << num_of_test[i][1] << endl;
    }
    //cout << num_of_test[0] << " " << num_of_test[1] << endl;
    long long ans=0;
    for(int i=1;i<9;i++){
        ans+=tab_ans[i][0]*num_of_test[i-1][1];
    }
    long double final_ans = ans;
    final_ans/=(num_of_test[8][0]*num_of_test[8][1]);
    cout << "------------------------------------\n\n";
    cout << final_ans << endl;
    for(int i=0;i<9;i++){
        cout << fixed << setprecision(6) << "tab_ans " << i << ": " << ((long double)tab_ans[i][0])/((long double)num_of_test[8][0]) << " " << ((long double)tab_ans[i][1])/((long double)num_of_test[8][1]) << endl;
    }
}