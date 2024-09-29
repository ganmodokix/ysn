#pragma once
#include "base_template.hpp"

// メモリ2倍の代わりに O(1) で fill(ALL(array), x) 可能な配列
template<typename T>
struct lazy_fill_vector {
    using time_type = uint64_t;
    // last_modified[i] == now_time iff. value[i] は valid
    time_type now_time = 1;
    T default_value = {};
    mutable vector<time_type> last_modified = {};
    vector<T> value = {};

    lazy_fill_vector() = default;
    lazy_fill_vector(size_t n_, const T fill_value_ = {}):
        default_value(fill_value_),
        last_modified(n_, 0),
        value(n_, fill_value_)
    {
    }

    bool valid(const size_t index) const {
        return last_modified[index] == now_time;
    }

    template <typename MaybeConstSelf>
    static auto& _accessor(MaybeConstSelf& self, const size_t index) {
        if (!self.valid(index)) {
            self.last_modified[index] = self.now_time;
            self.value[index] = self.default_value;
        }
        return self.value[index];
    }

    template <typename MaybeConstSelf>
    static auto& _index_checking_accessor(MaybeConstSelf& self, const size_t index) {
        if (index >= size()) {
            throw invalid_argument("Index out of range");
        }
        return _accessor(self, index);
    }

    // 要素アクセス O(1)
    T& operator[](const size_t index) { return _accessor(*this, index); }
    const T& operator[](const size_t index) const { return _accessor(*this, index); }
    T& at(const size_t index) { return _index_checking_accessor(*this, index); }
    const T& at(const size_t index) const { return _index_checking_accessor(*this, index); }

    // すべての要素を value にする O(1)
    template<typename TT>
    void fill(TT&& value) {
        default_value = forward<TT>(value);
        ++now_time;
    }

    // 末尾に追加
    template <typename TT>
    void push_back(TT&& x) {
        value.push_back(forward<TT>(x));
        last_modified.push_back(now_time);
    }
    template <typename... Args>
    void emplace_back(Args&&... args) {
        value.emplace_back(forward<Args>(args)...);
        last_modified.push_back(now_time);
    }

    // 末尾を削除
    void pop_back() {
        value.pop_back();
        last_modified.pop_back();
    }

    // サイズ
    size_t size() const noexcept {
        return value.size();
    }
};
