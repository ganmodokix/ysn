#pragma once
#include "base_template.hpp"
// XORShift 擬似乱数発生器
uint32_t xor128(void){
    static uint32_t x = 123456789;
    static uint32_t y = 362436069;
    static uint32_t z = 521288629;
    static uint32_t w = 88675123; 
    uint32_t t;

    t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));  
}
double rand_uniform(void) { return (double)xor128() / 0x100000000LL; }
template <typename T> T rand_range(T min, T max) { return min + xor128() % (max - min + 1); }
bool rand_prob(double p) { return rand_uniform() < p; }
template <typename T> size_t roulette_choice(const vector<T> &p) {
    T sum = std::accumulate(p.begin(), p.end(), (T)0);
    T dice = rand_uniform() * sum;
    for (size_t i = 0; i < p.size(); i++) {
        if ((dice -= p[i]) < 0) return i;
    }
    return p.size() - 1; // for underflow
}
pair<double, double> rand_normal(void) {
    const double x = rand_uniform();
    const double y = rand_uniform();
    const double norm = sqrt(-2 * log(x));
    const double z1 = norm * cos(acos(0)*4*y);
    const double z2 = norm * sin(acos(0)*4*y);
    return {z1, z2};
}
template <typename T> vector<long double> softmax(const vector<T> &v, bool divide_by_sum = true) {
    T vmax = *max_element(v.begin(), v.end());
    vector<long double> p(v.size());
    for (size_t i = 0; i < v.size(); i++) p[i] = exp(v[i] - vmax);
    if (divide_by_sum) {
        T sump = accumulate(p.begin(), p.end(), 0.0L);
        for (auto &pi : p) pi /= sump;
    }
    return p;
}
