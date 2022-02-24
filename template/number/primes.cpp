// #REQ: base_template
// 線形篩を用いた素数列挙 O(N)
vector<ll> primes_under(size_t n) {
    vector<ll> lp(n, 0);
    vector<ll> pr;
    for (size_t i = 2; i < n; i++) {
        if (!lp[i]) {
            lp[i] = i;
            pr.push_back(i);
        }
        for (size_t j = 0; j < pr.size() && pr[j] <= lp[i] && i * pr[j] < n; j++) {
            lp[i * pr[j]] = pr[j];
        }
    }
    return pr;
}