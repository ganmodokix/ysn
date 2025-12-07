#pragma once
#include "base_template.hpp"

namespace ganmodokix {

    struct cartesian_tree_result {
        static constexpr auto NONE = ptrdiff_t{-1LL};  // 無効を表す値
        vector<ptrdiff_t> par;
        vector<pair<ptrdiff_t, ptrdiff_t>> children;
        ptrdiff_t root = NONE;
    };

    ostream& __dump_single(const cartesian_tree_result& r) {
        cerr << "ctree{\e[2mpar=\e[m";
        __dump_single(r.par) << "\e[2m,\e[m\e[2mchildren=\e[m";
        return __dump_single(r.children) << "\n}";
    }

}

// Cartesian Tree による treap 構築: O(N)
// verified at https://atcoder.jp/contests/abc435/submissions/71531370
// 昇順で構築されるので、最大値を根にしたい場合は ::std::greater を渡す
// 同値関係にある要素があった場合 pair{value, index} での比較が行われるはず
template <
    ranges::forward_range R,
    typename Comp = ::std::less<>,
    typename Proj = ::std::identity
>
auto cartesian_tree(R&& r, Comp comp = {}, Proj proj = {})
    -> ganmodokix::cartesian_tree_result
{
    // aliases
    using ganmodokix::cartesian_tree_result;
    constexpr auto NONE = cartesian_tree_result::NONE;
    using value_type = ranges::range_value_t<R>;
    struct stack_value {
        value_type value;
        ptrdiff_t index;
    };

    // parent / children of binary tree vertices
    auto par = vector<ptrdiff_t>{};
    auto children = vector<pair<ptrdiff_t, ptrdiff_t>>{};
    if constexpr (ranges::sized_range<R>) {
        const auto size = ranges::size(r);
        par.reserve(size);
        children.reserve(size);
    }

    // stack-based construction
    auto st = vector<stack_value>{};  // {value, index}
    auto root = NONE;
    for (auto&& ritem : r) {
        const auto i = ssize(par);
        auto&& value = invoke(proj, forward<decltype(ritem)>(ritem));
        children.emplace_back(NONE, NONE);
        // 左にある自分より大きい要素のうち最小を左子とする
        while (!st.empty() && invoke(comp, value, st.back().value)) {
            const auto u = st.back().index;
            children.back().first = u;
            st.pop_back();
        }
        if (const auto u = children.back().first; u != NONE) {
            par[u] = i;
        }
        // 左にある自分より小さい要素のうち最大を根とする
        par.emplace_back(!st.empty() ? st.back().index : NONE);
        if (const auto u = par.back(); u != NONE) {
            children[u].second = i;
        } else {
            root = i;
        }
        // スタックに積む
        st.emplace_back(stack_value{
            .value = forward<decltype(value)>(value),
            .index = i
        });
    }

    return cartesian_tree_result{
        .par = move(par),
        .children = move(children),
        .root = root
    };
}