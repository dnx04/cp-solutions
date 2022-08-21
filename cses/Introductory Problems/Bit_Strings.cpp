#include <bits/stdc++.h>

using namespace std;

#ifdef local
#include "lib/prettyprint.hpp"
#endif

typedef unsigned long long ull;

template <ull mod>
struct Mod {
 private:
  ull v;
  static ull inv(ull a, ull m) {
    a %= m;
    assert(a);
    return a == 1 ? 1 : ull(m - __uint128_t(inv(m, a)) * m / a);
  }

 public:
  Mod() : v(0){};
  Mod(ull _v) : v((_v % mod + mod) % mod){};
  Mod inv() const {
    Mod res;
    res.v = inv(v, mod);
    return res;
  }
  friend Mod inv(const Mod& a) { return a.inv(); }
  Mod& operator+=(const Mod& a) {
    v += a.v;
    if (v >= mod) v -= mod;
    return *this;
  }
  Mod& operator-=(const Mod& a) {
    v += mod - a.v;
    if (v >= mod) v -= mod;
    return *this;
  }
  Mod& operator*=(const Mod& a) {
    v = __uint128_t(v) * a.v % mod;
    return *this;
  }
  Mod& operator/=(const Mod& a) { return *this *= a.inv(); }
  friend Mod operator+(const Mod& a, const Mod& b) { return Mod(a) += b; }
  friend Mod operator-(const Mod& a, const Mod& b) { return Mod(a) -= b; }
  friend Mod operator*(const Mod& a, const Mod& b) { return Mod(a) *= b; }
  friend Mod operator/(const Mod& a, const Mod& b) { return Mod(a) /= b; }
  Mod operator^(ull y) {
    if (!y) return Mod(1);
    Mod r = *this ^ (y >> 1);
    r = r * r;
    return y & 1 ? *this * r : r;
  }
  friend ostream& operator<<(std::ostream& out, const Mod& a) {
    return out << a.v;
  }
  friend istream& operator>>(std::istream& in, Mod& a) {
    ull v;
    in >> v;
    a = Mod(v);
    return in;
  }
};

signed main() {
  ios::sync_with_stdio(false), cin.tie(0);
  int n;
  cin >> n;
  Mod<1000000007> x = 2;
  cout << (x ^ n);
}