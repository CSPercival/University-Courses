#include<iostream>
#include <climits>
#define BOOST cin.tie(0);ios_base::sync_with_stdio(0)
#define st first
#define nd second

using namespace std;

typedef long long ll;

ll upper_fail = LLONG_MAX;
ll lower_fail = LLONG_MIN;

int RAI(){
    return rand();
}

struct Node{
    ll key;
    int prt;
    Node *L = NULL;
    Node *R = NULL;
    Node(){}
    Node(ll tkey){
        key = tkey;
        prt = RAI();
    }
    Node(ll tkey, int tprt){
        key = tkey;
        prt = tprt;
    }
};

void split(Node *T, ll skey, Node* &L, Node* &R){
    //c cout << "split1" << endl;
    if(T == NULL){
        //c cout << "split2" << endl;
        return;
    }
    if(T->key <= skey){
        //c cout << "split3" << endl;
        L = T;
        R = NULL;
        // Node *nn = NULL;
        // Node *nt = T->R;
        // L->R = nn;
        split(T->R,skey,T->R,R);
    } else {
        //c cout << "split4" << endl;
        R = T;
        L = NULL;
        // Node *nn = NULL;
        // Node *nt = T->L;
        // R->L = nn;
        split(T->L,skey,L,T->L);
    }
}

void merge(Node* &T, Node* L, Node* R){
    //c cout << "mer1" << endl;
    if(L == NULL){
        //c cout << "mer2" << endl;
        T = R;
        return;
    }
    if(R == NULL){
        //c cout << "mer3" << endl;
        T = L;
        return;
    }
    if(L->prt < R->prt){
        //c cout << "mer4" << endl;
        T = R;
        merge(T->L,L,T->L);
    } else {
        //c cout << "mer5" << endl;
        T = L;
        merge(T->R,T->R,R);
    }
}

void insert(Node* &T, Node *NN){
    //c cout << "ins1" << endl;
    if(T == NULL){
        //c cout << "ins2" << endl;
        T = NN;
        return;
    }
    // if(T->key == NN->key) return;
    if(T->prt < NN->prt){
        //c cout << "ins3" << endl;
        split(T,NN->key,NN->L,NN->R);
        T = NN;
    } else {
        //c cout << "ins4" << endl;
        //c cout << T->key << " - " << NN->key << endl;
        if(T->key < NN->key){
            //c cout << "ins5" << endl;
            insert(T->R,NN);
        } else {
            if(T->key > NN->key){
                //c cout << "ins6" << endl;
                insert(T->L,NN);
            }
        }
    }
}

bool erase(Node* &T, ll key){
    //c cout << "era1" << endl;
    if(T == NULL){
        //c cout << "era2" << endl;
        return false;
    }
    //c cout << T->key << endl;
    if(T->key < key){
        //c cout << "era3" << endl;
        return erase(T->R,key);
    }
    if(T->key > key){
        //c cout << "era4" << endl;
        return erase(T->L,key);; 
    }
    //c cout << "era5" << endl;
    merge(T,T->L,T->R);
    return true;
    // if(T->L == NULL){
    //     T = T->R;
    // } else {
    //     T = T->L;
    // }
}

ll upper(Node *T, ll ukey){
    if(T == NULL) return upper_fail;    
    if(T->key == ukey){
        return ukey;
    }
    if(T->key < ukey){
        return upper(T->R,ukey);
    }
    if(T->key > ukey){
        return min(T->key,upper(T->L,ukey));
    }
}

ll lower(Node *T, ll lkey){
    if(T == NULL) return lower_fail;    
    if(T->key == lkey){
        return lkey;
    }
    if(T->key < lkey){
        return max(T->key, lower(T->R,lkey));
    }
    if(T->key > lkey){
        return lower(T->L,lkey);
    }
}

void print(Node *T){
    if(T == NULL){
        return;
    }
    printf("( %lld, %d )\n",T->key,T->prt);
    printf("Sons: ");
    if(T->L != NULL){
        printf("L: %lld  ",T->L->key);
    }
    if(T->R != NULL){
        printf("R: %lld  ",T->R->key);
    }
    printf("\n");
    // cout << T->key << " - " << T->prt << endl;
    print(T->L);
    print(T->R);
}

int main(){
    BOOST;
    int n; cin >> n;
    ll sd = n;
    sd *= RAI();
    srand(sd % INT_MAX);

    Node *Treap = NULL;
    print(Treap);
    char op;
    ll x;
    ll ans;
    for(int i = 0; i < n; i++){
        cin >> op >> x;
        if(op == 'I'){
            if(upper(Treap,x) != x){
                Node* nn = new Node(x);
                insert(Treap,nn);
            }
            // print(Treap);
        }
        if(op == 'D'){
            if(!erase(Treap,x)){
                cout << "BRAK\n";
            } else {
                cout << "OK\n";
            }
            // print(Treap);
        }
        if(op == 'U'){
            ans = upper(Treap,x);
            if(ans == upper_fail){
                cout << "BRAK\n";
            } else {
                cout << ans << "\n";
            }
        }
        if(op == 'L'){
            ans = lower(Treap,x);
            if(ans == lower_fail){
                cout << "BRAK\n";
            } else {
                cout << ans << "\n";
            }
        }
    }
    // cout << "-----------------------\n";
    // print(Treap);
}
//1804289383
//846930886