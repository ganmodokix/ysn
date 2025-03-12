#pragma once
#include "base_template.hpp"
#include "conv/ntt.hpp"

// Shift of Sampling Points of Polynomial
// ref: https://suisen-cp.github.io/cp-library-cpp/library/polynomial/shift_of_sampling_points.hpp
// verify: https://judge.yosupo.jp/submission/271976
namespace sspp_998244353 {

    namespace fac_cache {

        constexpr auto pdiv = 998244353LL;

        vector<ll> fac = {1};
        vector<ll> invfac = {1};
        void make_cache(ll new_size) {
            const auto old_size = (ll)fac.size();
            if (new_size <= old_size) return;
            fac.resize(new_size);
            invfac.resize(new_size);
            RANGE(k, old_size, new_size - 1) {
                fac[k] = fac[k - 1] * k % pdiv;
            }
            invfac[new_size - 1] = modinv(fac[new_size - 1], pdiv);
            DSRNG(k, new_size - 2, old_size) {
                invfac[k] = invfac[k + 1] * (k + 1) % pdiv;
            }
        }
    };

    constexpr auto pdiv = 998244353LL;
    constexpr auto prim = 3LL;

    // Finds (a_i)_{i=0}^{N-1} s.t. f(x) = \sum_{i=0}^{N-1} a_i x^\underline{n}
    // f: sampling points (f(i))_{i=0}^{N-1}
    vector<ll> find_falling_factorial_poly(vector<ll> f) {
        // a_i = \sum_{i=0}^{N-1} \frac{f(j)}{j!} \cdot \frac{(-1)^{i-j}}{(i-j)!}
        const auto n = f.size();

        fac_cache::make_cache(n);
        using fac_cache::invfac;

        REP(i, n) {
            (f[i] *= invfac[i]) %= pdiv;
        }

        auto v = vector<ll>(invfac.begin(), invfac.begin() + n);
        STEP(i, 1, n-1, 2) {
            v[i] = v[i] == 0 ? 0 : pdiv - v[i];
        }
        
        auto wv = convolve_p<pdiv, prim>(f, v);
        wv.resize(n);
        return wv;
    }

    // ssps
    vector<ll> sspp(vector<ll> f, const ll c_, const ll m) {
        const auto c = (c_ % pdiv + pdiv) % pdiv;
        const auto n = f.size();
        
        fac_cache::make_cache(max<ll>(n, m));
        using fac_cache::fac;
        using fac_cache::invfac;
        
        auto a = find_falling_factorial_poly(move(f));

        REP(i, n) {
            (a[i] *= fac[i]) %= pdiv;
        }
        ranges::reverse(a);

        auto cffp = vector<ll>(n, 1LL);
        RANGE(i, 1, n-1) {
            cffp[i] = cffp[i-1] * (c + pdiv - i + 1) % pdiv;
        }
        REP(i, n) (cffp[i] *= invfac[i]) %= pdiv;

        auto b = convolve_p<pdiv, prim>(move(cffp), move(a));
        b.resize(n);

        ranges::reverse(b);
        REP(i, n) (b[i] *= invfac[i]) %= pdiv;

        auto invfacn = vector<ll>(invfac.begin(), invfac.begin() + m);
        auto ans = convolve_p<pdiv, prim>(move(b), move(invfacn));
        ans.resize(m);
        REP(i, m) {
            (ans[i] *= fac[i]) %= pdiv;
        }

        return ans;
    }

}