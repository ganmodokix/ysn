#include <gtest/gtest.h>

#include <vector>
#include <cstddef>
#include <ranges>

#include "array/argsort.hpp"

template <std::ranges::random_access_range R>
void validate_argsort(R&& r) {
    
    const auto n = std::ranges::size(r);
    const auto actual = argsort(r);

    // 結果が順列かチェック
    const auto s = std::unordered_set(std::ranges::begin(actual), std::ranges::end(actual));
    EXPECT_EQ(n, s.size());
    if (!std::ranges::empty(r)) {
        EXPECT_EQ(0, std::ranges::min(s));
        EXPECT_EQ(n - 1, std::ranges::max(s));
    }

    // 添字を射影してソートになるかチェック
    const auto proj = [&r](const auto idx) { return *(std::ranges::begin(r) + idx); };
    const auto vc = actual | std::views::transform(proj);
    EXPECT_TRUE(std::ranges::is_sorted(vc));

}

TEST(ArgSort, Normal)
{

    EXPECT_EQ(std::vector<std::size_t>{}, argsort(std::vector<int>{}));

    constexpr auto LNINF = std::numeric_limits<long long>::lowest();
    constexpr auto LINF = std::numeric_limits<long long>::max();
    validate_argsort(std::vector{23, 2187, 437, 120, 39});
    validate_argsort(std::vector{0, 1, 2, 3, 4});
    validate_argsort(std::vector{4, 3, 2, 1, 0});
    validate_argsort(std::vector{0, 0, 0, 0, 0});
    validate_argsort(std::vector{LINF, LINF, LNINF, LNINF});
    validate_argsort(std::vector{1});

    validate_argsort(std::array{1, 1, 2, 5, 14});

    
}