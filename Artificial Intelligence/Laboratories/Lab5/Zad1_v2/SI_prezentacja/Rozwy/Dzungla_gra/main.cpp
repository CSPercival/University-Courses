#ifndef MAIN_CPP
#define MAIN_CPP

#include "Libraries_and_Macros.h"
#include "Functions.h"
#include "Game.cpp"
#include "Board.cpp"
#include "Bot_template.cpp"
#include "Bot_st00pid.cpp"
#include "Bot_superior.cpp"
#include "Bot_0iq.cpp"

int main(){
    srand ( unsigned ( time(0) ) );

    //cerr << "IGRZYSKA CZAS ZACZAC\n";

    Bot *st00pido = new St00pid();
    Bot *st00pido2 = new OIQ();
    Bot *superito = new Superior();

    int numof_games = 1000;
    int end_result;
    int sumof_results = 0;
    int wins1 = 0, wins2 = 0;
    Game game1(st00pido,superito);
    Game game2(superito,st00pido);
    // Game game1(st00pido,st00pido2);
    // Game game2(st00pido2,st00pido);
    
    auto start = high_resolution_clock::now();
    
    for(int itg = 1; itg <= numof_games; itg++){
        cerr << "GRA: " << itg << endl;
        if(itg&1){
            game1.setup();
            end_result = game1.play();
        } else {
            game2.setup();
            end_result = game2.play() * -1;
        }
        if(end_result > 0){
            // if(end_result >= 25){
            //     cout << "CHECK GAME " << itg << endl;
            //     return 0;
            // }
            wins1++;
        }
        if(end_result < 0){
            wins2++;
        }
        sumof_results += end_result;
        cout << fixed;
        cout << "GAME " << itg  << "  SCORE: " << wins1 << " - " << wins2 << "   " << setprecision(2) << (double)wins2/(double)itg*100 << "% " << "     RESULT: " << end_result  << "   SUM: " << sumof_results << "\n";
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by experiment: "
         << setprecision(2) << duration.count()/(double)1000000 << " seconds" << endl;
    return 0;
}

#endif