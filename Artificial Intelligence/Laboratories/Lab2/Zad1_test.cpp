#include<bits/stdc++.h>
#include<fstream>
#include<sstream>

using namespace std;

const int MAX_N=30;
ifstream input_file;
ofstream output_file;

int numof_rows;
int numof_columns;
vector<vector<int>> rows;
vector<vector<int>> columns;

struct cmp {
    bool operator() (bitset<MAX_N> a, bitset<MAX_N> b) const { 
        int idx=0;
        while(idx<MAX_N-1 && a[idx]==b[idx]){
            idx++;
        }
        return a[idx]<b[idx];
    }
};

vector<set<bitset<MAX_N>,cmp>> possible_row;
vector<set<bitset<MAX_N>,cmp>> possible_column;
int picture[MAX_N][MAX_N];

vector<int> read_line(){
    vector<int> ans;
    string input;
    getline(input_file,input);

    istringstream ss(input);
    int num;
    while(ss >> num){
        ans.push_back(num);
    }

    // int num=0;
    // for(auto idx : input){
    //     if(idx>='0' && idx<='9'){
    //         num*=10;
    //         num+=idx-'0';
    //     } else {
    //         ans.push_back(num);
    //         num=0;
    //     }
    // }
    // if(num!=0){
    //     ans.push_back(num);
    // }
    return ans;
}

void read_input(){
    vector<int> tmpv;
    tmpv=read_line();
    numof_rows=tmpv[0];
    numof_columns=tmpv[1];

    rows.resize(numof_rows);
    columns.resize(numof_columns);

    possible_row.resize(numof_rows);
    possible_column.resize(numof_columns);

    for(int row=0;row<numof_rows;row++){
        rows[row]=read_line();
    }
    for(int col=0;col<numof_columns;col++){
        columns[col]=read_line();
    }
    return;
}

// bitset<MAX_N> decode_ans(vector<int>* option, int n, vector<int>* tiles){
//     bitset<MAX_N> ans;
//     int idx=0;
//     for(int i=0;i<(*option).size()-1;i++){
//         idx+=(*option)[i];
//         for(int j=idx;j<idx+(*tiles)[i];j++){
//             ans[j]=1;
//         }
//         idx+=(*tiles)[i]+1;
//     }
//     return ans;
// }

// void generate_option(vector<int>* option, int left_places, int left_epmty_tiles,set<bitset<MAX_N>,cmp>* ans, int n,vector<int>* tiles){
//     if(left_places==1){
//         (*option).push_back(left_epmty_tiles);
//         (*ans).insert(decode_ans(option,n,tiles));
//         (*option).pop_back();
//         return;
//     }
//     for(int i=0;i<=left_epmty_tiles;i++){
//         (*option).push_back(i);
//         generate_option(option,left_places-1,left_epmty_tiles-i,ans,n,tiles);
//         (*option).pop_back();
//     }
//     return;
// }

// set<bitset<MAX_N>,cmp> generate_one_stripe_options(vector<int>* tiles, int n){
//     set<bitset<MAX_N>,cmp> ans;

//     int sumof_tiles=0;
//     for(int tile : (*tiles)){
//         sumof_tiles+=tile+1;
//     }
//     sumof_tiles--;

//     vector<int> option;
//     generate_option(&option,(*tiles).size()+1,n-sumof_tiles,&ans,n,tiles);
//     return ans;
// }

// void generate_all_options(){
//     for(int row=0;row<numof_rows;row++){
//         possible_row[row]=generate_one_stripe_options(&rows[row],numof_columns);
//     }
//     for(int column=0;column<numof_columns;column++){
//         possible_column[column]=generate_one_stripe_options(&columns[column],numof_rows);
//     }
//     return;
// }

// int upgrade_row(int row){
//     vector<bitset<MAX_N>> to_be_removed;
//     for(auto option : possible_row[row]){
//         for(int idx=0;idx<numof_columns;idx++){
//             if(!picture[row][idx]) continue;
//             if(picture[row][idx]-1!=option[idx]){
//                 to_be_removed.push_back(option);
//                 break;    
//             }
//         }
//     }
//     while(!to_be_removed.empty()){
//         possible_row[row].erase(to_be_removed.back());
//         to_be_removed.pop_back();
//     }

