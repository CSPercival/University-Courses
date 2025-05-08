#include<bits/stdc++.h>

using namespace std;

int main(){
    string s;
    int d,ctr1;
    while(true){
        cin >> s >> d;
        int n=s.size();
        if(n<d){
            cout << "Error: Size of block is too big\n";
            continue;
        }

        ctr1=0;
        for(int i : s){
            if(i=='1'){
                ctr1++;
            }
        }
        int ans=0;
        int tmp_ans=0;
        for(int i=0;i<d;i++){
            if(s[i]=='1') tmp_ans++;
        }
        ans=tmp_ans;
        for(int i=d;i<n;i++){
            if(s[i]=='1') tmp_ans++;
            if(s[i-d]=='1') tmp_ans--;
            ans=max(ans,tmp_ans);
        }
        ans=d+ctr1-ans*2;
        cout << ans << "\n";
    }

    return 0;
}
/*
0010001000 5
0010001000 4
0010001000 3
0010001000 2
0010001000 1
0010001000 0
*/