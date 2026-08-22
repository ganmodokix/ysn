#include "base_template.hpp"

// 基数ヒープ https://dl.acm.org/doi/10.1145/77600.77615
// 最後にpopしたindexより小さいものをpushした場合UB
// empty() のとき pop() してもUB
template <integral K, move_constructible V>
requires (
    !same_as<remove_cv_t<K>, bool> &&
    numeric_limits<K>::digits <= numeric_limits<uint64_t>::digits
)
struct radix_heap {
    using index_type = uint64_t;
    using key_type = K;
    using value_type = V;

    index_type size_ = {};
    index_type last_ = {};
    array<vector<pair<index_type, value_type>>, 65> varr = {};

    index_type size() const { return size_; }
    bool empty() const { return size() == 0; }

    constexpr static index_type find_bucket(const index_type index, const index_type last) {
        return bit_width(index ^ last);
    }

    constexpr static index_type encode_key(const key_type key) {
        if constexpr (is_unsigned_v<key_type>) {
            return key;
        } else {
            constexpr auto bit_length = std::numeric_limits<index_type>::digits;
            return static_cast<index_type>(key) ^ (index_type{1} << (bit_length - 1));
        }
    }

    constexpr static key_type decode_key(const index_type key) {
        if constexpr (is_unsigned_v<key_type>) {
            return key;
        } else {
            constexpr auto bit_length = std::numeric_limits<index_type>::digits;
            return static_cast<key_type>(key ^ (index_type{1} << (bit_length - 1)));
        }
    }

    template <typename T>
    requires constructible_from<value_type, T>
    void push(const key_type key, T&& value) {
        ++size_;
        const auto index = encode_key(key);
        varr[find_bucket(index, last_)].emplace_back(index, value_type(forward<T>(value)));
    }

    template <typename... Args>
    requires constructible_from<value_type, Args...>
    void emplace(const key_type key, Args&&... args) {
        ++size_;
        const auto index = encode_key(key);
        varr[find_bucket(index, last_)].emplace_back(index, value_type(forward<Args>(args)...));
    }

    void pull() {
        if (!varr.front().empty()) return;

        auto idx = index_type{1};
        while (varr[idx].empty()) ++idx;
        last_ = min_element(ALL(varr[idx]))->first;
        
        for (auto&& p : varr[idx]) {
            const auto dst = find_bucket(p.first, last_);
            varr[dst].emplace_back(move(p));
        }
        varr[idx].clear();
    }

    pair<key_type, value_type> pop() {
        pull();
        --size_;
        auto index_value = move(varr[0].back());
        varr[0].pop_back();
        return {decode_key(index_value.first), move(index_value.second)};
    }

    pair<key_type, value_type> top()
    requires copy_constructible<value_type>
    {
        pull();
        const auto& index_value = varr[0].back();
        return {decode_key(index_value.first), index_value.second};
    }
};
