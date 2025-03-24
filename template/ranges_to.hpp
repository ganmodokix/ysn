#pragma once
#include "base_template.hpp"

namespace ganmodokix {

    template <typename Container>
    struct _To {
        _To() = default;
        template <ranges::input_range R>
        friend auto operator| (R&& r, const _To) {
            auto rv = r | views::common;
            auto result = Container(ranges::begin(rv), ranges::end(rv));
            return result;
        }
    };

}


template <typename T>
struct to_vector : ganmodokix::_To<vector<T>> {};
template <typename T>
struct to_unorderd_set : ganmodokix::_To<unordered_set<T>> {};
template <typename T>
struct to_set : ganmodokix::_To<set<T>> {};