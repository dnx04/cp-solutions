#include <bits/extc++.h>

using namespace std;
using namespace __gnu_cxx;
using namespace __gnu_pbds;

using ll = long long;
const int N = 3000;
const ll INF = numeric_limits<ll>::max();

vector<ll> a(N + 1), dp(N + 1), ndp(N + 1);

long long C(int i, int j) { return (a[j] - a[i - 1]) * (a[j] - a[i - 1]); }

void compute(int l, int r, int optl, int optr) {
  if (l > r) return;

  int mid = (l + r) >> 1;
  pair<ll, ll> best = {INF, -1};

  for (int k = optl; k <= min(mid, optr); k++) {
    best = min(best, {(k ? dp[k - 1] : 0) + C(k, mid), k});
  }

  ndp[mid] = best.first;
  int opt = best.second;

  compute(l, mid - 1, optl, opt);
  compute(mid + 1, r, opt, optr);
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr), cin.exceptions(cin.failbit);
  int n, k;
  cin >> n >> k;
  for (int i = 1; i <= n; ++i) cin >> a[i], a[i] += a[i - 1];
  for (int i = 1; i <= n; ++i) dp[i] = C(1, i);
  for (int i = 1; i < k; ++i) compute(1, n, 1, n), dp = ndp;
  cout << dp[n];
}