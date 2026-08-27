#include <gtest/gtest.h>

#include "segment_union.hpp"

template <typename T>
class SegmentUnionTest : public ::testing::Test {
};

using SegmentUnionTypes = ::testing::Types<
    int,
    long long int,
    ::std::uint64_t
>;

TYPED_TEST_SUITE(SegmentUnionTest, SegmentUnionTypes);

TYPED_TEST(SegmentUnionTest, Normal) {

    // random cases
    constexpr auto num_cases = 100;
    constexpr auto num_queries_per_case = 100;

    for (const auto index_case : std::views::iota(0, num_cases)) {
        auto mt = mt19937_64{static_cast<uint_fast64_t>(index_case)};
        auto qgen = uniform_int_distribution<ll>(0, 1);
        constexpr auto nv = 60LL;
        auto xgen = uniform_int_distribution<ll>(0, nv - 1);

        auto actual = segment_union{};
        auto expected = 0ULL;

        REP(i, num_queries_per_case) {

            // generate query
            auto l = xgen(mt);
            auto r = xgen(mt);
            if (l <= r) r++; else swap(l, r);
            auto qn = qgen(mt);
            
            // execute query
            if (qn) {
                actual.insert(l, r);
                expected |= ((1ULL << r) - 1) ^ ((1ULL << l) - 1);
            } else {
                actual.erase(l, r);
                expected &= ~(((1ULL << r) - 1) ^ ((1ULL << l) - 1));
            }

            // validate state
            {
                auto actual_bits = 0ULL;
                for (const auto& [l, r] : actual.s) {
                    actual_bits |= ((1ULL << r) - 1) ^ ((1ULL << l) - 1);
                }
                EXPECT_EQ(expected, actual_bits);
            }
        }
    }

}