// #REQ: base_template.hpp
// (Z/2Z)^hoge上のXOR基底, raw_valueが真の場合aから選んで線形独立なベクトルを返す
// https://twitter.com/noshi91/status/1200702280128856064
template <typename T>
vector<T> xor_basis(const vector<T> &a, bool raw_value = false) {
    // noshi基底 O(N)
    vector<T> result, bases;
    for (T d : a) {
        T e = d;
        for (T b : bases) e = min(e, e ^ b);
        if (e) {
            bases.push_back(e);
            result.push_back(raw_value ? d : e);
        }
    }
    return result;
}