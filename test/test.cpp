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

    DUMP(vector{1, 2, 3, 4, 5});
    DUMP(pair(1, 2));
    DUMP(pair(1, 'v'));
    DUMP(pair(1, pair(3, 2)));
    DUMP(pair(vector(2, 0LL), 12345));
    DUMP(tuple(vector(2, 0LL), vector(1, 0LL), 12345));
    DUMP(vector<ll>{});
    DUMP(tuple<>{});
    DUMP(tuple<int>{1});

    DUMP(make_tuple(make_tuple(2)));
    DUMP(make_tuple(make_tuple(make_tuple(2))));
    DUMP(make_tuple(pair(2, 2), pair(1, 4)));
    DUMP(pair(tuple(pair(1, 2), 1), tuple(pair(tuple(2), tuple(vector{tuple("sv"sv)})))));
}