#include <bits/extc++.h>

using namespace std;

#ifdef local
#include "lib/prettyprint.hpp"
#endif

signed main() {
  ios::sync_with_stdio(false), cin.tie(0);
  freopen("diamond.in", "r", stdin);
  freopen("diamond.out", "w", stdout);
  // observation: there exist an optimal solution such that all diamonds on case 1 are < case 2
  int n, k;
  cin >> n >> k;
  vector<int> a(n), mx1(n + 1), mx2(n + 1);
  for (auto &x : a) cin >> x;
  sort(begin(a), end(a));

  // mx1[i] = how many diamonds we can take
	// assuming i is the leftmost diamond
  for (int l = 0, r = 0; l < n; ++l) {
    while (r < n && a[r] - a[l] <= k) ++r;
    mx1[l] = r - l;
  }

  // mx2[i] = max value of mx1[x] for some x >= i.
  for (int i = n - 1; i >= 0; --i) {
    mx2[i] = max(mx2[i + 1], mx1[i]);
  }
  int ans = 0;
  for (int i = 0; i < n; ++i) {
    ans = max(ans, mx1[i] + mx2[i + mx1[i]]);
  }
  cout << ans;
}