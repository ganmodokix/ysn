#include "base_template.hpp"

// 区間をsetで管理するやつ
// 区間sの和集合を最小個数の区間で表す、つまり閉包に共通部分のない区間の集合で表す
// e.g., [3,5) ∪ [5,7) -> [3,7)
template <typename T = ll>
requires ::std::integral<T>
struct segment_union {

    using value_type = T;
    using segment_type = pair<value_type, value_type>;
    using container_type = set<segment_type>;
    
    set<segment_type> s = {};

    segment_union() = default;

    template<ranges::input_range R>
    segment_union(R&& range) {
        for (const auto& seg: range) {
            add(seg);
        }
    }

    // x と y が共通部分を持つ
    // as_closure: {l, r} を終点 r を含む [l, r] として扱う
    constexpr static bool overlaps(
        const segment_type& x,
        const segment_type& y,
        const bool as_closure = false
    ) {
        if (as_closure) {
            return x.second >= y.first && y.second >= x.first;
        } else {
            return x.second > y.first && y.second > x.first;
        }
    }

    // 点 x が含まれる区間のイテレータを返す、ない場合 nullopt
    // as_closure: 終点 r を含む [l, r] として扱う
    auto segment_including(const value_type x, const bool as_closure = false) const
        -> optional<typename container_type::const_iterator>
    {
        if (s.empty()) return nullopt;
        
        constexpr auto NINF = numeric_limits<value_type>::lowest();
        auto it = s.lower_bound({x + 1, NINF});
        if (it == s.begin()) return nullopt;

        const auto pit = ranges::prev(it);
        if (pit->first <= x && (as_closure ? x <= pit->second : x < pit->second)) {
            return pit;
        }

        return nullopt;
    }

    bool contains(const value_type x, const bool as_closure = false) const {
        return segment_including(x, as_closure).has_value();
    }

    // 区間 [l, r) を追加; S <- S ∪ [l, r)
    // 区間数に対してならし対数時間
    // 削除した区間を返す
    vector<segment_type> insert(const value_type l, const value_type r) {

        auto added_segment = pair{l, r};
        auto removed_segments = vector<segment_type>{};

        // 左につなげる
        if (
            const auto ito = segment_including(added_segment.first, true);
            ito.has_value()
        ){
            const auto& it = *ito;
            chmin(added_segment.first, it->first);
            chmax(added_segment.second, it->second);
            removed_segments.emplace_back(*it);
            s.erase(it);
        }
        
        // 右につなげる
        const auto NINF = numeric_limits<value_type>::lowest();
        while (true) {
            auto it = s.lower_bound(pair{added_segment.first, NINF});
            if (it == s.end()) break;
            if (added_segment.second < it->first) break;
            chmin(added_segment.first, it->first);
            chmax(added_segment.second, it->second);
            removed_segments.emplace_back(*it);
            s.erase(it);
        }

        s.emplace(move(added_segment));

        return removed_segments;
    }

    // 区間 [l, r) を意味する seg = {l, r} を追加; S <- S ∪ [l, r)
    // 区間数に対してならし対数時間
    template <typename Seg>
    void insert(const Seg& seg) {
        const auto& [l, r] = seg;
        insert(l, r);
    }

    // 区間 [l, r) を除去; S <- S \ [l, r)
    // 区間数に対してならし対数時間
    void erase(const value_type l, const value_type r) {
        
        auto replaced_segments = vector<segment_type>{};
        const auto seg = segment_type{l, r};

        // 左
        if (
            const auto oit = segment_including(l, true);
            oit.has_value()
        ) {
            replaced_segments.emplace_back(**oit);
            s.erase(*oit);
        }

        // 右
        constexpr auto NINF = numeric_limits<value_type>::lowest();
        auto itl = s.lower_bound(pair{l, NINF});
        auto itr = itl;
        for ( ; itr != s.end() && overlaps(*itr, seg); ++itr) {
            replaced_segments.emplace_back(*itr);
        }
        s.erase(itl, itr);

        // 除去して {l,r} を除いた区間に変換
        for (const auto& [x, y] : replaced_segments) {
            if (x < l) s.emplace(x, l);
            if (r < y) s.emplace(r, y);
        }
        
    }

    // 区間 [l, r) を意味する seg = {l, r} を除去; S <- S \ [l, r)
    // 区間数に対してならし対数時間
    template <typename Seg>
    void erase(const Seg& seg) {
        const auto& [l, r] = seg;
        erase(l, r);
    }
    
    // origin以上のmexを求める
    // 区間数に対して対数時間
    value_type mex(const value_type origin = 0) const {
        auto origin_seg = segment_including(origin);
        if (!origin_seg.has_value()) {
            return origin;
        }
        return origin_seg.value()->second;
    }

};