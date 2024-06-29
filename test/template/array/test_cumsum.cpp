#include <gtest/gtest.h>

#include "array/cumsum.hpp"

template <typename T>
class CumulativeSumTest : public ::testing::Test {
};

using CumulativeSumTypes = ::testing::Types<
    int,
    long long int,
    ::std::uint64_t,
    float,
    double
>;

TYPED_TEST_SUITE(CumulativeSumTest, CumulativeSumTypes);

TYPED_TEST(CumulativeSumTest, Normal) {

    const auto cs1 = cumsum(vector<TypeParam>{1, 2, 3, 4, 5}, ::std::plus{}, TypeParam{});
    if constexpr (::std::integral<TypeParam>) {
        EXPECT_EQ(TypeParam{}, cs1.sum(0));
        EXPECT_EQ(TypeParam{1}, cs1.sum(1));
        EXPECT_EQ(TypeParam{3}, cs1.sum(2));
        EXPECT_EQ(TypeParam{6}, cs1.sum(3));
        EXPECT_EQ(TypeParam{10}, cs1.sum(4));
        EXPECT_EQ(TypeParam{15}, cs1.sum(5));
    }
    if constexpr (::std::floating_point<TypeParam>) {
        EXPECT_FLOAT_EQ(TypeParam{}, cs1.sum(0));
        EXPECT_FLOAT_EQ(TypeParam{1}, cs1.sum(1));
        EXPECT_FLOAT_EQ(TypeParam{3}, cs1.sum(2));
        EXPECT_FLOAT_EQ(TypeParam{6}, cs1.sum(3));
        EXPECT_FLOAT_EQ(TypeParam{10}, cs1.sum(4));
        EXPECT_FLOAT_EQ(TypeParam{15}, cs1.sum(5));
    }

    const auto cs2 = cumsum(vector<TypeParam>{1, 2, 3, 4, 5}, ::std::plus{}, TypeParam{0});
    if constexpr (::std::integral<TypeParam>) {
        EXPECT_EQ(TypeParam{}, cs2.sum(0));
        EXPECT_EQ(TypeParam{1}, cs2.sum(1));
        EXPECT_EQ(TypeParam{3}, cs2.sum(2));
        EXPECT_EQ(TypeParam{6}, cs2.sum(3));
        EXPECT_EQ(TypeParam{10}, cs2.sum(4));
        EXPECT_EQ(TypeParam{15}, cs2.sum(5));
    }
    if constexpr (::std::floating_point<TypeParam>) {
        EXPECT_FLOAT_EQ(TypeParam{}, cs2.sum(0));
        EXPECT_FLOAT_EQ(TypeParam{1}, cs2.sum(1));
        EXPECT_FLOAT_EQ(TypeParam{3}, cs2.sum(2));
        EXPECT_FLOAT_EQ(TypeParam{6}, cs2.sum(3));
        EXPECT_FLOAT_EQ(TypeParam{10}, cs2.sum(4));
        EXPECT_FLOAT_EQ(TypeParam{15}, cs2.sum(5));
    }

}