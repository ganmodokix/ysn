#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "modint/modint_factorial_cache.hpp"
#include "conv/ntt.hpp"

// Shift of Sampling Points of Polynomial
// ref: https://suisen-cp.github.io/cp-library-cpp/library/polynomial/shift_of_sampling_points.hpp
// verify: https://judge.yosupo.jp/submission/279497
namespace sspp {

    // Finds (a_i)_{i=0}^{N-1} s.t. f(x) = \sum_{i=0}^{N-1} a_i x^\underline{n}
    // f: sampling points (f(i))_{i=0}^{N-1}
    template<mod_integral T>
    vector<T> find_falling_factorial_poly(vector<T> f) {
        // a_i = \sum_{i=0}^{N-1} \frac{f(j)}{j!} \cdot \frac{(-1)^{i-j}}{(i-j)!}
        const auto n = f.size();

        auto& fac_cache = get_fac_cache_singleton<T>();
        fac_cache.prepare(n);
        const auto& invfac = fac_cache.invfac;

        REP(i, n) {
            f[i] *= invfac[i];
        }

        auto v = vector(invfac.begin(), invfac.begin() + n);
        STEP(i, 1, n-1, 2) {
            v[i] = -v[i];
        }
        
        auto wv = convolve_p(f, v);
        wv.resize(n);
        return wv;
    }

    
    // ssps, (f(i))_{i=0}^{N-1} が得られているときに (f(c + i))_{i=0}^{M-1} を求める
    template<mod_integral T>
    vector<T> sspp(vector<T> f, const ll c_, const size_t m) {
        const auto c = T{c_};
        const auto n = f.size();
        
        auto a = find_falling_factorial_poly(move(f));
        
        auto& fac_cache = get_fac_cache_singleton<T>();
        fac_cache.prepare((ll)max(n, m));
        const auto& fac = fac_cache.fac;
        const auto& invfac = fac_cache.invfac;

        REP(i, n) {
            a[i] *= fac[i];
        }
        ranges::reverse(a);

        auto cffp = vector(n, T{1});
        RANGE(i, 1, n-1) {
            cffp[i] = cffp[i-1] * (c - i + 1);
        }
        REP(i, n) cffp[i] *= invfac[i];

        auto b = convolve_p(move(cffp), move(a));
        b.resize(n);

        ranges::reverse(b);
        REP(i, n) b[i] *= invfac[i];

        auto invfacn = vector(invfac.begin(), invfac.begin() + m);
        auto ans = convolve_p(move(b), move(invfacn));
        ans.resize(m);
        REP(i, m) {
            ans[i] *= fac[i];
        }

        return ans;
    }

}
