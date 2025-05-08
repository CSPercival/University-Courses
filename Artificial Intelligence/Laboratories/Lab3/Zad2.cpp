#include<bits/stdc++.h>
#include<sstream>

using namespace std;

const int MAX_N = 30;
const string USER_MARK = "user";
const string VALIDATOR_MARK = "validator";
ifstream input_file;
ofstream output_file;

int numof_rows;
int numof_columns;
vector<vector<int>> rows;
vector<vector<int>> columns;

struct cmp {
    bool operator() (bitset<MAX_N> a, bitset<MAX_N> b) const { 
        int idx=0;
        while(idx<MAX_N-1 && a[idx]==b[idx]){
            idx++;
        }
        return a[idx]<b[idx];
    }
};

vector<int> read_line(string type){
    vector<int> ans;
    string input;
    if(type == USER_MARK) getline(cin,input);
    else  getline(input_file,input);

    istringstream ss(input);
    int num;
    while(ss >> num){
        ans.push_back(num);
    }
    return ans;
}

void read_input(string type){
    vector<int> tmpv;
    tmpv = read_line(type);
    numof_rows = tmpv[0];
    numof_columns = tmpv[1];

    rows.resize(numof_rows);
    columns.resize(numof_columns);

    for(int row = 0; row < numof_rows; row++){
        rows[row] = read_line(type);
    }
    for(int col = 0; col < numof_columns; col++){
        columns[col] = read_line(type);
    }
    return;
}

void print_output(vector<vector<int>> picture, string type){
    char tmpc;
    for(auto row : picture){
        for(auto field : row){
            switch(field){
                case 2:
                    tmpc = '#';
                    break;
                case 1:
                    tmpc = '.';
                    break;
                default:
                    tmpc = '?';
                    break;
            }
            if(type == USER_MARK) cout << tmpc;
            else  output_file << tmpc;
        }
        if(type == USER_MARK) cout << "\n";
        else  output_file << "\n";
    }
}

class SolvePicture{
    public:
        vector<set<bitset<MAX_N>,cmp>> possible_row;
        vector<set<bitset<MAX_N>,cmp>> possible_column;
        vector<vector<int>> picture;
        int numof_unknown_fields;
        int numof_unknown_fields_after_upgrade;

        SolvePicture(vector<set<bitset<MAX_N>,cmp>> possible_row, vector<set<bitset<MAX_N>,cmp>> possible_column, vector<vector<int>> picture){
            this->possible_row = possible_row;
            this->possible_column = possible_column;
            this->picture = picture;

            numof_unknown_fields = 0;
            for(auto row : picture){
                for(auto field : row){
                    if(field == 0) numof_unknown_fields++;
                }
            }
            numof_unknown_fields_after_upgrade = numof_unknown_fields;
            // cout << "CONSTRUCTOR DONE" << endl;
        }

        void upgrade_row(int row){
            vector<bitset<MAX_N>> to_be_removed;
            for(auto option : possible_row[row]){
                for(int idx=0;idx<numof_columns;idx++){
                    if(!picture[row][idx]) continue;
                    if(picture[row][idx]-1!=option[idx]){
                        to_be_removed.push_back(option);
                        break;    
                    }
                }
            }
            while(!to_be_removed.empty()){
                possible_row[row].erase(to_be_removed.back());
                to_be_removed.pop_back();
            }

            bitset<MAX_N> bior,biand;
            biand.flip();
            for(auto option : possible_row[row]){
                bior|=option;
                biand&=option;
            }
            for(int idx=0;idx<numof_columns;idx++){
                if(picture[row][idx]) continue;
                if(!bior[idx]){
                    picture[row][idx]=1;
                    numof_unknown_fields_after_upgrade--;
                }
                if(biand[idx]){
                    picture[row][idx]=2;
                    numof_unknown_fields_after_upgrade--;
                }
            }
        }

        void upgrade_column(int column){
            vector<bitset<MAX_N>> to_be_removed;
            for(auto option : possible_column[column]){
                for(int idx=0;idx<numof_rows;idx++){
                    if(!picture[idx][column]) continue;
                    if(picture[idx][column]-1!=option[idx]){
                        to_be_removed.push_back(option);
                        break;    
                    }
                }
            }
            while(!to_be_removed.empty()){
                possible_column[column].erase(to_be_removed.back());
                to_be_removed.pop_back();
            }

            bitset<MAX_N> bior,biand;
            biand.flip();
            for(auto option : possible_column[column]){
                bior|=option;
                biand&=option;
            }
            for(int idx=0;idx<numof_rows;idx++){
                if(picture[idx][column]) continue;
                if(!bior[idx]){
                    picture[idx][column] = 1;
                    numof_unknown_fields_after_upgrade--;
                }
                if(biand[idx]){
                    picture[idx][column] = 2;
                    numof_unknown_fields_after_upgrade--;
                }
            }
        }

        void upgrade_picture(){
            for(int row = 0; row < numof_rows; row++)
                upgrade_row(row);
            for(int column = 0; column < numof_columns; column++)
                upgrade_column(column);
        }

        bool error_in_picture(){
            for(int row = 0; row < numof_rows; row++){
                upgrade_row(row);
                if(possible_row[row].size() == 0) return true;
            }
            for(int column = 0; column < numof_columns; column++){
                upgrade_column(column);
                if(possible_column[column].size() == 0) return true;
            }
            return false;
        }

