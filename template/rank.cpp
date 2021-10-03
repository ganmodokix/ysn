template <typename T>
vector<size_t> rank_of(const vector<T> &arr) {
    vector<size_t> rank(arr.size());
    iota(ALLOF(rank), 0LL);
    sort(ALLOF(rank), [&](ll i, ll j) { return arr[i] < arr[j]; });
}