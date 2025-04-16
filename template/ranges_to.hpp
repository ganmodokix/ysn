#pragma once
#include "base_template.hpp"

namespace ganmodokix {

    template <typename From, typename To>
    struct _Converter {
        constexpr To operator() (From x) const;
    };

    template <typename From, typename To>
    requires convertible_to<From, To>
    struct _Converter<From, To> {
        constexpr To operator() (From x) const {
            return static_cast<To>(x);
        }
    };

    template <typename Container>
    struct _To {
        _To() = default;
        template <ranges::input_range R>
        friend auto operator| (R&& r, const _To) {
            using FromT = ranges::range_value_t<R>;
            using ToT = Container::value_type;
            auto rv = r | views::transform(_Converter<FromT, ToT>{}) | views::common;
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