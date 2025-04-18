#pragma once
#include "base_template.hpp"
#include "modint/modint_petit_p.hpp"
#include "modint/modint.hpp"
#include "number/primitive_root.hpp"
#include "number/miller_rabin.hpp"

// 2段ずつのバタフライ演算
// 参考実装: ACL https://github.com/atcoder/ac-library/blob/master/atcoder/convolution.hpp

// (pdiv, prim) ごとに前計算可能なもの
template <mod_integral T, T prim>
struct butterfly_preprocess {
    
    static constexpr ll rank2 = countr_zero<ull>(T::pdiv - 1);
    array<T, rank2 + 1> root;
    array<T, rank2 + 1> iroot;
    array<T, max(0LL, rank2 - 1)> rate2;
    array<T, max(0LL, rank2 - 1)> irate2;
    array<T, max(0LL, rank2 - 2)> rate3;
    array<T, max(0LL, rank2 - 2)> irate3;

    constexpr butterfly_preprocess() noexcept {
        root[rank2] = prim.pow((T::pdiv - 1) >> rank2);
        iroot[rank2] = root[rank2].inv();
        DSRNG(i, rank2 - 1, 0) {
            root[i] = root[i + 1] * root[i + 1];
            iroot[i] = iroot[i + 1] * iroot[i + 1];
        }
        {
            auto prod = T{1};
            auto iprod = T{1};
            REP(i, rank2 - 1) {
                rate2[i] = root[i + 2] * prod;
                irate2[i] = iroot[i + 2] * iprod;
                prod *= iroot[i + 2];
                iprod *= root[i + 2];
            }
        }
        {
            auto prod = T{1};
            auto iprod = T{1};
            REP(i, rank2 - 2) {
                rate3[i] = root[i + 3] * prod;
                irate3[i] = iroot[i + 3] * iprod;
                prod *= iroot[i + 3];
                iprod *= root[i + 3];
            }
        }
    }

    static butterfly_preprocess<T, prim>& singleton() noexcept {
        static auto prep = butterfly_preprocess<T, prim>{};
        return prep;
    }

};

// ACL実装のバタフライ演算
template <mod_integral T, T prim>
requires (is_prime(T::pdiv))
vector<T> butterfly(vector<T> a) {

    assert(has_single_bit(a.size()));  // 長さが2冪か
    assert(countr_zero<ull>(T::pdiv - 1) >= countr_zero(a.size()));  // 1の|A|乗根が求まるか

    const auto n = (ll)a.size();
    const auto h = (ll)countr_zero<ull>(n);

    static const auto& prep = butterfly_preprocess<T, prim>::singleton();

    // 飛ばし飛ばしのバタフライ演算で定数倍を改善
    for (auto len = 0LL; len < h; ) {
        auto rot = T{1};
        if (len + 1 == h) {
            const auto p = 1LL << (h - len - 1);
            REP(s, 1 << len) {
                const auto offset = s << (h - len);
                REP(i, p) {
                    auto&& al = a[i + offset];
                    auto&& ar = a[i + offset + p];
                    const auto l = al;
                    const auto r = ar * rot;
                    al = l + r;
                    ar = l - r;
                }
                if (s + 1 == s_len) break;
                rot *= prep.rate2[countr_zero(~(ull)s)];
            }
            len++;
        } else {
            // 4-base
            const ll p = 1 << (h - len - 2);
            const auto imag = prep.root[2];
            REP(s, 1 << len) {
                const auto rot2 = rot * rot;
                const auto rot3 = rot2 * rot;
                const ll offset = s << (h - len);
                REP(i, p) {
                    const auto a0 = a[i + offset        ];
                    const auto a1 = a[i + offset + p    ] * rot;
                    const auto a2 = a[i + offset + p * 2] * rot2;
                    const auto a3 = a[i + offset + p * 3] * rot3;
                    const auto a1na3imag = (a1 - a3) * imag;
                    a[i + offset        ] = a0 + a2 + a1 + a3;
                    a[i + offset + p    ] = a0 + a2 - a1 - a3;
                    a[i + offset + p * 2] = a0 - a2 + a1na3imag;
                    a[i + offset + p * 3] = a0 - a2 - a1na3imag;
                }
                if (s + 1 == s_len) break;
                rot *= prep.rate3[countr_zero(~(ull)s)];
            }
            len += 2;
        }
    }

    return a;
}

// ACL実装の逆バタフライ演算
template <mod_integral T, T prim>
vector<T> butterfly_inv(vector<T> a) {
    assert(has_single_bit(a.size()));  // 長さが2冪か
    assert(countr_zero<ull>(T::pdiv - 1) >= countr_zero(a.size()));  // 1の|A|乗根が求まるか

    const auto n = (ll)a.size();
    const auto h = (ll)countr_zero<ull>(n);

    static const auto& prep = butterfly_preprocess<T, prim>::singleton();

    // 飛ばし飛ばしのバタフライ演算で定数倍を改善
    for (ll len = h; len; ) {
        const ll p = 1 << (h - len);
        auto irot = T{1};
        if (len == 1) {
            REP(s, 1 << (len - 1)) {
                const auto offset = s << (h - len + 1);
                REP(i, p) {
                    auto&& al = a[i + offset];
                    auto&& ar = a[i + offset + p];
                    const auto l = al;
                    const auto r = ar;
                    al = l + r;
                    ar = (l - r) * irot;
                }
                if (s + 1 == s_len) break;
                irot *= prep.irate2[countr_zero(~(ull)(s))];
            }
            len--;
        } else {
            // 4-base
            const auto iimag = prep.iroot[2];
            REP(s, 1 << (len - 2)) {
                const auto irot2 = irot * irot;
                const auto irot3 = irot2 * irot;
                const auto offset = s << (h - len + 2);
                REP(i, p) {
                    const auto a0 = a[i + offset        ];
                    const auto a1 = a[i + offset + p    ];
                    const auto a2 = a[i + offset + p * 2];
                    const auto a3 = a[i + offset + p * 3];
                    const auto a2na3iimag = (a2 - a3) * iimag;
                    a[i + offset        ] = a0 + a1 + a2 + a3;
                    a[i + offset + p    ] = (a0 - a1 + a2na3iimag) * irot;
                    a[i + offset + p * 2] = (a0 + a1 - a2 - a3) * irot2;
                    a[i + offset + p * 3] = (a0 - a1 - a2na3iimag) * irot3;
                }
                if (s + 1 == s_len) break;
                irot *= prep.irate3[countr_zero(~(ull)(s))];
            }
            len -= 2;
        }
    }

    const auto iz = T{1 << h}.inv();
    for (auto&& x : a) x *= iz;

    return a;
}
