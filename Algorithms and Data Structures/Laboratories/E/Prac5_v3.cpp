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
    if(T == NULL){
        return;
    }
    if(T->key <= skey){
        L = T;
        R = NULL;
        split(T->R,skey,T->R,R);
    } else {
        R = T;
        L = NULL;
        split(T->L,skey,L,T->L);
    }
}

void merge(Node* &T, Node* L, Node* R){
    if(L == NULL){
        T = R;
        return;
    }
    if(R == NULL){
        T = L;
        return;
    }
    if(L->prt < R->prt){
        T = R;
        merge(T->L,L,T->L);
    } else {
        T = L;
        merge(T->R,T->R,R);
    }
}

void insert(Node* &T, Node *NN){
    if(T == NULL){
        T = NN;
        return;
    }
    if(T->prt < NN->prt){
        split(T,NN->key,NN->L,NN->R);
        T = NN;
    } else {
        if(T->key < NN->key){
            insert(T->R,NN);
        } else {
            if(T->key > NN->key){
                insert(T->L,NN);
            }
        }
    }
}

bool erase(Node* &T, ll key){
    if(T == NULL){
        return false;
    }
    if(T->key < key){
        return erase(T->R,key);
    }
    if(T->key > key){
        return erase(T->L,key);; 
    }
    merge(T,T->L,T->R);
    return true;
}

ll upper(Node *T, ll ukey){
    if(T == NULL) return upper_fail;    
    if(T->key == ukey){
        return ukey;
    }
    if(T->key < ukey){
        return upper(T->R,ukey);
    }
    return min(T->key,upper(T->L,ukey));
}

ll lower(Node *T, ll lkey){
    if(T == NULL) return lower_fail;    
    if(T->key == lkey){
        return lkey;
    }
    if(T->key < lkey){
        return max(T->key, lower(T->R,lkey));
    }
    return lower(T->L,lkey);
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
        }
        if(op == 'D'){
            if(!erase(Treap,x)){
                cout << "BRAK\n";
            } else {
                cout << "OK\n";
            }
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
}
