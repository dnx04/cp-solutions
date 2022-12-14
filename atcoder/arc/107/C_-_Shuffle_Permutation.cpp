#line 1 "C_-_Shuffle_Permutation.cpp"
#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;

#line 2 "algo/number-theory/modint.hpp"

using ll = long long;

template <const ll m>
struct Mod {
  ll v;

  Mod() : v(0){};
  Mod(ll v) : v((v + m) % m){};
  explicit operator ll() { return v; }
  Mod inv() const {
    ll a = v, b = m, ax = 1, bx = 0;
    while (b) {
      ll q = a / b, t = a % b;
      a = b, b = t, t = ax - bx * q, ax = bx, bx = t;
    }
    assert(a == 1);
    ax = (ax < 0 ? ax + m : ax);
    return ax;
  }
  Mod& operator+=(const Mod& that) {
    v = (v + that.v >= m ? v + that.v - m : v + that.v);
    return *this;
  }
  Mod& operator-=(const Mod& that) {
    v = (v >= that.v ? v - that.v : v + m - that.v);
    return *this;
  }
  Mod& operator*=(const Mod& that) {
    v = v * that.v % m;
    return *this;
  }
  Mod& operator/=(const Mod& that) { return (*this) *= that.inv(); }
  Mod operator^(ll y) {
    if (!y) return Mod(1);
    Mod r = *this ^ (y >> 1);
    r = r * r;
    return y & 1 ? *this * r : r;
  }
  Mod operator+(const Mod& that) const { return Mod(*this) += that; }
  Mod operator-(const Mod& that) const { return Mod(*this) -= that; }
  Mod operator*(const Mod& that) const { return Mod(*this) *= that; }
  Mod operator/(const Mod& that) const { return Mod(*this) /= that; }
  bool operator==(const Mod& that) const { return this->v == that.v; }
  friend istream& operator>>(istream& in, Mod& that) {
    ll val;
    in >> val;
    that = Mod(val);
    return in;
  }
  friend ostream& operator<<(ostream& out, const Mod& that) {
    return out << that.v;
  }
};
#line 1 "algo/data-structure/dsu.hpp"
struct dsu {
 public:
  dsu(int n) : n(n), p(n, -1) {}

  int merge(int a, int b) {
    assert(0 <= a && a < n);
    assert(0 <= b && b < n);
    int x = head(a), y = head(b);
    if (x == y) return x;
    if (-p[x] < -p[y]) swap(x, y);
    p[x] += p[y];
    p[y] = x;
    return x;
  }

  bool same(int a, int b) {
    assert(0 <= a && a < n);
    assert(0 <= b && b < n);
    return head(a) == head(b);
  }

  int head(int a) {
    assert(0 <= a && a < n);
    if (p[a] < 0) return a;
    return p[a] = head(p[a]);
  }

  int size(int a) {
    assert(0 <= a && a < n);
    return -p[head(a)];
  }

  vector<vector<int>> groups() {
    vector<int> tmp(n), sz(n);
    for (int i = 0; i < n; ++i) tmp[i] = head(i), ++sz[tmp[i]];
    vector<vector<int>> gr(n);
    for (int i = 0; i < n; ++i) gr[i].reserve(sz[i]);
    for (int i = 0; i < n; ++i) gr[tmp[i]].push_back(i);
    gr.erase(remove_if(begin(gr), end(gr),
                       [&](const vector<int>& v) { return v.empty(); }),
             end(gr));
    return gr;
  }

 private:
  int n;
  vector<int> p;
};
#line 10 "C_-_Shuffle_Permutation.cpp"

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr), cin.exceptions(cin.failbit);
  using Fp = Mod<998244353>;
  int n, k;
  cin >> n >> k;
  int a[n][n];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      cin >> a[i][j];
  dsu ufr(n), ufc(n);
  vector<Fp> fac(n + 1, 1);
  for (int i = 1; i <= n; i++)
    fac[i] = fac[i - 1] * i;
  for(int x = 0; x < n; ++x) {
    for(int y = 0; y < n; ++y) {
      bool ok = true;
      for(int i = 0; i < n; ++i) {
        if(a[x][i] + a[y][i] > k) {
          ok = false;
          break;
        }
      }
      if(ok) ufr.merge(x, y);
    }
  }
  for(int x = 0; x < n; ++x) {
    for(int y = 0; y < n; ++y) {
      bool ok = true;
      for(int i = 0; i < n; ++i) {
        if(a[i][x] + a[i][y] > k) {
          ok = false;
          break;
        }
      }
      if(ok) ufc.merge(x, y);
    }
  }
  Fp ans = 1;
  for(auto gr: ufr.groups()) ans *= fac[gr.size()];
  for(auto gr: ufc.groups()) ans *= fac[gr.size()];
  cout << ans;
}
