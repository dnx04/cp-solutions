#line 1 "Subarray_Sum_Queries.cpp"
#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;

#line 1 "algo/data-structure/segtree.hpp"
template <class S, S (*op)(S, S), S (*e)()>
struct segtree {
 public:
  segtree() : segtree(0) {}
  explicit segtree(int n) : segtree(vector<S>(n, e())) {}
  explicit segtree(const vector<S>& v) : n(int(v.size())) {
    log = ceil(log2(double(n)));
    size = 1 << log;
    d = vector<S>(2 * size, e());
    for (int i = 0; i < n; i++) d[size + i] = v[i];
    for (int i = size - 1; i >= 1; i--) {
      update(i);
    }
  }

  void set(int p, S x) {
    assert(0 <= p && p < n);
    p += size;
    d[p] = x;
    for (int i = 1; i <= log; i++) update(p >> i);
  }

  S get(int p) const {
    assert(0 <= p && p < n);
    return d[p + size];
  }

  S prod(int l, int r) const {
    assert(0 <= l && l <= r && r <= n);
    S sml = e(), smr = e();
    l += size;
    r += size;

    while (l < r) {
      if (l & 1) sml = op(sml, d[l++]);
      if (r & 1) smr = op(d[--r], smr);
      l >>= 1;
      r >>= 1;
    }
    return op(sml, smr);
  }

  S all_prod() const { return d[1]; }

  template <bool (*f)(S)>
  int max_right(int l) const {
    return max_right(l, [](S x) { return f(x); });
  }
  template <class F>
  int max_right(int l, F f) const {
    assert(0 <= l && l <= n);
    assert(f(e()));
    if (l == n) return n;
    l += size;
    S sm = e();
    do {
      while (l % 2 == 0) l >>= 1;
      if (!f(op(sm, d[l]))) {
        while (l < size) {
          l = (2 * l);
          if (f(op(sm, d[l]))) {
            sm = op(sm, d[l]);
            l++;
          }
        }
        return l - size;
      }
      sm = op(sm, d[l]);
      l++;
    } while ((l & -l) != l);
    return n;
  }

  template <bool (*f)(S)>
  int min_left(int r) const {
    return min_left(r, [](S x) { return f(x); });
  }
  template <class F>
  int min_left(int r, F f) const {
    assert(0 <= r && r <= n);
    assert(f(e()));
    if (r == 0) return 0;
    r += size;
    S sm = e();
    do {
      r--;
      while (r > 1 && (r % 2)) r >>= 1;
      if (!f(op(d[r], sm))) {
        while (r < size) {
          r = (2 * r + 1);
          if (f(op(d[r], sm))) {
            sm = op(d[r], sm);
            r--;
          }
        }
        return r + 1 - size;
      }
      sm = op(d[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

 private:
  int n, size, log;
  vector<S> d;

  void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
};
#line 9 "Subarray_Sum_Queries.cpp"

struct S {
  ll pre, suf, sum, ans;
};

S e() { return S{0, 0, 0, 0}; }

S op(S a, S b) {
  return S{max({a.pre, a.sum + b.pre, 0ll}), max({b.suf, b.sum + a.suf, 0ll}),
           a.sum + b.sum, max({a.ans, b.ans, a.suf + b.pre, 0ll})};
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr), cin.exceptions(cin.failbit);
  int n, q;
  cin >> n >> q;
  segtree<S, op, e> st(n);
  for (int i = 0; i < n; ++i) {
    int x;
    cin >> x;
    st.set(i, S{x, x, x, x});
  }
  while (q--) {
    int pos, x;
    cin >> pos >> x;
    --pos;
    st.set(pos, S{x, x, x, x});
    cout << st.all_prod().ans << '\n';
  }
}
