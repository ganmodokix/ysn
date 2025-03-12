#pragma once
#include "base_template.hpp"
// 与えられた列 r を、要素が等価な最長の連続部分列に分割して返す
// ランレングス圧縮と等価
template <
    ranges::random_access_range R,
    typename Comp = ::std::less<>,
    typename Proj = ::std::identity
>
auto equiv_subranges(R&& r, Comp comp = {}, Proj proj = {}) {
    auto result = vector<ranges::subrange<ranges::iterator_t<R>>>{};
    for (auto it = ranges::begin(r); it != ranges::end(r); ) {
        auto jt = it;
        while (jt != ranges::end(r)) {
            auto&& pit = invoke(proj, *it);
            auto&& pjt = invoke(proj, *jt);
            if (invoke(comp, pit, pjt) || invoke(comp, pjt, pit)) break;
            ++jt;
        }
        result.emplace_back(it, jt);
        it = jt;
    }
    return result;
}
