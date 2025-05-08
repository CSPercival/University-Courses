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

    pair<string,pair<pair<int,int>,pair<int,int>>> ans;
    ans = superito->perform(ONEMORE_MESS,emptyPP);
    cout << RDY_MESS << "\n";
    cout << flush;

    string message;
    double trash1,trash2; 
    int x1,x2, y1,y2;   
    while(true){
        cin >> message;
        if(message == UGO_MESS){
            cin >> trash1 >> trash2;
            ans = superito->perform(UGO_MESS,emptyPP);
            cout << ans.st << " " << ans.nd.st.nd << " " << ans.nd.st.st << " " << ans.nd.nd.nd << " " << ans.nd.nd.st << "\n";
            //cout << ans.st << " " << ans.nd.st.st << " " << ans.nd.st.nd << " " << ans.nd.nd.st << " " << ans.nd.nd.nd << "\n";
            cout << flush;
            continue;
        }
        if(message == HEDID_MESS){
            cin >> trash1 >> trash2;
            cin >> y1 >> x1 >> y2 >> x2;
            //cin >> x1 >> y1 >> x2 >> y2;
            ans = superito->perform(HEDID_MESS,{{x1,y1},{x2,y2}});
            cout << ans.st << " " << ans.nd.st.nd << " " << ans.nd.st.st << " " << ans.nd.nd.nd << " " << ans.nd.nd.st << "\n";
            //cout << ans.st << " " << ans.nd.st.st << " " << ans.nd.st.nd << " " << ans.nd.nd.st << " " << ans.nd.nd.nd << "\n";
            cout << flush;
            continue;
        }
        if(message == ONEMORE_MESS){
            ans = superito->perform(ONEMORE_MESS,emptyPP);
            cout << ans.st << "\n";
            cout << flush;
            continue;
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