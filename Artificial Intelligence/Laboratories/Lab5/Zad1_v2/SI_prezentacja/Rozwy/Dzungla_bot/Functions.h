#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Libraries_and_Macros.h"

pair<int,int> operator+(pair<int,int> const&, pair<int,int> const&);

pair<int,int> operator-(pair<int,int> const&, pair<int,int> const&);

bool operator==(pair<int,int> const&, pair<int,int> const&);

int random_int(int, int);

char obstacle(pair<int,int>);

#endif