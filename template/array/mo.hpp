#pragma once
#include "base_template.hpp"
// Mo's algorithm: 半開区間クエリ [l, r) をオフラインで処理する。
// 利用側は現在区間に対応する集計状態を参照キャプチャなどの外部コンテキストに保持し、
// その初期状態を [initial_l, initial_r) と一致させる。各コールバックは状態を一要素分だけ更新し、
// solve は更新後の状態から Solution を返す。結果は入力 queries と同じ順序（重複を含む）で返る。
// {Shrink, Expand}{Left, Right} の引数は操作前の区間 (Index l, Index r)。
template <typename Solution, // コピーたくさんしても大丈夫な軽いもの，重いコンテクストとかはラムダ式のキャプチャで
    integral Index, // 添字 (クエリ範囲)
    ranges::input_range R,
    typename ShrinkLeft,
    typename ShrinkRight,
    typename ExpandLeft,
    typename ExpandRight,
    typename CalculateSolution>
requires constructible_from<pair<Index, Index>, ranges::range_reference_t<R>>
vector<Solution> mo_s_algorithm(
    R&& queries,
    ShrinkLeft shrink_left, // [l,r) -> [l+1, r)
    ShrinkRight shrink_right, // [l,r) -> [l, r-1)
    ExpandLeft expand_left, // [l,r) -> [l-1, r)
    ExpandRight expand_right, // [l,r) -> [l, r+1)
    CalculateSolution solve, // f: void -> Solution
    Index initial_l = 0,
    Index initial_r = 0
) {

    auto query_list = vector<pair<Index, Index>>{};
    if constexpr (ranges::sized_range<R>) {
        query_list.reserve(ranges::size(queries));
    }
    for (auto&& query : queries) {
        query_list.emplace_back(query);
    }
    if (query_list.empty()) return {};

    using BucketType = common_type_t<Index, ptrdiff_t>;
    
    Index query_min = numeric_limits<Index>::max();
    Index query_max = numeric_limits<Index>::min();
    for (const auto &[x, y] : query_list) {
        chmin(query_min, x);
        chmax(query_max, x);
        chmin(query_min, y);
        chmax(query_max, y);
    }
    
    const auto query_range =
        static_cast<BucketType>(query_max) - static_cast<BucketType>(query_min);
    const auto num_bucket = max(
        BucketType{1}, static_cast<BucketType>(sqrtl(query_list.size()))
    );
    const auto block_size = max(BucketType{1}, query_range / num_bucket);
    auto block = [&](Index i) {
        return (
            static_cast<BucketType>(i) - static_cast<BucketType>(query_min)
        ) / block_size;
    };

    auto sorted_query = query_list;
    UNIQUE(sorted_query);
    sort(ALL(sorted_query), [&](pair<Index, Index> a, pair<Index, Index> b) {
        const auto block_a = block(a.first);
        const auto block_b = block(b.first);
        if (block_a != block_b) {
            return block_a < block_b;
        } else {
            return block_a % 2 == 0 ? a.second < b.second : a.second > b.second;
        }
    });

    Index lcurr = initial_l, rcurr = initial_r;
    map<pair<Index, Index>, Solution> sol;
    for (const auto &[l, r] : sorted_query) {
        while (lcurr < l) { shrink_left(lcurr++, rcurr); }
        while (lcurr > l) { expand_left(lcurr--, rcurr); }
        while (rcurr < r) { expand_right(lcurr, rcurr++); }
        while (rcurr > r) { shrink_right(lcurr, rcurr--); }
        sol[pair(l, r)] = solve();
    }

    vector<Solution> ans; ans.reserve(query_list.size());
    for (const auto &lr : query_list) {
        ans.emplace_back(sol[lr]);
    }
    return ans;

}
