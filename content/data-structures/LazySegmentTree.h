/**
 * Author: Simon Lindholm
 * Date: 2016-10-08
 * License: CC0
 * Source: me
 * Description: Segment tree with ability to add or set values of large intervals, and compute max of intervals.
 * Can be changed to other things.
 * Use with a bump allocator for better performance, and SmallPtr or implicit indices to save memory.
 * Time: O(\log N).
 * Usage: Node* tr = new Node(v, 0, sz(v));
 * Status: fuzz-tested a bit
 *
 */
#pragma once

#include "../various/BumpAllocator.h"

struct Node {
    typedef int T;
    static const T LOW = 0;
    #define UPDATE ((hi - lo) * x)
    // #define UPDATE (x)
    T f(T a, T b) { return (a + b); }

    Node *l = 0, *r = 0;
    int lo, hi;
    T mset = -1, madd = 0, val = LOW;
    Node(int lo, int hi) : lo(lo), hi(hi) {}
    ~Node() { delete l; delete r; }
    Node(vector<T>& v, int lo, int hi) : lo(lo), hi(hi) {
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
    void set(int L, int R, T x) {
        if (R <= lo || hi <= L) return;
        if (L <= lo && hi <= R) {
            mset = x, madd = 0;
            val = UPDATE;
        } else {
            push(), l->set(L, R, x), r->set(L, R, x);
            val = f(l->val, r->val);
        }
    }
    void add(int L, int R, T x) {
        if (R <= lo || hi <= L) return;
        if (L <= lo && hi <= R) {
            if (mset != -1) mset += x;
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
        if (mset != -1) {
            l->set(lo,hi,mset), r->set(lo,hi,mset), mset = -1;
        } else if (madd) {
            l->add(lo,hi,madd), r->add(lo,hi,madd), madd = 0;
        }
    }
};
