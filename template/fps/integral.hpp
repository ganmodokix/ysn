// #REQ: base_template.cpp modint_petit.cpp

// FPS 積分 O(N) 定数項は 0 とする
template <ll pdiv, ll prim>
vector<ll> fps_integral(vector<ll> f) {
    f.emplace_back(0);
    DSRNG(i, f.size() - 1, 1) {
        f[i] = f[i-1] * modinv(i, pdiv) % pdiv;
    }
    f[0] = 0;
    return f;
}