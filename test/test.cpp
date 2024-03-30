#include <gtest/gtest.h>

#include "base_template.hpp"

TEST(BaseTemplate, DumpMacro)
{

    DUMP('a');
    DUMP(123);
    DUMP(uint64_t{123});
    DUMP((ll)1e14);
    DUMP((ull)1e10);
    DUMP(1.0);
    DUMP(2.0f);
    DUMP(true);
    DUMP(false);
    DUMP('a', 123, (ll)1e14, (ull)1e10, 1.0, 2.0f);

    DUMP(optional<ll>(1234565));
    DUMP(optional<ll>{});

    DUMP("const char *");
    DUMP("string"s);
    DUMP("string_view"sv);

    const auto v = vector{1, 2, 3, 4, 5};
    DUMP(v);
    const auto p = pair(1, 2);
    DUMP(p);
    const auto p2 = pair(1, 'v');
    DUMP(p2);
    const auto pp = pair(1, pair(3, 2));
    DUMP(pp);
    const auto pv = tuple(vector(2, 0LL), 12345);
    DUMP(pv);
    const auto tvvi = tuple(vector(2, 0LL), vector(1, 0LL), 12345);
    DUMP(tvvi);
    const auto ve = vector<ll>{};
    DUMP(ve);
    const auto te = tuple<>{};
    DUMP(te);

    const auto tnested = tuple(tuple(tuple(tuple(tuple(tuple(1, 2, 3), tuple(2, 43))), tuple(1))));
    DUMP(tnested);
}