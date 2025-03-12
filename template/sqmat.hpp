#pragma once
#include "base_template.hpp"

// 固定サイズ行列
template <
    typename T, // value type
    size_t r, size_t c = r, // matrix size
    T add_id = T{0}, // additive identity
    T mul_id = T{1} // multiplicative identity
>
struct array_matrix {
    array<T, r * c> a;
    

}