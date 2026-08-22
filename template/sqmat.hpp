#pragma once
#include "base_template.hpp"

// 固定サイズ行列（行優先）
template <
    typename T, // value type
    size_t r, size_t c = r, // matrix size
    T add_id = T{0}, // additive identity
    T mul_id = T{1} // multiplicative identity
>
struct array_matrix {
    using value_type = T;

    static constexpr size_t rows = r;
    static constexpr size_t cols = c;
    static constexpr T additive_identity = add_id;
    static constexpr T multiplicative_identity = mul_id;

    array<T, r * c> a;

    constexpr array_matrix() { a.fill(add_id); }

    constexpr explicit array_matrix(const array<T, r * c>& values) : a(values) {}

    constexpr array_matrix(initializer_list<T> values)
        requires (c != 1)
    {
        assert(values.size() == r * c);
        copy(values.begin(), values.end(), a.begin());
    }

    constexpr array_matrix(initializer_list<initializer_list<T>> values) {
        assert(values.size() == r);
        size_t i = 0;
        for (const auto& row : values) {
            assert(row.size() == c);
            copy(row.begin(), row.end(), a.begin() + i * c);
            ++i;
        }
    }

    constexpr T& operator()(const size_t i, const size_t j) { return a[i * c + j]; }
    constexpr const T& operator()(const size_t i, const size_t j) const { return a[i * c + j]; }

    constexpr T& at(const size_t i, const size_t j) { return a.at(i * c + j); }
    constexpr const T& at(const size_t i, const size_t j) const { return a.at(i * c + j); }

    constexpr span<T, c> operator[](const size_t i) { return span<T, c>(a.data() + i * c, c); }
    constexpr span<const T, c> operator[](const size_t i) const { return span<const T, c>(a.data() + i * c, c); }

    constexpr auto begin() { return a.begin(); }
    constexpr auto begin() const { return a.begin(); }
    constexpr auto end() { return a.end(); }
    constexpr auto end() const { return a.end(); }

    constexpr array_matrix& operator+=(const array_matrix& rhs) {
        for (size_t i = 0; i < r * c; ++i) a[i] += rhs.a[i];
        return *this;
    }

    constexpr array_matrix& operator-=(const array_matrix& rhs) {
        for (size_t i = 0; i < r * c; ++i) a[i] -= rhs.a[i];
        return *this;
    }

    constexpr array_matrix operator+(const array_matrix& rhs) const {
        auto result = *this;
        return result += rhs;
    }

    constexpr array_matrix operator-(const array_matrix& rhs) const {
        auto result = *this;
        return result -= rhs;
    }

    template <size_t d>
    constexpr array_matrix<T, r, d, add_id, mul_id>
    operator*(const array_matrix<T, c, d, add_id, mul_id>& rhs) const {
        array_matrix<T, r, d, add_id, mul_id> result;
        for (size_t i = 0; i < r; ++i) {
            for (size_t k = 0; k < c; ++k) {
                for (size_t j = 0; j < d; ++j) {
                    result(i, j) += (*this)(i, k) * rhs(k, j);
                }
            }
        }
        return result;
    }

    constexpr array_matrix& operator*=(const array_matrix& rhs)
        requires (r == c)
    {
        return *this = *this * rhs;
    }

    template <typename S>
    constexpr array_matrix& operator*=(const S& scalar) {
        for (auto& value : a) value *= scalar;
        return *this;
    }

    template <typename S>
    constexpr array_matrix& operator/=(const S& scalar) {
        for (auto& value : a) value /= scalar;
        return *this;
    }

    template <typename S>
    constexpr array_matrix operator*(const S& scalar) const {
        auto result = *this;
        return result *= scalar;
    }

    template <typename S>
    constexpr array_matrix operator/(const S& scalar) const {
        auto result = *this;
        return result /= scalar;
    }

    static constexpr array_matrix identity()
        requires (r == c)
    {
        array_matrix result;
        for (size_t i = 0; i < r; ++i) result(i, i) = mul_id;
        return result;
    }

    static constexpr array_matrix eye()
        requires (r == c)
    {
        return identity();
    }

    constexpr array_matrix pow(uint64_t n) const
        requires (r == c)
    {
        auto result = identity();
        auto base = *this;
        while (n != 0) {
            if (n & 1) result *= base;
            base *= base;
            n >>= 1;
        }
        return result;
    }

    constexpr array_matrix<T, c, r, add_id, mul_id> transpose() const {
        array_matrix<T, c, r, add_id, mul_id> result;
        for (size_t i = 0; i < r; ++i) {
            for (size_t j = 0; j < c; ++j) result(j, i) = (*this)(i, j);
        }
        return result;
    }

    constexpr bool operator==(const array_matrix&) const = default;
};

template <typename S, typename T, size_t r, size_t c, T add_id, T mul_id>
constexpr array_matrix<T, r, c, add_id, mul_id>
operator*(const S& scalar, const array_matrix<T, r, c, add_id, mul_id>& matrix) {
    return matrix * scalar;
}
