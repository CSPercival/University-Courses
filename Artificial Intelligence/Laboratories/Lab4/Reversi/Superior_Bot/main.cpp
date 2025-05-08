#ifndef MAIN_CPP
#define MAIN_CPP

#include "Libraries_and_Macros.h"
#include "Functions.h"
#include "Board.cpp"
#include "Bot_template.cpp"
#include "Bot_superior.cpp"

int main(){
    srand ( unsigned ( time(0) ) );
    Bot *superito = new Superior();

    pair<string,pair<int,int>> ans;
    ans = superito->perform(ONEMORE_MESS,emptyP);
    cout << RDY_MESS << "\n";
    cout << flush;

    string message;
    double trash1,trash2; 
    int x, y;   
    while(true){
        cin >> message;
        if(message == UGO_MESS){
            cin >> trash1 >> trash2;
            ans = superito->perform(UGO_MESS,emptyP);
            cout << ans.st << " " << ans.nd.st << " " << ans.nd.nd << "\n";
            cout << flush;
        }
        if(message == HEDID_MESS){
            cin >> trash1 >> trash2;
            cin >> x >> y;
            ans = superito->perform(UGO_MESS,{x,y});
            cout << ans.st << " " << ans.nd.st << " " << ans.nd.nd << "\n";
            cout << flush;
        }
        if(message == ONEMORE_MESS){
            ans = superito->perform(ONEMORE_MESS,emptyP);
            cout << ans.st << "\n";
            cout << flush;
        }
        if(message == BYE_MESS){
            break;
        }
        cout << ERROR_MESS << "\n";
        cout << flush;
    }
    return 0;
}

#endif