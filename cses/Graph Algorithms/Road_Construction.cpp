#include "bits/extc++.h"

using namespace std;

#ifdef DEBUG
#include "lib/prettyprint.hpp"
#endif

struct uf {
 public:
  uf(int _n) : n(_n), p(_n, -1) {}

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

signed main() {
  ios::sync_with_stdio(false), cin.tie(0);
  int n, m;
  cin >> n >> m;
  uf dsu(n);

  for (int i = 0, cc = n, mx = 1; i < m; ++i) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    if (!dsu.same(a, b)) --cc;
    mx = max(mx, dsu.size(dsu.merge(a, b)));
    cout << cc << ' ' << mx << '\n';
  }
}