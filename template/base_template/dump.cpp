// #REQ: base_template/575 base_template/debug_mode
#define DUMP(...) ::ganmodokix::__dump(__FILE__, __LINE__, #__VA_ARGS__, __VA_ARGS__)
namespace ganmodokix {
    template <typename T>
    ostream& __dump_single(const T& value) {
        if constexpr (is_convertible_v<T, string_view>) {
            cerr << "\e[32m\"" << value << "\"\e[m";
        } else if constexpr (is_arithmetic_v<T>) {
            if constexpr (is_integral_v<T>) { cerr << "\e[33m"; }
            if constexpr (is_floating_point_v<T>) { cerr << "\e[35m"; }
            if constexpr (is_same_v<remove_cv_t<remove_reference_t<T>>, char>) { cerr << "\e[31m\'"; }
            cerr << boolalpha << value;
            if constexpr (is_same_v<remove_cv_t<remove_reference_t<T>>, char>) { cerr << "\'"; }
            if constexpr (is_floating_point_v<T>) { cerr << "f"; }
            if constexpr (is_unsigned_v<T> && !is_convertible_v<T, bool>) { cerr << "U"; }
        } else {
            cerr << value;
        }
        return cerr << "\e[m";
    }
    template <typename T, template <typename> typename Container>
    ostream& __dump_single(const Container<T>& value) {
        cerr << "{";
        auto first = true;
        for (const auto& x : value) {
            cerr << ", " + first * 2;
            __dump_single<T>(x);
            first = false;
        }
        cerr << "}";
        return cerr;
    }
    template <typename T>
    ostream& __dump_single(basic_string<T> value) {
        cerr << "\e[32m\"" << value << "\"s\e[m";
        return cerr;
    }
    template <typename T>
    ostream& __dump_single(basic_string_view<T> value) {
        cerr << "\e[32m\"" << value << "\"sv\e[m";
        return cerr;
    }
    template <typename... Args>
    void __dump(const char* const file, int line, const char* const title, const Args&... args) {
        if (!DEBUG_MODE) return;
        constexpr auto is_multiple = sizeof...(Args) > 1;
        const auto kakko_first = "{" + !is_multiple;
        const auto kakko_last = "}" + !is_multiple;
        auto first = true;
        cerr << "\e[33m\e[2m[" << file << "]\e[m ";
        cerr << "\e[36m" << kakko_first << title << kakko_last << "\e[m: ";
        cerr << kakko_first;
        (((cerr << ", " + first * 2, __dump_single(args)), first = false), ...);
        cerr << kakko_last;
        cerr << " \e[37m(L" << line << ")\e[m";
        cerr << endl;
    }
}