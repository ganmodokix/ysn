#pragma once
#include "base_template.hpp"
// {Shink,Expand}{Left,Right} の引数は操作前の区間 (Index l, Index r)
// 半開区間で扱う [left, right). 最初は [initial_l, initial_r) から始まる
template <typename Solution, // コピーたくさんしても大丈夫な軽いもの，重いコンテクストとかはラムダ式のキャプチャで
    typename Index, // 添字 (クエリ範囲)
    typename ShrinkLeft,
    typename ShrinkRight,
    typename ExpandLeft,
    typename ExpandRight,
    typename CalculateSolution>
vector<Solution> mo_s_algorithm(
    const vector<pair<Index, Index>> &queries,
    ShrinkLeft shrink_left, // [l,r) -> [l+1, r)
    ShrinkRight shrink_right, // [l,r) -> [l, r-1)
    ExpandLeft expand_left, // [l,r) -> [l-1, r)
    ExpandRight expand_right, // [l,r) -> [l, r+1)
    CalculateSolution solve, // f: void -> Solution
    Index initial_l = 0,
    Index initial_r = 0
) {
    
    Index query_min = numeric_limits<Index>::max();
    Index query_max = numeric_limits<Index>::min();
    for (const auto &[x, y] : queries) {
        chmin(query_min, x);
        chmax(query_max, x);
        chmin(query_min, y);
        chmax(query_max, y);
    }
    
    const ll query_range = (ll)query_max - (ll)query_min;
    const ll num_bucket = max(1LL, (ll)sqrtl(queries.size()));
    const ll block_size = max(1LL, query_range / num_bucket);
    auto block = [&](Index i) { return ((ll)i - (ll)query_min) / block_size; };

    vector<pair<Index, Index>> sorted_query = queries;
    UNIQUE(sorted_query);
    sort(ALL(sorted_query), [&](pair<Index, Index> a, pair<Index, Index> b) {
        ll block_a = block(a.first);
        ll block_b = block(b.first);
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

    vector<Solution> ans; ans.reserve(queries.size());
    for (const auto &lr : queries) {
        ans.emplace_back(sol[lr]);
    }
    return ans;

}
