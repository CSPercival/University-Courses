#ifndef LIBRARIES_AND_MACROS_H
#define LIBRARIES_AND_MACROS_H

#include <bits/stdc++.h>
#include <ctime>
#include <chrono>
//#include <assert.h>
//#include <thread>

//#define wait(x) this_thread::sleep_for((x)*1ms);
#define st first
#define nd second

using namespace std;
using namespace chrono;

const int MAX_N = 8;
const int MAX_DEPTH = 1;

const string UGO_MESS =     "UGO";
const string HEDID_MESS =   "HEDID";
const string ONEMORE_MESS = "ONEMORE";
const string BYE_MESS =     "BYE";
const string RDY_MESS =     "RDY";
const string IDO_MESS =     "IDO";
const string ERROR_MESS =   "ERROR";
const string RESULT_MESS =   "RESULT";

const vector<int> emptyV;
const pair<int,int> emptyP = {-1,-1};

const vector <pair <int,int>> dxy{{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
const vector <pair <int,int>> corners{{0,0},{MAX_N-1,0},{0,MAX_N-1},{MAX_N-1,MAX_N-1}};
const vector <pair <int,int>> cover_dir{{0,6},{4,6},{0,2},{2,4}};
const int stable_disc_weight = 30;
const vector< vector<int>> weights  {{20,-4,3,3,3,3,-4,20},
                                     {-4,-6,-2,-2,-2,-2,-6,-4},
                                     {3,-2,1,1,1,1,-2,3},
                                     {3,-2,1,1,1,1,-2,3},
                                     {3,-2,1,1,1,1,-2,3},
                                     {3,-2,1,1,1,1,-2,3},
                                     {-4,-6,-2,-2,-2,-2,-6,-4},
                                     {20,-4,3,3,3,3,-4,20}};

#endif
