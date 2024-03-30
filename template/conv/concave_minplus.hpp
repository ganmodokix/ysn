#pragma once
#include "base_template.hpp"
#include "monge/smawk.hpp"

// C_i = min_j { A_j + B_{i-j} } (B: 下に凸) を求める O(|A|+|B|)
// ⚠ idがlessの単位元である必要がある
//    (comp=less{}, id=INF), (comp=greater{}, id=-INF) など
//    id は射影 proj を掛けた後のものであることに注意
// ⚠ bは(下に)凸である必要がある
template <typename T, typename Comp = less<>, typename Proj = identity>
vector<T> convolve_concave_min_plus(
    const vector<T> &a, const vector<T> &b,
    Comp comp = {},
    const T id = numeric_limits<T>::max(),
    Proj proj = {}
) {
    auto func = [&a, &b, &id](const auto i, const auto j) {
        if (i < j || i - j >= b.size()) {
            return id;
        }
        return a[j] + b[i - j];
    };

    auto mm = smawk(
        a.size() + b.size() - 1, a.size(),
        func,
        comp, proj
    );

    auto c = vector<T>(mm.size());

    RPE(i, a.size() + b.size() - 1) {
        c[i] = func(static_cast<size_t>(i), mm[i]);
    }

    return c;
    
}
