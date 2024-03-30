// #REQ: base_template.hpp

// FPS 微分 O(N)
template <ll pdiv, ll prim>
vector<ll> fps_derivative(vector<ll> f) {
    if (f.empty()) return {};
    REP(i, f.size() - 1) {
        f[i] = f[i+1] * (i + 1) % pdiv;
    }
    f.pop_back();
    return f;
}