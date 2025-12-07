#include <gtest/gtest.h>

#include "array/bicumsum.hpp"

template <typename T>
class BidirectionalCumulativeSumTest : public ::testing::Test {
};

using BidirectionalCumulativeSumTypes = ::testing::Types<
    int,
    long long int,
    ::std::uint64_t,
    float,
    double
>;

TYPED_TEST_SUITE(BidirectionalCumulativeSumTest, BidirectionalCumulativeSumTypes);

TYPED_TEST(BidirectionalCumulativeSumTest, Normal) {

    const auto v = ::std::vector<TypeParam>{1, 2, 3, 4, 5};
    const auto op = std::plus{};
    const auto id = TypeParam{0};

    const auto bcs = bicumsum(v, op, id);

}