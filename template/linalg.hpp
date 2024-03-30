#pragma once
#include "base_template.hpp"
template <typename T>
struct matrix {
    size_t rows, cols;
    vector<T> values;
    matrix(): rows(1), cols(1), values(1, (T)T_ADD_IDENTITY) {}
    matrix(size_t rows_ = 1, size_t cols_ = 1, T default_value = T_ADD_IDENTITY):
        rows(rows_), cols(cols_), values(rows * cols, default_value) {}
    template <typename U>
    matrix(vector<U> &v): rows(v.size()), cols(1), values(rows) { REP(i, rows) at(i) = v[i]; }
    template <typename U>
    matrix(initializer_list<U> init) : rows(init.size()), cols(1), values(init.begin(), init.end()) {}
    template <typename U>
    matrix(initializer_list<initializer_list<U>> init) : rows(init.size()), cols(init.begin()->size()), values(rows * cols, (T)T_ADD_IDENTITY) { size_t i = 0, j = 0; for (const auto &initi : init) { for (const auto &initij : initi) { at(i, j) = initij; j++; } i++; j = 0; } }
    T& at(size_t i = 0, size_t j = 0) { return values.at(i * cols + j); }
    const T& at(size_t i = 0, size_t j = 0) const { return values.at(i * cols + j); }
    pair<size_t, size_t> size() const { return {rows, cols}; }
    // 用途に合わせて書き換える箇所 はじめ
    constexpr static T T_ADD_IDENTITY = 0;
    constexpr static T T_MUL_IDENTITY = 1;
    static T add(T a, T b) { return a + b; } // 半環
    static T mul(T a, T b) { return a * b; } // 半環
    static T sub(T a, T b) { return a - b; } // 環として使う場合
    static T div(T a, T b) { return a / b; } // スカラーが体として使う場合
    // 用途に合わせて書き換える箇所 おわり
    matrix<T> operator+ (const matrix<T> &t) const { assert(size() == t.size()); matrix<T> r = *this; REP(i, rows) REP(j, cols) r.at(i, j) = add(r.at(i, j), t.at(i, j)); return r; }
    matrix<T> operator- (const matrix<T> &t) const { assert(size() == t.size()); matrix<T> r = *this; REP(i, rows) REP(j, cols) r.at(i, j) = sub(r.at(i, j), t.at(i, j)); return r; }
    matrix<T> operator* (const matrix<T> &t) const { assert(cols == t.rows); matrix<T> r(rows, t.cols, (T)T_ADD_IDENTITY); REP(i, rows) REP(k, cols) REP(j, t.cols) r.at(i, j) = add(r.at(i, j), mul(at(i, k), t.at(k, j))); return r; }
    template <typename S>
    matrix<T> operator* (const S &t) const { matrix<T> r = *this; REP(i, rows) REP(j, cols) at(i, j) = mul(at(i, j), t); return r; }
    template <typename S>
    matrix<T> operator/ (const S &t) const { matrix<T> r = *this; REP(i, rows) REP(j, cols) at(i, j) = div(at(i, j), t); return r; }
    matrix<T>& operator+= (const matrix<T> &t) { assert(size() == t.size()); REP(i, rows) REP(j, cols) at(i, j) = add(at(i, j), t.at(i, j)); return *this; }
    matrix<T>& operator-= (const matrix<T> &t) { assert(size() == t.size()); REP(i, rows) REP(j, cols) at(i, j) = sub(at(i, j), t.at(i, j)); return *this; }
    matrix<T>& operator*= (const matrix<T> &t) { return *this = *this * t; }
    template <typename S>
    matrix<T> operator*= (const S &t) { REP(i, rows) REP(j, cols) at(i, j) = mul(at(i, j), t); return *this; }
    template <typename S>
    matrix<T> operator/= (const S &t) { REP(i, rows) REP(j, cols) at(i, j) = div(at(i, j), t); return *this; }
    static matrix<T> eye(size_t n) { matrix<T> r(n, n); REP(i, n) r.at(i, i) = T_MUL_IDENTITY; return r; }
    matrix<T> pow(ull n) const { assert(rows == cols); matrix<T> m = eye(rows), b = *this; while (n) { if (n & 1) m *= b; n >>= 1; b *= b; } return m; }
    matrix<T> transpose() const { matrix<T> r(cols, rows); REP(i, rows) REP(j, cols) r.at(j, i) = at(i, j); return r; }
};
