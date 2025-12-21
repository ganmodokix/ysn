#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "fps/derivative.hpp"
#include "fps/integral.hpp"
#include "fps/inv.hpp"
#include "conv/ntt.hpp"

// FPS log O(NlogN)
template <mod_integral T>
constexpr vector<T> fps_log(vector<T> f) {
    assert(f[0] == 1);
    if (ssize(f) == 1) return vector{T{0}};
    const auto n = f.size();
    auto derf = fps_derivative(f);
    auto invf = fps_inv(move(f));
    auto fpf = convolve_p(move(derf), move(invf)); // f' / f
    fpf.resize(n - 1);
    return fps_integral(move(fpf)); // \int \frac{f'}{f} = \log (f)
}
