#pragma once
#include "base_template.hpp"
#include "modint_petit_p.hpp"

// FPS 積分 O(N) 定数項は 0 とする
template <ll pdiv, ll prim>
vector<ll> fps_integral(vector<ll> f) {
    f.emplace_back(0);
    DSRNG(i, f.size() - 1, 1) {
        f[i] = f[i-1] * modinv_p<pdiv>(i) % pdiv;
    }
    f[0] = 0;
    return f;
}
