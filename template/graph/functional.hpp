#pragma once
#include "base_template.hpp"

template <typename T, typename ValueOp = plus<>>
struct functional_graph {
    using pos_type = size_t;
    using value_type = T;
    vector<pos_type> f = {};
    vector<value_type> v = {};
    ValueOp value_op = {};
    functional_graph() = default;
    functional_graph(size_t n): f(n), v(n) {
        iota(ALL(f), 0);
    }
    
    static functional_graph compose(
        const functional_graph<T, ValueOp>& a,
        const functional_graph<T, ValueOp>& b
    ) {
        const auto n = a.f.size();
        
    }
};