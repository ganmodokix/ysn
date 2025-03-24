#pragma once
#include "base_template.hpp"

template <typename T>
struct run_length_segment {
    size_t length = {};
    const T& content = {};
};
template <typename T>
ostream& __dump_single(const run_length_segment<T>& value) {
    using namespace ganmodokix;
    cerr << "rl{";
    __dump_single(value.content) << "\e[2m*\e[m";
    return __dump_single(value.length) << "}";
}

// ランレングス圧縮 O(N * (中身の比較))
// 各区間の先頭の要素への参照が保持される
template <ranges::input_range R, typename Pred = ranges::equal_to>
auto run_length(R&& r, Pred pred = {}) {
    using T = ranges::range_value_t<R>;
    auto rl = vector<run_length_segment<T>>{};
    for (const auto& x : r) {
        if (rl.empty() || !invoke(pred, rl.back().content, x)) {
            rl.emplace_back(run_length_segment{.length = 0, .content = x});
        }
        ++rl.back().length;
    }
    return rl;
}
