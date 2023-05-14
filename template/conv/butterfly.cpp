// #REQ: base_template modint_petit number/ceillog2
// 2段ずつのバタフライ演算
// 参考実装: ACL https://github.com/atcoder/ac-library/blob/master/atcoder/convolution.hpp

// (pdiv, prim) ごとに前計算可能なもの
template <ll pdiv, ll prim>
struct butterfly_proprocess {
    
    static constexpr ll rank2 = __builtin_ctz(pdiv - 1);
    array<ll, rank2 + 1> root;
    array<ll, rank2 + 1> iroot;
    array<ll, max(0LL, rank2 - 1)> rate2;
    array<ll, max(0LL, rank2 - 1)> irate2;
    array<ll, max(0LL, rank2 - 2)> rate3;
    array<ll, max(0LL, rank2 - 2)> irate3;

    butterfly_proprocess() {
        root[rank2] = modpow(prim, (pdiv - 1) >> rank2, pdiv);
        iroot[rank2] = modinv(root[rank2], pdiv);
        DSRNG(i, rank2 - 1, 0) {
            root[i] = root[i + 1] * root[i + 1] % pdiv;
            iroot[i] = iroot[i + 1] * iroot[i + 1] % pdiv;
        }
        {
            ll prod = 1, iprod = 1;
            REP(i, rank2 - 1) {
                rate2[i] = root[i + 2] * prod % pdiv;
                irate2[i] = iroot[i + 2] * iprod % pdiv;
                (prod *= iroot[i + 2]) %= pdiv;
                (iprod *= root[i + 2]) %= pdiv;
            }
        }
        {
            ll prod = 1, iprod = 1;
            REP(i, rank2 - 2) {
                rate3[i] = root[i + 3] * prod % pdiv;
                irate3[i] = iroot[i + 3] * iprod % pdiv;
                (prod *= iroot[i + 3]) %= pdiv;
                (iprod *= root[i + 3]) %= pdiv;
            }
        }
    }

    static butterfly_proprocess<pdiv, prim>& singleton() {
        static butterfly_proprocess<pdiv, prim> prep;
        return prep;
    }

};

// ACL実装のバタフライ演算
template <ll pdiv = 998244353, ll prim = 3>
vector<ll> butterfly(vector<ll>&& a_) {
    assert(!a_.empty() && popcountll(a_.size()) == 1);

    vector<ll> a = move(a_);
    
    ll n = a.size();
    ll h = ceillog2(n);

    static auto prep = butterfly_proprocess<pdiv, prim>::singleton();

    // 飛ばし飛ばしのバタフライ演算で定数倍を改善
    for (ll len = 0; len < h; ) {
        if (len + 1 == h) {
            ll p = 1LL << (h - len - 1);
            ll rot = 1;
            REP(s, 1 << len) {
                ll offset = s << (h - len);
                REP(i, p) {
                    auto &al = a[i + offset];
                    auto &ar = a[i + offset + p];
                    ll l = al;
                    ll r = ar * rot % pdiv;
                    al = l + r; if (al >= pdiv) al -= pdiv;
                    ar = l + pdiv - r; if (ar >= pdiv) ar -= pdiv;
                }
                if (s + 1 != s_len) {
                    (rot *= prep.rate2[__builtin_ctzll(~(ull)s)]) %= pdiv;
                }
            }
            len++;
        } else {
            // 4-base
            ll p = 1 << (h - len - 2);
            ll rot = 1, imag = prep.root[2];
            REP(s, 1 << len) {
                ll rot2 = rot * rot % pdiv;
                ll rot3 = rot2 * rot % pdiv;
                ll offset = s << (h - len);
                REP(i, p) {
                    ull pdiv2 = (ull)pdiv * (ull)pdiv;
                    ull a0 = a[i + offset        ];
                    ull a1 = a[i + offset + p    ] * rot;
                    ull a2 = a[i + offset + p * 2] * rot2;
                    ull a3 = a[i + offset + p * 3] * rot3;
                    ull a1na3 = (a1 + pdiv2 - a3) % pdiv;
                    ull a1na3imag = a1na3 * (ull)imag % pdiv;
                    ull na2 = pdiv2 - a2;
                    (a[i + offset        ] = a0 + a2 + a1 + a3) %= pdiv;
                    (a[i + offset + p    ] = a0 + a2 + (pdiv2 * 2 - (a1 + a3))) %= pdiv;
                    (a[i + offset + p * 2] = a0 + na2 + a1na3imag) %= pdiv;
                    (a[i + offset + p * 3] = a0 + na2 + (pdiv2 - a1na3imag)) %= pdiv;
                }
                if (s + 1 != s_len) {
                    (rot *= prep.rate3[__builtin_ctzll(~(ull)s)]) %= pdiv;
                }
            }
            len += 2;
        }
    }

    return a;
}

