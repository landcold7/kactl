#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < int(b); ++i)
#define trav(a, v) for(auto& a : v)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

#include "../content/various/BumpAllocator.h"
struct Node {
    typedef int T;
    static const T LOW = numeric_limits<T>::min();
    #define UPDATE (x)
    T f(T a, T b) { return max(a, b); }

    Node *l = 0, *r = 0;
    int lo, hi;
    const T FLAG = numeric_limits<T>::min();
    T mset = FLAG, madd = 0, val = LOW;
    Node(int lo, int hi) : lo(lo), hi(hi) {}
    Node(vi& v, int lo, int hi) : lo(lo), hi(hi) {
        if (lo + 1 < hi) {
            int mid = lo + (hi - lo) / 2;
            l = new Node(v, lo, mid);
            r = new Node(v, mid, hi);
            val = f(l->val, r->val);
        } else {
            val = v[lo];
        }
    }
    T query(int L, int R) {
        if (R <= lo || hi <= L) return LOW;
        if (L <= lo && hi <= R) return val;
        push();
        return f(l->query(L, R), r->query(L, R));
    }
    void set(int L, int R, int x) {
        if (R <= lo || hi <= L) return;
        if (L <= lo && hi <= R) {
            mset = x, madd = 0;
            val = UPDATE;
        } else {
            push(), l->set(L, R, x), r->set(L, R, x);
            val = f(l->val, r->val);
        }
    }
    void add(int L, int R, int x) {
        if (R <= lo || hi <= L) return;
        if (L <= lo && hi <= R) {
            if (mset != FLAG) mset += x;
            else madd += x;
            val += UPDATE;
        } else {
            push(), l->add(L, R, x), r->add(L, R, x);
            val = f(l->val, r->val);
        }
    }
    void push() {
        if (!l) {
            int mid = lo + (hi - lo) / 2;
            l = new Node(lo, mid);
            r = new Node(mid, hi);
        }
        if (mset != FLAG) {
            l->set(lo,hi,mset), r->set(lo,hi,mset), mset = FLAG;
        } else if (madd) {
            l->add(lo,hi,madd), r->add(lo,hi,madd), madd = 0;
        }
    }
};

static unsigned R;
int ra() {
    R *= 791231;
    R += 1231;
    return (int)(R >> 1);
}

volatile int res;
int main() {
    int N = 10;
    vi v(N);
    iota(all(v), 0);
    random_shuffle(all(v), [](int x) { return ra() % x; });
    Node* tr = new Node(v,0,N);
    rep(i,0,N) rep(j,0,N) if (i <= j) {
        int ma = numeric_limits<int>::min();
        rep(k,i,j) ma = max(ma, v[k]);
        assert(ma == tr->query(i,j));
    }
    rep(it,0,1000000) {
        int i = ra() % (N+1), j = ra() % (N+1);
        if (i > j) swap(i, j);
        int x = (ra() % 10) - 5;

        int r = ra() % 100;
        if (r < 30) {
            ::res = tr->query(i, j);
            int ma = numeric_limits<int>::min();
            rep(k,i,j) ma = max(ma, v[k]);
            assert(ma == ::res);
        }
        else if (r < 70) {
            tr->add(i, j, x);
            rep(k,i,j) v[k] += x;
        }
        else {
            tr->set(i, j, x);
            rep(k,i,j) v[k] = x;
        }
    }
    exit(0);
}
