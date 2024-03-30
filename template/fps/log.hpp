// #REQ: base_template.hpp fps/derivative.hpp fps/integral.hpp fps/inv.hpp conv/ntt.hpp

// FPS log O(NlogN)
template <ll pdiv, ll prim>
vector<ll> fps_log(vector<ll> f) {
    assert((f[0] % pdiv + pdiv) % pdiv == 1);
    const auto n = f.size();
    auto derf = fps_derivative<pdiv, prim>(f);
    auto invf = fps_inv<pdiv, prim>(move(f));
    auto fpf = convolve_p<pdiv, prim>(derf, invf); // f' / f
    fpf.resize(n - 1);
    return fps_integral<pdiv, prim>(fpf);
}