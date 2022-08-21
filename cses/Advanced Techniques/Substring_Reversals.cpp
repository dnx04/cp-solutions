#line 1 "Substring_Reversals.cpp"
#include <bits/extc++.h>

using namespace std;
using namespace __gnu_cxx;
using namespace __gnu_pbds;

#line 1 "algo/data-structure/splay.hpp"
template <class K, class S, class F>
struct node_t {
  using Node = node_t<K, S, F>;

  array<Node *, 2> child;
  Node *father;
  int size;
  bool reverse;

  K key;
  S data;
  F lazy;
};
template <class K,            // key
          class S,            // node aggregate data
          S (*op)(S, K, S),   // for recomputing data of a node
          pair<K, S> (*e)(),  // identity data
          class F,            // lazy propagation tag
          pair<K, S> (*mapping)(F, node_t<K, S, F> *),  // apply tag F on a node
          F (*composition)(F, F),                       // combine 2 tags
          F (*id)()                                     // identity tag
          >
struct splay_tree {
  using Node = node_t<K, S, F>;

  Node *nil, *root;

  splay_tree() {
    initNil();
    root = nil;
  }
  splay_tree(const vector<K> &keys) {
    initNil();
    root = createNode(keys, 0, (int)keys.size());
  }

  vector<K> get_keys() {
    vector<K> keys;
    traverse(root, keys);
    return keys;
  }

  // k in [0, n-1]
  Node *kth(int k) {
    auto res = _kth(root, k);
    splay(res);
    root = res;
    return res;
  }

  // Return <L, R>:
  // - L contains [0, k-1]
  // - R contains [k, N-1]
  // Modify tree
  pair<Node *, Node *> cut(int k) {
    if (k == 0) {
      return {nil, root};
    } else if (k == root->size) {
      return {root, nil};
    } else {
      Node *left = kth(k - 1);  // kth already splayed
      Node *right = left->child[1];
      left->child[1] = right->father = nil;
      pushUp(left);
      return {left, right};
    }
  }

  // Return <X, Y, Z>:
  // - X contains [0, u-1]
  // - Y contains [u, v-1]
  // - Z contains [v, N-1]
  // This is useful for queries on [u, v-1]
  // Modify tree
  tuple<Node *, Node *, Node *> cut(int u, int v) {
    auto [xy, z] = cut(v);
    root = xy;
    auto [x, y] = cut(u);
    return {x, y, z};
  }

  // Make this tree x + y
  void join(Node *x, Node *y) {
    if (x == nil) {
      root = y;
      return;
    }
    while (1) {
      pushDown(x);
      if (x->child[1] == nil) break;
      x = x->child[1];
    }
    splay(x);
    setChild(x, y, 1);
    pushUp(x);
    root = x;
  }

  // reverse range [u, v-1]
  void reverse(int u, int v) {
    assert(0 <= u && u <= v && v <= root->size);
    if (u == v) return;

    auto [x, y, z] = cut(u, v);
    y->reverse = true;
    join(x, y);
    join(root, z);
  }

  // apply F on range [u, v-1]
  void apply(int u, int v, const F &f) {
    assert(0 <= u && u <= v && v <= root->size);
    if (u == v) return;

    auto [x, y, z] = cut(u, v);
    y->lazy = composition(f, y->lazy);
    tie(y->key, y->data) = mapping(f, y);

    join(x, y);
    join(root, z);
  }

  // Insert before pos
  // pos in [0, N]
  void insert(int pos, K key) {
    assert(0 <= pos && pos <= root->size);
    // x: [0, pos-1]
    // y: [pos, N-1]
    auto [x, y] = cut(pos);
    auto node = createNode(key);
    setChild(node, x, 0);
    setChild(node, y, 1);
    pushUp(node);
    root = node;
  }

  // Delete pos; pos in [0, N-1]
  K erase(int pos) {
    assert(0 <= pos && pos < root->size);

    // x = [0, pos-1]
    // y = [pos, pos]
    // z = [pos+1, N-1]
    auto [x, y, z] = cut(pos, pos + 1);
    join(x, z);
    return y->key;
  }

  // aggregated data of range [l, r-1]
  S prod(int l, int r) {
    auto [x, y, z] = cut(l, r);
    auto res = y->data;
    join(x, y);
    join(root, z);
    return res;
  }

