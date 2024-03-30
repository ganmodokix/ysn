#pragma once
#include "base_template.hpp"
// Manacher's Algorithm: {s[i] = iを中心にした最長の回文の半径} を返す．線形時間．
template<typename RandomAccessIterator>
vector<size_t> manacher(RandomAccessIterator first, RandomAccessIterator last) {
    size_t i = 0, j = 0;
    assert(last - first >= 0);
    size_t n = last - first;
    vector<size_t> r(n, 0);
    while (i < n) {
        while (i >= j && i + j < n && *(first + i-j) == *(first + i+j)) j++;
        r[i] = j;
        size_t k = 1;
        while (i >= k && k + r[i-k] < j) r[i+k] = r[i-k], k++;
        i += k;
        j -= k;
    }
    return r;
}
