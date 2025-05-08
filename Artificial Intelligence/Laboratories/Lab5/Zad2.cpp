#include<bits/stdc++.h>

using namespace std;

int n = 70;
int board[100][100];
int used[25];

bool check(int k, int x, int y){
    if(x + k >= n || y + k >= n) return false;
    for(int i = x; i < x + k; i++){
        for(int j = y; j < y + k; j++){
            if(board[i][j]) return false;
        }
    }
    return true;
}

void add(int k, int x, int y){
    for(int i = x; i < x + k; i++){
        for(int j = y; j < y + k; j++){
            board[i][j] = k;
        }
    }
}

int main(){

    // for(int i = 0; i < n; i++){
    //     for(int j = 0; j < n; j++){
    //         for(int k = 24; k > 0; k--){
    //             if(!used[k] && check(k,i,j)){
    //                 add(k,i,j);
    //                 used[k] = true;
    //                 break;
    //             }
    //         }
    //     }
    // }

    for(int k = 24; k > 0; k--){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){        
                if(!used[k] && check(k,i,j)){
                    add(k,i,j);
                    used[k] = true;
                    break;
                }
            }
        }
    }
    
    int empty_squares = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(board[i][j]){
                board[i][j] += 'A' - 1;
            } else {
                board[i][j] = '.';
                empty_squares++;
            }
        }
    }
    cout << empty_squares << "\n";
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << (char)board[i][j];
        }
        cout << "\n";
    }
}