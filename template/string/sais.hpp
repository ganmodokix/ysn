// #REQ: base_template.hpp bisection.hpp

// SA-IS による Suffix Array 構築 O(N)
// 参考: https://blog.knshnb.com/posts/sa-is/
// verify: https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_D
// verify: https://atcoder.jp/contests/abc272/tasks/abc272_f
struct suffix_array_sais {

    const string s;
    const ll n;
    vector<ll> sa;

    suffix_array_sais() = delete;
    suffix_array_sais(const string &s_): s(s_), n(s.size()), sa() {

        // [s_1, s_2, ..., s_-1, -∞]
        vector<ll> sll(n + 1);
        REP(i, n) {
            sll[i] = (ll)s[i] - numeric_limits<char>::min() + 1;
        }
        sa = sa_is(sll, numeric_limits<char>::max() - numeric_limits<char>::min() + 2);

    }

    vector<ll> sa_is(const vector<ll> &s, ll k) {

        const ll n = s.size();

        // L/S 判定
        vector iss(n, false); iss.back() = true;
        DSRNG(i, n-2, 0) {
            iss[i] = s[i] < s[i+1] || (s[i] == s[i+1] && iss[i+1]);
        }

        // LMS判定
        vector islms(n, false);
        vector<ll> lmss;
        REP(i, n) {
            if (iss[i] && (!i || !iss[i-1])) {
                islms[i] = true;
                lmss.emplace_back(i);
            }
        }

        // first path
        vector<ll> psa = induced_sort(s, lmss, iss, k);

        // lmsをソート
        vector<ll> lms_sorted; lms_sorted.reserve(lmss.size());
        for (const auto &i : psa) {
            if (islms[i]) {
                lms_sorted.emplace_back(i);
            }
        }

        // 隣接LMSを比較
        psa[lms_sorted.front()] = 0;
        ll rank = 0;
        if (lms_sorted.size() > 1) { psa[lms_sorted[1]] = ++rank; }
        RANGE(i, 1, (ll)lms_sorted.size() - 2) {
            bool diff = false;
            REP(j, n) {
                ll p = lms_sorted[i] + j;
                ll q = lms_sorted[i+1] + j;
                if (s[p] != s[q] || islms[p] != islms[q]) {
                    diff = true;
                    break;
                }
                if (j > 0 && islms[p]) {
                    assert(islms[q]);
                    break;
                }
            }
            psa[lms_sorted[i+1]] = rank += diff;
        }
        // LMS-substring を文字と見做す
        vector<ll> s2; s2.reserve(lmss.size());
        REP(i, n) {
            if (islms[i]) {
                s2.emplace_back(psa[i]);
            }
        }
        // 再帰
        vector<ll> sa_lms;
        if (rank + 1 == (ll)lmss.size()) {
            sa_lms = lms_sorted;
        } else {
            sa_lms = sa_is(s2, rank + 1);
            for (auto &x : sa_lms) x = lmss[x];
        }
        // second path
        return induced_sort(s, sa_lms, iss, k);
    }

    vector<ll> induced_sort(const vector<ll> &s, const vector<ll> &lmss, const vector<bool> &iss, ll k) {

        const ll n = s.size();

        // バケツソート
        vector<ll> bucket(n);
        vector<ll> ldx(k + 1);
        REP(i, n) {
            ll si = s[i];
            ldx[si + 1]++;
        }
        REP(i, k) ldx[i+1] += ldx[i];
        vector<ll> rdx(ldx.begin() + 1, ldx.end());
        auto rrdx = rdx;
        ldx.pop_back();

        // LMSを後ろから詰める
        DSRNG(i, (ll)lmss.size() - 1, 0) {
            ll si = s[lmss[i]];
            bucket[--rdx[si]] = lmss[i];
        }

        // 昇順に見て、入れたものの1つ前がLの場合前から詰める
        REP(i, n) {
            ll j = bucket[i] - 1;
            if (j < 0 || iss[j]) continue;
            ll sj = s[j];
            bucket[ldx[sj]++] = j;
        }

        // 降順に観て、入れたものの前がSの場合後ろから詰める (このときもう一回lmsを詰め直す)
        rdx = rrdx;
        DSRNG(i, n-1, 0) {
            ll j = bucket[i] - 1;
            if (j < 0 || !iss[j]) continue;
            ll sj = s[j];
            bucket[--rdx[sj]] = j;
        }

        return bucket;

    }

    // T が S の部分文字列として含まれるかを判定 O(|T| log |S|)
    bool contains(const string &t) const {
        ll lb = bisection(0LL, n, [&](ll i) { return s.substr(sa[i], t.size()) >= t; });
        return s.substr(sa[lb], t.size()) == t;
    }

    // Longest Common Prefix (LCP) 配列の構築 O(N)
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