template <ll pdiv = 998244353, ll prim = 3>
vector<ll> butterfly(const vector<ll>& a) {
    return butterfly<pdiv, prim>(vector<ll>(a));
}

// ACL実装の逆バタフライ演算
template <ll pdiv = 998244353, ll prim = 3>
vector<ll> butterfly_inv(vector<ll>&& a_) {
    assert(!a_.empty() && popcountll(a_.size()) == 1);

    vector<ll> a = move(a_);
    
    ll n = a.size();
    ll h = ceillog2(n);

    static const auto prep = butterfly_proprocess<pdiv, prim>::singleton();

    // 飛ばし飛ばしのバタフライ演算で定数倍を改善
    for (ll len = h; len; ) {
        if (len == 1) {
            ll p = 1 << (h - len);
            ll irot = 1;
            REP(s, 1 << (len - 1)) {
                ll offset = s << (h - len + 1);
                REP(i, p) {
                    auto &al = a[i + offset];
                    auto &ar = a[i + offset + p];
                    ll l = al;
                    ll r = ar;
                    al = l + r; if (al >= pdiv) al -= pdiv;
                    ar = l + pdiv - r; if (ar >= pdiv) ar -= pdiv;
                    (ar *= irot) %= pdiv;
                }
                if (s + 1 != s_len) {
                    (irot *= prep.irate2[__builtin_ctzll(~(ull)(s))]) %= pdiv;
                }
            }
            len--;
        } else {
            // 4-base
            ll p = 1 << (h - len);
            ll irot = 1, iimag = prep.iroot[2];
            REP(s, 1 << (len - 2)) {
                ll irot2 = irot * irot % pdiv;
                ll irot3 = irot2 * irot % pdiv;
                ll offset = s << (h - len + 2);
                REP(i, p) {
                    ull a0 = a[i + offset        ];
                    ull a1 = a[i + offset + p    ];
                    ull a2 = a[i + offset + p * 2];
                    ull a3 = a[i + offset + p * 3];
                    ull a2na3iimag = (a2 + pdiv - a3) * iimag % pdiv;
                    (a[i + offset        ] = a0 + a1 + a2 + a3) %= pdiv;
                    (a[i + offset + p    ] = (a0 + (pdiv - a1) + a2na3iimag) * irot) %= pdiv;
                    (a[i + offset + p * 2] = (a0 + a1 + (pdiv - a2) + (pdiv - a3)) * irot2) %= pdiv;
                    (a[i + offset + p * 3] = (a0 + (pdiv - a1) + (pdiv - a2na3iimag)) * irot3) %= pdiv;
                }
                if (s + 1 != s_len) {
                    (irot *= prep.irate3[__builtin_ctzll(~(ull)(s))]) %= pdiv;
                }
            }
            len -= 2;
        }
    }

    ll iz = modinv(1 << h, pdiv);
    for (auto &x : a) (x *= iz) %= pdiv;

    return a;
}

template <ll pdiv = 998244353, ll prim = 3>
vector<ll> butterfly_inv(const vector<ll>& a) {
    return butterfly_inv<pdiv, prim>(vector<ll>(a));
}
