#pragma once
#include "base_template.hpp"
template <typename T>
struct factorial_cache {

    vector<T> fac = {T{1}}, invfac = {T{1}};

    void prepare(long long nmax) {

        if (nmax < (long long)fac.size()) return;
        while (!(nmax < (long long)fac.size())) {
            fac.emplace_back(fac.back() * fac.size());
        }

        long long m0 = invfac.size();
        invfac.resize(nmax+1);
        invfac[nmax] = fac[nmax].inv();
        for (long long n = nmax; --n >= m0; ) {
            invfac[n] = invfac[n+1] * (n+1);
        }

    }

};


template<typename T>
factorial_cache<T>& get_fac_cache_singleton() {
    static auto fac_cache = factorial_cache<T>{};
    return fac_cache;
}