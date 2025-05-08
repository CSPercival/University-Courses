#ifndef MAIN_CPP
#define MAIN_CPP

#include "Libraries_and_Macros.h"
#include "Functions.h"
#include "Game.cpp"
#include "Board.cpp"
#include "Bot_template.cpp"
#include "Bot_st00pid.cpp"
#include "Bot_superior.cpp"
#include "Bot_GigaChad.cpp"

int main(){
    srand ( unsigned ( time(0) ) );

    //cerr << "IGRZYSKA CZAS ZACZAC\n";

    Bot *st00pido = new St00pid({});
    Bot *st00pido2 = new St00pid({});
    Bot *superito = new Superior({});
    Bot *gigachad1 = new Gigachad({2,2500});
    Bot *gigachad2 = new Gigachad({2,100});

    int numof_games = 10;
    int end_result;
    int sumof_results = 0;
    int wins1 = 0, wins2 = 0;
    Game game1(superito,gigachad1);
    Game game2(gigachad1,superito);
    
    // Game game1(st00pido,gigachad1);
    // Game game2(gigachad1,st00pido);

    // Game game1(st00pido,superito);
    // Game game2(superito,st00pido);
    
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
            if(end_result >= 20){
                cout << "CHECK GAME " << itg << endl;
                //return 0;
            }
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