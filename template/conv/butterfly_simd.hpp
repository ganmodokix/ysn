#pragma once
#include "base_template.hpp"
#include "base_template/immintrin.hpp"
#include "modint/modint_petit_p.hpp"

constexpr uint32_t MOD  = 998244353;
constexpr uint64_t R2    = (uint64_t(1) << 32) % MOD * ((uint64_t(1) << 32) % MOD) % MOD; 
// INV = -MOD^{-1} mod 2^32 を事前に計算
constexpr uint32_t INV32 = modinv_extgcd(MOD, uint64_t{1} << 32);

// ——— モンゴメリ乗算 + 還元 ———
// 64bit レーン ×4（計 256bit）版
static inline __m256i montgomery_mul256(__m256i a, __m256i b) {
    // acc = a * b の 52bit 積を下位/上位に分けて累積
    __m256i acc = _mm256_setzero_si256();
    acc = _mm256_madd52lo_epu64(acc, a, b);  // VPMADD52LUQ
    acc = _mm256_madd52hi_epu64(acc, a, b);  // VPMADD52HUQ
    // acc = z + m*MOD (両命令で既に加算済み) → 64bit 単位で u
    // 最後に 2^32 でシフトすれば Montgomery 還元完了
    return _mm256_srli_epi64(acc, 32);
}

// 8 要素一括バタフライ（32bit 値を 64bit レーンにゼロ拡張して処理）
void butterfly_simd(
    uint32_t *a,      // モンゴメリ表現済みの配列
    long long n,      // 全長
    long long p,      // ステップ
    uint32_t irot,    // ω^p のモンゴメリ表現
    uint32_t iimag    // ω^{2p} のモンゴメリ表現
) {
    // 64bit レーンに直接セット
    __m256i vIrot  = _mm256_set1_epi64x(irot);
    __m256i vImag  = _mm256_set1_epi64x(iimag);
    __m256i vIrot2 = _mm256_set1_epi64x(uint64_t(irot)*irot % MOD);
    __m256i vIrot3 = _mm256_set1_epi64x(uint64_t(irot)*irot % MOD * irot % MOD);
    __m256i vR2    = _mm256_set1_epi64x(R2);

    for ( long long i = 0; i < n; i += 4*p ) {
        for ( long long j = 0; j < p; j += 4 ) {
            // 32bit→64bit ゼロ拡張ロード
            __m256i a0 = _mm256_cvtepu32_epi64(_mm_loadu_si128((__m128i*)(a + i + j      )));
            __m256i a1 = _mm256_cvtepu32_epi64(_mm_loadu_si128((__m128i*)(a + i + j +   p )));
            __m256i a2 = _mm256_cvtepu32_epi64(_mm_loadu_si128((__m128i*)(a + i + j + 2*p)));
            __m256i a3 = _mm256_cvtepu32_epi64(_mm_loadu_si128((__m128i*)(a + i + j + 3*p)));

            // t0 = (a0 + a1 + a2 + a3) * R^{-1} mod MOD
            __m256i sum01 = _mm256_add_epi64(a0, a1);
            __m256i sum23 = _mm256_add_epi64(a2, a3);
            __m256i t0    = _mm256_add_epi64(sum01, sum23);
            t0 = montgomery_mul256(vR2, t0);

            // t1 = (a0 - a1 + (a2 - a3)*iimag) * irot
            __m256i sub01  = _mm256_sub_epi64(a0, a1);
            __m256i sub23  = _mm256_sub_epi64(a2, a3);
            __m256i tmp1   = montgomery_mul256(sub23, vImag);
            __m256i t1_pre = _mm256_add_epi64(sub01, tmp1);
            __m256i t1     = montgomery_mul256(t1_pre, vIrot);

            // t2 = (a0 + a1 - a2 - a3) * irot2
            __m256i t2_pre = _mm256_sub_epi64(sum01, sum23);
            __m256i t2     = montgomery_mul256(t2_pre, vIrot2);

            // t3 = (a0 - a1 - (a2 - a3)*iimag) * irot3
            __m256i t3_pre = _mm256_sub_epi64(sub01, tmp1);
            __m256i t3     = montgomery_mul256(t3_pre, vIrot3);

            // 64bit→32bit に切り捨て（値範囲は <2^32 なので安全）
            __m128i r0 = _mm256_cvtepi64_epi32(t0);
            __m128i r1 = _mm256_cvtepi64_epi32(t1);
            __m128i r2 = _mm256_cvtepi64_epi32(t2);
            __m128i r3 = _mm256_cvtepi64_epi32(t3);

            // 書き戻し
            _mm_storeu_si128((__m128i*)(a + i + j    ), r0);
            _mm_storeu_si128((__m128i*)(a + i + j + p), r1);
            _mm_storeu_si128((__m128i*)(a + i + j +2*p), r2);
            _mm_storeu_si128((__m128i*)(a + i + j +3*p), r3);
        }
    }
}

void to_montgomery_simd(uint32_t *a, size_t n) {
    __m256i vR2 = _mm256_set1_epi64x(R2);
    for ( size_t i = 0; i < n; i += 4 ) {
        // ① 32bit×4 を 64bit×4 レーンにゼロ拡張ロード
        __m128i tmp = _mm_loadu_si128((__m128i*)(a + i));
        __m256i v   = _mm256_cvtepu32_epi64(tmp);
        // ② 各要素を montgomery_mul256(v, vR2) → x * R mod M
        v = montgomery_mul256(v, vR2);
        // ③ 64bit→32bit に安全にトランケートして戻す
        __m128i out = _mm256_cvtepi64_epi32(v);
        _mm_storeu_si128((__m128i*)(a + i), out);
    }
}