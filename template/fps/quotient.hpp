#pragma once
#include "base_template.hpp"
#include "conv/ntt.hpp"
#include "fps/inv.hpp"

// FPS 商 O(NlogN)
template <ll pdiv, ll prim>
vector<ll> fps_quotient(vector<ll> f, vector<ll> g) {
    const auto n = f.size();
    const auto m = g.size();
    assert(n >= m);
    reverse(ALL(f));
    reverse(ALL(g));
    const auto k = n - m + 1;
    f.resize(k);
    g.resize(k);
    g = fps_inv<pdiv, prim>(move(g));
    auto q = convolve_p<pdiv, prim>(move(f), move(g));
    q.resize(k);
    reverse(ALL(q));
    return q;
}
