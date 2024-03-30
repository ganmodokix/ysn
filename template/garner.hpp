// #REQ: base_template.cpp modint_petit.cpp
// Garnerのアルゴリズム O(|w|^2)
// w[i] = {x[i], m[i]} とすると {x === x[i] mod m[i]} から x mod pdiv を復元
// w[i].second (m[i]) は pairwise 互いに素である必要がある
ll garner(const vector<pair<ll, ll>> &w, const ll pdiv) {
    vector<ll> p(w.size()+1, 1);
    vector<ll> v(w.size()+1, 0);
    for (size_t k = 0; k < w.size(); k++) {
        ll xk, mk; tie(xk, mk) = w[k];
        ll t = ((xk - v[k]) % mk + mk) % mk * modinv(p[k], mk) % mk;
        for (size_t i = k+1; i <= w.size(); i++) {
            ll bi = i == w.size() ? pdiv : w[i].second;
            (v[i] += t * p[i]) %= bi;
            (p[i] *= w[k].second) %= bi;
        }
    }
    return v.back();
};