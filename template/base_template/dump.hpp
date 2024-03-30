#pragma once
#include "base_template/575.hpp"
#include "base_template/debug_mode.hpp"
#define DUMP(...) ::ganmodokix::__dump(__FILE__, __LINE__, #__VA_ARGS__, __VA_ARGS__)
namespace ganmodokix {
    ostream& __dump_single(const char* value);
    ostream& __dump_single(string_view value);
    ostream& __dump_single(const string& value);
    template <typename T>
    ostream& __dump_single(optional<T> value);
    template <typename Container>
    auto __dump_single(const Container& value) -> decltype(begin(value), end(value), (cerr));
    template <typename... Args>
    ostream& __dump_single(const tuple<Args...>& value);
    template <typename T, typename U>
    ostream& __dump_single(const pair<T, U>& value);

    template <typename T>
    auto __dump_single(const T& value) -> decltype((cerr << value)) {
        if constexpr (is_convertible_v<T, string_view>) {
            cerr << "\e[32m\"" << value << "\"\e[m";
        } else if constexpr (is_arithmetic_v<T>) {
            if constexpr (is_integral_v<T>) { cerr << "\e[33m"; }
            if constexpr (is_floating_point_v<T>) { cerr << "\e[35m"; }
            if constexpr (is_same_v<remove_cv_t<remove_reference_t<T>>, char>) { cerr << "\e[31m\'"; }
            cerr << boolalpha << value;
            if constexpr (is_same_v<remove_cv_t<remove_reference_t<T>>, char>) { cerr << "\'(" << (int)value << ")"; }
            if constexpr (is_floating_point_v<T>) { cerr << "f"; }
            if constexpr (is_unsigned_v<T> && !is_same_v<remove_cv_t<remove_reference_t<T>>, bool>) { cerr << "U"; }
        } else {
            cerr << value;
        }
        return cerr << "\e[m";
    }
    template <typename T, typename U>
    ostream& __dump_single(const pair<T, U>& value) {
        cerr << "pair{"; __dump_single(value.first) << ", "; __dump_single(value.second); return cerr << "}";
    }
    template <typename Tuple, typename T, size_t... Seq>
    void __dump_tuple(const Tuple& t, integer_sequence<T, Seq...>)
    {
        auto first = true;
        ((cerr << ", " + first * 2, __dump_single(get<Seq>(t)), first = false), ...);
    }
    template <typename... Args>
    ostream& __dump_single(const tuple<Args...>& value) {
        cerr << "tuple{"; __dump_tuple(value, index_sequence_for<Args...>{}); return cerr << "}";
    }
    ostream& __dump_single(const char* const value) { cerr << "\e[32m\"" << value << "\"\e[m"; return cerr; }
    ostream& __dump_single(const string& value) { cerr << "\e[32m\"" << value << "\"s\e[m"; return cerr; }
    ostream& __dump_single(string_view value) { cerr << "\e[32m\"" << value << "\"sv\e[m"; return cerr; }
    template <typename Container>
    auto __dump_single(const Container& value) -> decltype(begin(value), end(value), (cerr)) {
        cerr << "c{";
        auto first = true;
        for (const auto& x : value) { cerr << ", " + first * 2; __dump_single(x); first = false; }
        return cerr << "}";
    }
    template <typename T>
    ostream& __dump_single(optional<T> value) {
        if (!value.has_value()) {
        return cerr << "\e[30mnullopt\e[m";
        } else {
            return __dump_single(value.value());
        }
    }
    template <typename... Args>
    void __dump(const char* const file, int line, const char* const title, const Args&... args) {
        if (!DEBUG_MODE) return;
        constexpr auto is_multiple = sizeof...(Args) > 1;
        const auto par_first = "{" + !is_multiple;
        const auto par_last = "}" + !is_multiple;
        auto first = true;
        cerr << "\e[33m\e[2m[" << file << "]\e[m ";
        cerr << "\e[36m" << par_first << title << par_last << "\e[m: ";
        cerr << par_first; (((cerr << ", " + first * 2, __dump_single(args)), first = false), ...); cerr << par_last;
        cerr << " \e[37m(L" << line << ")\e[m" << endl;
    }
}
