// #REQ: base_template/575
namespace opf {
    struct min { template <typename T> T operator() (const T &x, const T &y) const { return std::min(x, y); } };
    struct max { template <typename T> T operator() (const T &x, const T &y) const { return std::max(x, y); } };
    struct gcd { template <typename T> T operator() (const T &x, const T &y) const { return std::gcd(x, y); } };
    struct lcm { template <typename T> T operator() (const T &x, const T &y) const { return std::lcm(x, y); } };
    struct plus { template <typename T> T operator() (const T &x, const T &y) const { return x + y; } };
    struct multiply { template <typename T> T operator() (const T &x, const T &y) const { return x * y; } };
};