        pair<bool, vector<vector<int>>> solve(){
            // cout << "START SOLVING" << endl;
            do{
                // print_user_output(picture);
                // cout << endl;
                numof_unknown_fields = numof_unknown_fields_after_upgrade;
                upgrade_picture();
            } while(numof_unknown_fields_after_upgrade != numof_unknown_fields);
            
            // print_output(picture,USER_MARK);
            // cout << endl;

            if(error_in_picture()){
                // cout << "ERROR FOUND" << endl;
                return {false, picture};
            }
            if(numof_unknown_fields == 0){
                // cout << "SOLUTION FOUND" << endl;
                return {true, picture};
            }
            
            // cout << "WE NEED TO GO DEEPER "  << numof_unknown_fields << endl;

            int tmp_row = -1,tmp_column = -1;

            for(int row = 0; row < numof_rows; row++){
                for(int column = 0; column < numof_columns; column++){
                    if(!picture[row][column]){
                        tmp_row = row;
                        tmp_column = column;
                        column = numof_columns;
                        row = numof_rows;
                    }
                }
            }
            // cout << "FOUND xy: " << tmp_row << " " << tmp_column << endl;
            pair<bool, vector<vector<int>>> ans;

            picture[tmp_row][tmp_column] = 1;
            SolvePicture guess1(possible_row, possible_column, picture);
            ans = guess1.solve();
            if(!ans.first){
                picture[tmp_row][tmp_column] = 2;
                SolvePicture guess2(possible_row, possible_column, picture);
                ans = guess2.solve();
            }
            return ans;
        }
};

bitset<MAX_N> decode_ans(vector<int>* option, int n, vector<int>* tiles){
    bitset<MAX_N> ans;
    int idx=0;
    for(int i=0;i<(*option).size()-1;i++){
        idx+=(*option)[i];
        for(int j=idx;j<idx+(*tiles)[i];j++){
            ans[j]=1;
        }
        idx+=(*tiles)[i]+1;
    }
    return ans;
}

void generate_option(vector<int>* option, int left_places, int left_epmty_tiles,set<bitset<MAX_N>,cmp>* ans, int n,vector<int>* tiles){
    if(left_places==1){
        (*option).push_back(left_epmty_tiles);
        (*ans).insert(decode_ans(option,n,tiles));
        (*option).pop_back();
        return;
    }
    for(int i=0;i<=left_epmty_tiles;i++){
        (*option).push_back(i);
        generate_option(option,left_places-1,left_epmty_tiles-i,ans,n,tiles);
        (*option).pop_back();
    }
    return;
}

set<bitset<MAX_N>,cmp> generate_one_stripe_options(vector<int>* tiles, int n){
    set<bitset<MAX_N>,cmp> ans;

    int sumof_tiles=0;
    for(int tile : (*tiles)){
        sumof_tiles+=tile+1;
    }
    sumof_tiles--;

    vector<int> option;
    generate_option(&option,(*tiles).size()+1,n-sumof_tiles,&ans,n,tiles);
    return ans;
}

void generate_all_options(vector<set<bitset<MAX_N>,cmp>>* possible_row, vector<set<bitset<MAX_N>,cmp>>* possible_column){
    for(int row = 0; row < numof_rows; row++){
        (*possible_row)[row] = generate_one_stripe_options(&rows[row],numof_columns);
        //cout << possible_row[row].size() << " ";
    }
    //cout << "\n";
    for(int column = 0; column < numof_columns; column++){
        (*possible_column)[column] = generate_one_stripe_options(&columns[column],numof_rows);
        //cout << possible_column[column].size() << " ";
    }
    //cout << "\n";
    return;
}

int main(){
    // cout << "TEST START" << endl;
    input_file.open("zad_input.txt");
    output_file.open("zad_output.txt");
    // read_input(USER_MARK);
    read_input(VALIDATOR_MARK);
    // cout << "WCZYTANO INPUT" << endl;

    vector<set<bitset<MAX_N>,cmp>> all_possible_row(numof_rows);
    vector<set<bitset<MAX_N>,cmp>> all_possible_column(numof_columns);
    generate_all_options(&all_possible_row, &all_possible_column);
    // cout << "WYGENEROWANO WSZYSTKIE OPCJE" << endl;

    vector<vector<int>> picture(numof_rows, vector<int>(numof_columns, 0));
    SolvePicture test(all_possible_row,all_possible_column,picture);
    pair<bool, vector<vector<int>>> ans;
    ans = test.solve();
    // cout << "ZAKONCZONO PROCES" << endl;
    if(!ans.first){
        cout << "NO I DUPA\n";
    }

    // print_output(ans.second,USER_MARK);
    print_output(ans.second,VALIDATOR_MARK);
    input_file.close();
    output_file.close();
    // cout << "TEST KONIEC" << endl;
}
/*
TEST 0
5 5
5
1 1
3
2 2
5
2 2
1 3
3 1
1 3
2 2


TEST
15 15
5
6
4 1 4
3 2 6
3 1 1 2 3
2 1 1 1 3
3 2 1 2
3 1 1 2
2 3 5
7 1 3
2 2 2 2
2 2 3
3 5 2
4 1
14
9
9 5
5 2 6
3 2 3
2 1 2 2
3 1 3 1
5 3 1
1 2 1
2 2 1 1
2 2 1 1 1
2 1 2 1 1
2 1 2 1
4 4 1
11 1
7 3

HARD TEST?
20 15
7
7
1 1 1 1 1
3 3
3 3
2 1 1 2
3 3
9
7
2 2 
2 3
6 1
2 2 3
4 3 1
2 3 1 3
3 1 5
4 5
4 5
1 1 1 1
1 1 1 1
4
7
3 2 3
4 1 4 4
2 2 3 2 3
3 8 1
2 3 2
2 2 2
2 6 1
3 6 3
2 2 3 3 3
4 1 1 6
3 5
6
3
*/
