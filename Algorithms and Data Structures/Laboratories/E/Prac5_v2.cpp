#include<iostream>
#define BOOST cin.tie(0);ios_base::sync_with_stdio(0)
#define st first
#define nd second

using namespace std;

typedef long long ll;

int RAI(){
    return rand();
}

struct Node{
    ll key;
    int prt;
    Node *L = nullptr;
    Node *R = nullptr;
    Node(ll tkey){
        key = tkey;
        prt = RAI();
    }
    Node(ll tkey, ll tprt){
        key = tkey;
        prt = tprt;
    }
};

struct Treap{
    Node *root = nullptr;

    Treap(Node new_node){

    }

    void merge(Treap *T2){

    }

    void split(ll split_key, Treap *T2){
        
    }
};

int main(){
    BOOST;

    int n; cin >> n;
    Treap T;

}