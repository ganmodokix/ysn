// #REQ: base_template.hpp
// 遅延評価セグ木 T: 値の型, U: 操作の型
template<typename T, typename U, typename JOIN_T, typename APPLY_U, typename JOIN_U, typename MULTIPLY_U, typename HALF_U>
class segtree {
    T IT; // T の単位元
    U IU; // U の単位元
    JOIN_T join_t; // 値同士のjoin; T(T: 左の値, T: 右の値)
    APPLY_U apply_u; // 操作を値に適用する; T(T: 操作前の値, U: 操作)
    JOIN_U join_u; // 操作をjoinする; U(U: 古い操作, U: 新しい操作)
    MULTIPLY_U multiply_u; // 操作をn回やる操作を作る; U(U: 操作, ll: 2冪の正整数)
    HALF_U half_u; // 半分の操作を作る; U(U: multiply_uによって2冪倍された操作)
    ll n; // 元の配列のサイズ
    vector<T>    node;
    vector<U>    lazy;
    vector<char> fall; // vector<bool>は空間が良いが時間が悪いのでvector<char>で代用
    // [l, r) を見ており, k を遅延評価
    inline void eval(ll k, ll l, ll r) {
        if (!fall[k]) return;
        node[k] = apply_u(node[k], lazy[k]);
        if (r-l > 1) { // 子があるなら伝播
            lazy[k*2+1] = join_u(lazy[k*2+1], half_u(lazy[k]));
            lazy[k*2+2] = join_u(lazy[k*2+2], half_u(lazy[k]));
            fall[k*2+1] = fall[k*2+2] = true;
        }
        lazy[k] = IU;
        fall[k] = false;
    }
public:
    // コンストラクタ
    segtree(const vector<T> &v, T _it, U _iu, JOIN_T _jot, APPLY_U _apu, JOIN_U _jou, MULTIPLY_U _muu, HALF_U _hau):
        IT(_it), IU(_iu), join_t(_jot), apply_u(_apu), join_u(_jou), multiply_u(_muu), half_u(_hau)
    {
        ll sz = (ll)v.size();
        n = 1; while (n < sz) n *= 2;
        this->node = vector<T   >(n * 2 - 1, IT);
        this->lazy = vector<U   >(n * 2 - 1, IU);
        this->fall = vector<char>(n * 2 - 1, false);
        for (ll i = 0; i < sz; i++) node[n-1 + i] = v[i];
        for (ll i = n-1; i--; )  node[i] = join_t(node[i*2+1], node[i*2+2]);
    }
    // 区間 [a, b) に x を適用（但し, [l, r)を見て今kをいじっている）
    void apply(ll a, ll b, U x, ll k = 0, ll l = 0, ll r = -1) {
        if (r < 0) r = n; // デフォルト値には定数しか入れられない
        eval(k, l, r);
        if (b <= l || r <= a) return; // セグフォ防止
        if (a <= l && r <= b) {
            lazy[k] = join_u(lazy[k], multiply_u(x, r - l));
            fall[k] = true;
            eval(k, l, r);
        } else {
            apply(a, b, x, k*2+1, l, (l+r)/2);
            apply(a, b, x, k*2+2, (l+r)/2, r);
            node[k] = join_t(node[k*2+1], node[k*2+2]);
        }
    }
    // 区間 [a, b) の操作後の値を求める
    T fetch(ll a, ll b, ll k = 0, ll l = 0, ll r = -1) {
        if (r < 0) r = n; // デフォルト値には定数しか入れられない
        if (b <= l || r <= a) return IT; // セグフォ防止
        eval(k, l, r);
        if (a <= l && r <= b) return node[k];
        T vl = fetch(a, b, k*2+1, l, (l+r)/2);
        T vr = fetch(a, b, k*2+2, (l+r)/2, r);
        return join_t(vl, vr);
    }
    // 点 a == 区間 [a, a+1) に x を適用
    inline void applyp(ll a, U x) { apply(a, a+1, x); }
    // 点 a == 区間 [a,a+1) の操作後の値を求める
    inline T fetchp(ll a) { return fetch(a, a+1); }
};
// C++17以降の環境ではこの関数は不要 (テンプレート引数推論が働くため)
template<typename T, typename U, typename JOIN_T, typename APPLY_U, typename JOIN_U, typename MULTIPLY_U, typename HALF_U>
segtree<T, U, JOIN_T, APPLY_U, JOIN_U, MULTIPLY_U, HALF_U> make_segtree(const vector<T> &v, T _it, U _iu, JOIN_T _jot, APPLY_U _apu, JOIN_U _jou, MULTIPLY_U _muu, HALF_U _hau) {
    return segtree<T, U, JOIN_T, APPLY_U, JOIN_U, MULTIPLY_U, HALF_U>(v, _it, _iu, _jot, _apu, _jou, _muu, _hau);
}
// おまけ
const auto SEGTREE_ASGF = [](ll, ll y) { return y; };
const auto SEGTREE_EYE2 = [](ll x, ll) { return x; };
const auto SEGTREE_EYE = [](ll x) { return x; };
auto make_segtree_rminq(const vector<ll> &v) {
    auto minf = [](ll x, ll y) { return min(x, y); };
    return make_segtree(v, LINF, LINF, minf, minf, minf, SEGTREE_EYE2, SEGTREE_EYE);
    // return make_segtree(v, LINF, LINF, minf, SEGTREE_ASGF, SEGTREE_ASGF, SEGTREE_EYE2, SEGTREE_EYE);
}
auto make_segtree_rmaxq(const vector<ll> &v) {
    auto maxf = [](ll x, ll y) { return max(x, y); };
    return make_segtree(v, -LINF, -LINF, maxf, maxf, maxf, SEGTREE_EYE2, SEGTREE_EYE);
    // return make_segtree(v, -LINF, -LINF, maxf, SEGTREE_ASGF, SEGTREE_ASGF, SEGTREE_EYE2, SEGTREE_EYE);
}
auto make_segtree_raddq(const vector<ll> &v) {
    auto addf = [](ll x, ll y) { return x + y; };
    auto mltp = [](ll x, ll n) { return x * n; };
    auto hlff = [](ll x) { return x / 2; };
    return make_segtree(v, 0LL, 0LL, addf, addf, addf, mltp, hlff);
}