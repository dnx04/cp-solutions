#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr), cin.exceptions(cin.failbit);
  int tc;
  cin >> tc;
  auto optimize = [&](int low, int high, auto f) -> ll {
    while (high - low > 2) {
      const int m1 = (low + high) / 2;
      const int m2 = m1 + 1;
      if (f(m1) < f(m2)) {
        low = m1;
      } else {
        high = m2;
      }
    }
    return f(low + 1);
  };
  while (tc--) {
    int n, m;
    cin >> n >> m;
    ll a = 0, b = 0, ans = numeric_limits<ll>::min();
    for (int i = 0; i < n; ++i) {
      ll x, y;
      cin >> x >> y;
      const auto f = [&](const int k) {
        return a + b * k + x * k * (k + 1) / 2;
      };
      if (x > 0)
        ans = max({ans, f(1), f(y)});
      else
        ans = max(ans, optimize(0, y + 1, f));
      a = f(y);
      b += x * y;
    }
    cout << ans << '\n';
  }
}