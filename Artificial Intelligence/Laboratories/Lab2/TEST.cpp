#include<bits/stdc++.h>

int main(){
    int idx=3;
    int t=5;
    for(int i=idx;i<(idx+=t);i++){
        std::cout << i << " ";
    }
    std::cout << "\n" << idx << "\n";
}