// #REQ: base_template.hpp array/cumsum.hpp
template <typename T, typename Op>
struct bicumsum {
    Op op;
    T id;
    cumsum<T, Op> forward_cs;
    cumsum<T, Op> backward_cs;
    bicumsum() = default;
    bicumsum(const vector<T> &a, Op op_, T id_):
        op(op_), id(id_),
        forward_cs(a, op, id),
        backward_cs(vector<T>(a.rbegin(), a.rend()), op, id)
    {}
    // sum of a[i] for i in [0, i)
    T forward(size_t i) const { return forward_cs.sum(i); }
    // sum of a[i] for i in [i, n)
    T backward(size_t i) const { return backward_cs.sum(backward_cs.s.size()-1-i); }
    // sum of a[i] for i in [0, n) \\ {i}
    T except(size_t i) const { return op(forward(i), backward(i+1)); }
};