
#include "Functions.h"

pair<int,int> operator+(pair<int,int> const& p1, pair<int,int> const& p2){
    return {p1.st+p2.st, p1.nd+p2.nd};
}

pair<int,int> operator-(pair<int,int> const& p1, pair<int,int> const& p2){
    return {p1.st-p2.st, p1.nd-p2.nd};
}

int random_int(int l, int r){
    return (rand()%(r-l))+l;
}
