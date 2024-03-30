// #REQ: base_template
template <typename T>
vector<size_t> argsort(const vector<T> &arr) {
    vector<size_t> rank(arr.size());
    iota(ALL(rank), 0);
    sort(ALL(rank), [&](auto i, auto j) {
        return arr[i] < arr[j];
    });
    return rank;
}