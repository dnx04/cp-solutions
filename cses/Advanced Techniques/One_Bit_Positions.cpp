#line 1 "One_Bit_Positions.cpp"
#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;

#line 1 "algo/convolution/fft.hpp"
using cd = complex<double>;

const double PI = acos(-1);

void fft(vector<cd>& a, bool invert) {
  int n = a.size();

  for (int i = 1, j = 0; i < n; i++) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;

    if (i < j) swap(a[i], a[j]);
  }

  for (int len = 2; len <= n; len <<= 1) {
    double ang = 2 * PI / len * (invert ? -1 : 1);
    cd wlen(cos(ang), sin(ang));
    for (int i = 0; i < n; i += len) {
      cd w(1);
      for (int j = 0; j < len / 2; j++) {
        cd u = a[i + j], v = a[i + j + len / 2] * w;
        a[i + j] = u + v;
        a[i + j + len / 2] = u - v;
        w *= wlen;
      }
    }
  }

  if (invert) {
    for (cd& x : a) x /= n;
  }
}

template <typename T>
vector<T> fftconv(vector<T> const& a, vector<T> const& b) {
  vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
  int n = 1;
  while (n < int(a.size() + b.size())) n <<= 1;
  fa.resize(n);
  fb.resize(n);

  fft(fa, false);
  fft(fb, false);
  for (int i = 0; i < n; i++) fa[i] *= fb[i];
  fft(fa, true);

  vector<T> result(n);
  for (int i = 0; i < n; i++) result[i] = round(fa[i].real());
  return result;
}
#line 9 "One_Bit_Positions.cpp"

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr), cin.exceptions(cin.failbit);
  string s;
  cin >> s;
  vector<ll> a(begin(s), end(s));
  for (auto& x : a) x -= '0';
  auto ra = a;
  reverse(begin(ra), end(ra));
  auto conv = fftconv(a, ra);
  int n = s.size();
  for (int i = 1; i < n; ++i) cout << conv[n - 1 + i] << ' ';
}
