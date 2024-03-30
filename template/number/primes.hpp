#pragma once
#include "base_template.hpp"
#include "number/sieve.hpp"
// 素数列挙 O(N)
vector<ll> primes_under(size_t n) {
    return sieve_under(n).second;
}