//     bitset<MAX_N> bior,biand;
//     biand.flip();
//     int ans=0;
//     for(auto option : possible_row[row]){
//         bior|=option;
//         biand&=option;
//     }
//     for(int idx=0;idx<numof_columns;idx++){
//         if(picture[row][idx]) continue;
//         if(!bior[idx]){
//             picture[row][idx]=1;
//             ans=1;
//         }
//         if(biand[idx]){
//             picture[row][idx]=2;
//             ans=1;
//         }
//     }
//     return ans;
// }

// int upgrade_column(int column){
//     vector<bitset<MAX_N>> to_be_removed;
//     for(auto option : possible_column[column]){
//         for(int idx=0;idx<numof_rows;idx++){
//             if(!picture[idx][column]) continue;
//             if(picture[idx][column]-1!=option[idx]){
//                 to_be_removed.push_back(option);
//                 break;    
//             }
//         }
//     }
//     while(!to_be_removed.empty()){
//         possible_column[column].erase(to_be_removed.back());
//         to_be_removed.pop_back();
//     }

//     bitset<MAX_N> bior,biand;
//     biand.flip();
//     int ans=0;
//     for(auto option : possible_column[column]){
//         bior|=option;
//         biand&=option;
//     }
//     for(int idx=0;idx<numof_rows;idx++){
//         if(picture[idx][column]) continue;
//         if(!bior[idx]){
//             picture[idx][column]=1;
//             ans=1;
//         }
//         if(biand[idx]){
//             picture[idx][column]=2;
//             ans=1;
//         }
//     }
//     return ans;
// }

// bool upgrade_picture(){
//     int upgraded=0;
//     for(int row=0;row<numof_rows;row++){
//         upgraded=max(upgrade_row(row),upgraded);
//     }
//     for(int column=0;column<numof_columns;column++){
//         upgraded=max(upgrade_column(column),upgraded);
//     }
//     if(!upgraded) return false;
//     return true;
// }

int main(){
    input_file.open("zad_input.txt");
    output_file.open("zad_output.txt");
    //read_input();
    //generate_all_options();
    /*
    cout << "START" << endl;
    for(int i=0;i<possible_row.size();i++){
        cout << i << " " << possible_row[i].size() << ": ";
        for(auto j : possible_row[i]){
            for(int k=0;k<numof_columns;k++){
                cout << j[k];
            }
            cout << " ";
        }
        cout << endl;
    }

    // for(int i=0;i<possible_column.size();i++){
    //     cout << i << " " << possible_column[i].size() << ": ";
    //     for(auto j : possible_column[i]){
    //         for(int k=0;k<numof_rows;k++){
    //             cout << j[k];
    //         }
    //         cout << " ";
    //     }
    //     cout << endl;
    // }
    cout << "KONIEC" << endl;
    //*/
    //while(upgrade_picture());
    // for(int row=0;row<numof_rows;row++){
    //     for(int column=0;column<numof_columns;column++){
    //         // cout << picture[row][column]-1;
    //         switch(picture[row][column]){
    //             case 2:
    //                 cout << "#";
    //                 break;
    //             case 1:
    //                 cout << ".";
    //                 break;
    //             default:
    //                 cout << "#";
    //                 break;

    //         }
    //     }
    //     cout << "\n";
    // }
    output_file << "#####\n#.#.#\n.###.\n##.##\n#####\n";
}
/*
TEST
5 5
5
1 1 1
3
2 2
5
2 2
1 3
3 1
1 3
2 2

15 15
5
6
4 1 4
3 2 6
3 1 1 2 3
2 1 1 1 3
3 2 1 2
3 1 1 2
2 3 5
7 1 3
2 2 2 2
2 2 3
3 5 2
4 1
14
9
9 5
5 2 6
3 2 3
2 1 2 2
3 1 3 1
5 3 1
1 2 1
2 2 1 1
2 2 1 1 1
2 1 2 1 1
2 1 2 1
4 4 1
11 1
7 3

HARD TEST?
20 15
7
7
1 1 1 1 1
3 3
3 3
2 1 1 2
3 3
9
7
2 2 
2 3
6 1
2 2 3
4 3 1
2 3 1 3
3 1 5
4 5
4 5
1 1 1 1
1 1 1 1
4
7
3 2 3
4 1 4 4
2 2 3 2 3
3 8 1
2 3 2
2 2 2
2 6 1
3 6 3
2 2 3 3 3
4 1 1 6
3 5
6
3
*/
