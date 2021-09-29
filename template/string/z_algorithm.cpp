// Z-algorithm
template <class RandomAccessIterator>
vector<ll> common_prefix(RandomAccessIterator first, RandomAccessIterator last) {
    const ll sz = last - first;
    vector<ll> prefix(sz, 0);
    for (ll i = 1, j = 0; i < sz; i++) {
        if (i + prefix[i-j] < j + prefix[j]) {
            prefix[i] = prefix[i-j];
        } else {
            ll k = max(0LL, j + prefix[j] - i);
            while (i + k < sz && *(first + k) == *(first + (i+k))) k++;
            prefix[i] = k;
            j = i;
        }
    }
    prefix[0] = sz;
    return prefix;
}