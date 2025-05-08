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

const int MAX_N = 9;
const int MAX_M = 7;
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
const pair<int,int> emptyP = {0,0};

const vector <pair <int,int>> dxy{{-1,0},{0,1},{1,0},{0,-1}};

const vector< vector<char>> obst   {{'.','.','#','*','#','.','.'},
                                    {'.','.','.','#','.','.','.'},
                                    {'.','.','.','.','.','.','.'},
                                    {'.','~','~','.','~','~','.'},
                                    {'.','~','~','.','~','~','.'},
                                    {'.','~','~','.','~','~','.'},
                                    {'.','.','.','.','.','.','.'},
                                    {'.','.','.','#','.','.','.'},
                                    {'.','.','#','*','#','.','.'}};
const char cave = '*';
const char trap = '#';
const char river = '~';

const vector< vector <pair<int,int>>> start_pos{{{7,2},{0,0},{0,0},{0,2},{0,0},{0,0},{6,2}},
                                                {{0,0},{3,2},{0,0},{0,0},{0,0},{2,2},{0,0}},
                                                {{1,2},{0,0},{5,2},{0,0},{4,2},{0,0},{8,2}},
                                                {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
                                                {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
                                                {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
                                                {{8,1},{0,0},{4,1},{0,0},{5,1},{0,0},{1,1}},
                                                {{0,0},{2,1},{0,0},{0,0},{0,0},{3,1},{0,0}},
                                                {{6,1},{0,0},{0,0},{0,1},{0,0},{0,0},{7,1}}};
const vector< vector <char>> ntp   {{'.','r','c','d','w','j','t','l','e'},
                                    {'.','R','C','D','W','J','T','L','E'}};

#endif