 private:
  void initNil() {
    nil = new Node();
    nil->child[0] = nil->child[1] = nil->father = nil;
    nil->size = 0;
    nil->reverse = false;
    tie(nil->key, nil->data) = e();
    nil->lazy = id();
  }
  void pushUp(Node *x) {
    if (x == nil) return;
    x->size = x->child[0]->size + x->child[1]->size + 1;
    x->data = op(x->child[0]->data, x->key, x->child[1]->data);
  }
  void pushDown(Node *x) {
    if (x == nil) return;

    if (x->reverse) {
      for (auto c : x->child) {
        if (c != nil) {
          c->reverse ^= 1;
        }
      }
      swap(x->child[0], x->child[1]);
      x->reverse = false;
    }

    for (auto c : x->child) {
      if (c != nil) {
        tie(c->key, c->data) = mapping(x->lazy, c);
        c->lazy = composition(x->lazy, c->lazy);
      }
      // For problem like UPIT, where we want to push different
      // lazy tags to left & right children, may need to modify
      // code here
      // (query L R X: a(L) += X, a(L+1) += 2X, ...)
      // e.g. for UPIT:
      // x->lazy.add_left += (1 + c->size) * x->lazy.step;
    }

    x->lazy = id();
  }
  Node *createNode(K key) {
    Node *res = new Node();
    res->child[0] = res->child[1] = res->father = nil;
    res->key = key;
    res->size = 1;
    res->data = e().second;
    res->lazy = id();
    return res;
  }
  void setChild(Node *x, Node *y, int d) {
    x->child[d] = y;
    if (y != nil) y->father = x;
  }
  // Assumption: x is father of y
  int getDirection(Node *x, Node *y) {
    assert(y->father == x);
    return x->child[0] == y ? 0 : 1;
  }
  // create subtree from keys[l, r-1]
  Node *createNode(const vector<K> &keys, int l, int r) {
    if (l >= r) {  // empty
      return nil;
    }
    int mid = (l + r) / 2;
    Node *p = createNode(keys[mid]);
    Node *left = createNode(keys, l, mid);
    Node *right = createNode(keys, mid + 1, r);

    setChild(p, left, 0);
    setChild(p, right, 1);

    pushUp(p);
    return p;
  }
  void traverse(Node *x, vector<K> &keys) {
    if (x == nil) return;
    pushDown(x);
    traverse(x->child[0], keys);
    keys.push_back(x->key);
    traverse(x->child[1], keys);
  }

  void rotate(Node *x, int d) {
    Node *y = x->father;
    Node *z = x->child[d];
    setChild(x, z->child[d ^ 1], d);
    setChild(y, z, getDirection(y, x));
    setChild(z, x, d ^ 1);
    pushUp(x);
    pushUp(z);
  }
  // Make x root of tree
  Node *splay(Node *x) {
    if (x == nil) return nil;
    while (x->father != nil) {
      Node *y = x->father;
      Node *z = y->father;
      int dy = getDirection(y, x);
      int dz = getDirection(z, y);
      if (z == nil) {
        rotate(y, dy);
      } else if (dy == dz) {
        rotate(z, dz);
        rotate(y, dy);
      } else {
        rotate(y, dy);
        rotate(z, dz);
      }
    }
    return x;
  }

  Node *_kth(Node *p, int k) {
    pushDown(p);
    // left: [0, left->size - 1]
    if (k < p->child[0]->size) {
      return _kth(p->child[0], k);
    }
    k -= p->child[0]->size;
    if (!k) return p;
    return _kth(p->child[1], k - 1);
  }
};
#line 8 "Substring_Reversals.cpp"

struct S {};

struct F {};

using Node = node_t<char, S, F>;

S op(S left, char key, S right) { return S{}; };
pair<char, S> e() { return {'\0', S{}}; }
pair<char, S> mapping(F f, Node *node) { return {node->key, node->data}; }
F composition(F f, F g) { return {}; }
F id() { return {}; }

void solve(int ith) {
  int n, q;
  cin >> n >> q;
  splay_tree<char, S, op, e, F, mapping, composition, id> st;
  for (int i = 0; i < n; ++i) {
    char ch;
    cin >> ch;
    st.insert(i, ch);
  }

  for (int i = 0; i < q; ++i) {
    int l, r;
    cin >> l >> r;
    st.reverse(l - 1, r);
  }
  auto ans = st.get_keys();
  for (auto val : ans) {
    cout << val;
  }
}

signed main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr), cin.exceptions(cin.failbit);
  int tc = 1;
  // cin >> tc;
  for (int i = 1; i <= tc; ++i) solve(i);
}
