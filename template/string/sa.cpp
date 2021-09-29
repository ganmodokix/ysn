// ダブリングによる O(Nlog^2N) での Suffix Array 構築
// Validated by https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_D&lang=jp
struct suffix_array {

    const string s;
    const ll n;
    vector<ll> sa;

    suffix_array() = delete;
    suffix_array(const string &s_): s(s_), n(s.size()), sa(n+1, 0) {
        
        vector rank(n+1, 0LL);

        // prefix 1 文字のみでソート
        iota(ALLOF(sa), 0LL);
        REP(i, n+1) {
            rank[i] = s[i];
        }

        // k文字ソートされていて、2k文字でソートしてダブリング
        const auto rank_sa = [&](ll i, ll k) -> pair<ll, ll> {
            return {rank[i], i+k < n+1 ? rank[i+k] : -LINF};
        };
        for (ll k = 1; k <= n; k *= 2) {
            const auto compare_sa = [&](ll i, ll j) { return rank_sa(i, k) < rank_sa(j, k); };
            sort(ALLOF(sa), compare_sa);
            vector tmp(n+1, 0LL);
            RANGE(i, 1, n) {
                tmp[sa[i]] = tmp[sa[i-1]] + compare_sa(sa[i-1], sa[i]);
            }
            tmp.swap(rank);
        }
        
    }

    // O(|T| log |S|) で T が S の部分文字列として含まれるかを判定
    bool contains(const string &t) const {
        ll a = 0, b = n;
        while (abs(a - b) > 1) {
            ll c = (a + b) / 2;
            (s.substr(sa[c], t.size()) < t ? a : b) = c;
        }
        return s.substr(sa[b], t.size()) == t;
    }

    // O(N) での Longest Common Prefix (LCP) 配列の構築
    // lcp[i] := |s[sa[i]:] と s[sa[i+1]:] の longest commmon prefix|
    // Verified by https://atcoder.jp/contests/abc213/tasks/abc213_f
    vector<ll> lcp() const {
        
        vector rank(n+1, 0LL);
        REP(i, n+1) rank[sa[i]] = i;

        ll h = 0;
        vector result(n, 0LL);
        REP(i, n) {
            ll j = sa[rank[i] - 1];
            if (h > 0) h--;
            for (; j+h < n && i+h < n; h++) {
                if (s[j+h] != s[i+h]) break;
            }
            result[rank[i] - 1] = h;
        }

        return result;

    }

};