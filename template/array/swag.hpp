#pragma once
#include "base_template.hpp"
// Sliding Window Aggregation
// - モノイド (T, Opr, e) に対し総和を返す (半群でもできるが、めんどい)
// - litr/ritr を一つ進めるのは Θ(1) amortized
// - 総和を求めるのは Θ(1)
// ! Opr は T Opr::operator(T) を持つ必要がある
template<typename T, typename Opr>
struct swag {
    const T id;
    const Opr opr;
    stack<pair<T, T>> fs, bs;

    // _id: 単位元, _opr: 演算 (T,T)->T
    swag(T _id, Opr _opr):
        id(_id), opr(_opr), fs(), bs()
    {}

    T sum_front() const { return fs.empty() ? id : fs.top().second; }
    T sum_back () const { return bs.empty() ? id : bs.top().second; }
    T sum() const { return opr(sum_front(), sum_back()); }

    void pop_front() {
        assert(!fs.empty() || !bs.empty());
        if (fs.empty()) {
            while (!bs.empty()) {
                T ai = bs.top().first;
                T si = opr(ai, sum_front());
                bs.pop();
                fs.push(pair<T, T>(ai, si));
            }
        }
        fs.pop();
    }

    void push_back(const T ai) {
        T si = opr(sum_back(), ai);
        bs.push(pair<T, T>(ai, si));
    }

};
