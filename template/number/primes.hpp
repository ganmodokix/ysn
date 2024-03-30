// #REQ: base_template number/sieve
// 素数列挙 O(N)
vector<ll> primes_under(size_t n) {
    return sieve_under(n).second;
}