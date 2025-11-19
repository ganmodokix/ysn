#pragma once
#include "base_template.hpp"
#include "modint/modint.hpp"
#include "conv/ntt.hpp"

template <mod_integral T>
auto poly_frac_sequence_add(vector<pair<vector<T>, vector<T>>> fracs)
    -> pair<vector<T>, vector<T>>
{
    auto proj = [&](const ll i) {
        return max(ssize(fracs[i].first), ssize(fracs[i].second));
    };
    auto comp = [&](const ll i, const ll j) -> bool {
        return proj(i) > proj(j);
    };
    auto pque = priority_queue<ll, vector<ll>, decltype(comp)>(comp);
    REP(i, ssize(fracs)) {
        pque.emplace(i);
    }
    while (!pque.empty()) {
        // DUMP(ssize(pque), ssize(fracs));
        const auto i = pque.top(); pque.pop();
        if (pque.empty()) {
            return fracs[i];
        }
        const auto j = pque.top(); pque.pop();
        // DUMP(proj(i), proj(j));
        auto&& [fi, gi] = move(fracs[i]);
        auto&& [fj, gj] = move(fracs[j]);
        auto figj = convolve_p(move(fi), gj);
        auto fjgi = convolve_p(move(fj), gi);
        figj.resize(max(ssize(figj), ssize(fjgi)));
        fjgi.resize(max(ssize(figj), ssize(fjgi)));
        auto numer = move(figj);
        REP(i, ssize(numer)) numer[i] += fjgi[i];
        auto denom = convolve_p(move(gi), move(gj));
        fracs.emplace_back(move(numer), move(denom));
        pque.emplace(ssize(fracs) - 1);
        // DUMP(proj(ssize(fracs) - 1));
    }
    assert(ssize(pque) > 0);
    return fracs[pque.top()];
}