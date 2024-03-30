// #REQ: base_template.hpp
// 線形篩 O(N)
// lp[i] := i の最小素因数 (2 <= i < n)
// pr: n 未満の素数リスト
pair<vector<ll>, vector<ll>> sieve_under(size_t n) {
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
    return {lp, pr};
}