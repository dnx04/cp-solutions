#line 1 "E_Placing_Jinas.cpp"
#include <bits/stdc++.h>

using namespace std;

using ll = long long;

#line 1 "algo/number-theory/modint.hpp"
using ll = long long;

template <const ll m>
struct Mod {
  ll v;

  Mod() : v(0){};
  Mod(ll _v) : v((_v + m) % m){};
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
#line 8 "E_Placing_Jinas.cpp"

void solve(int ith) {
  using mint = Mod<1000000007>;
  const int N = 4e5 + 10;
  vector<mint> fac(N, 1);
  for (int i = 2; i < N; ++i) fac[i] = fac[i - 1] * i;
  auto C = [&](int n, int k) -> mint {
    if (n < k) return 0;
    return fac[n] / fac[k] / fac[n - k];
  };
  int n;
  cin >> n;
  vector<int> a(n + 1);
  for (auto &x : a) cin >> x;
  mint ans = 0;
  for (int i = 0; i <= n; ++i) ans += C(i + a[i], i + 1);
  cout << ans << '\n';
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr), cin.exceptions(cin.failbit);
  int tc = 1;
  // cin >> tc;
  for (int i = 1; i <= tc; ++i) solve(i);
}
