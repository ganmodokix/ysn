#pragma once
#include "base_template.hpp"

template <integral T>
constexpr T euler_phi(T n) {
    assert(n > 0);

    auto phi = n;
    for (auto i = T{2}; i <= n / i; ++i) {
        if (n % i == 0) {
            phi -= phi / i;
            do { n /= i; } while (n % i == 0);
        }
    }

    if (n > 1) {
        phi -= phi / n;
    }

    return phi;
}