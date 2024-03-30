// #REQ: base_template.hpp

// 中央値が取れるmultiset。追加・削除・中央値取得などがならしO(logN)となるはず。
// SetTypeをset<T>にすることで重複無しが可能。
template<typename T, typename SetType = multiset<T>>
struct median_multiset {

    SetType s0;
    SetType s1;

    // 順序を保ったまま個数のバランスを取って{n(+1),n}とする。
    void balance() {
        const auto parity = size() % 2;
        while (s0.size() > s1.size() + parity) {
            const auto itr = prev(s0.end());
            s1.emplace(*itr);
            s0.erase(itr);
        }
        while (s0.size() < s1.size()) {
            const auto itr = s1.begin();
            s0.emplace(*itr);
            s1.erase(itr);
        }
        assert(s0.size() == s1.size() + parity);
    }

    // 構築して追加。
    template <typename... Args>
    void emplace(Args&&... args) {
        auto a = T(forward<Args>(args)...);
        if (s0.empty()) {
            s0.emplace(move(a));
        } else if (s1.empty()) {
            if (a < *s0.begin()) {  // s0.size() == 1
                s0.swap(s1);
                s0.emplace(move(a));
            } else {
                s1.emplace(move(a));
            }
        } else {
            if (a < *prev(s0.end())) {
                s0.emplace(move(a));
            } else if (*s1.begin() < a) {
                s1.emplace(move(a));
            } else {
                if (s0.size() > s1.size()) {
                    s1.emplace_hint(s1.begin(), move(a));
                } else {
                    s0.emplace_hint(s0.end(), move(a));
                }
            }
        }
        balance();
    }
    
    // デフォルトでは floor(size()/2) 番目 (0-indexed) が返る。
    // 適宜(a+b)/2などに変更して使用のこと。
    T take_median_of_two(const T& a, const T&) const {
        return a;
    }

    // 中央値を取得する。
    T median() const {
        assert(!empty());
        if (const auto n0 = s0.size(), n1 = s1.size(); n0 > n1) {
            return *s0.rbegin();
        } else {
            return take_median_of_two(*s0.rbegin(), *s1.begin());
        }
    }

    // 要素数を取得する。O(1)
    size_t size() const {
        return s0.size() + s1.size();
    }

    // 空白のときtrueを返し、そうでないときfalseを返す。
    bool empty() const {
        return size() == 0;
    }

    // 値 a の要素を **すべて** 削除する。
    void erase_all(T a) {
        s0.erase(a);
        s1.erase(a);
        balance();
    }

    // 値 a の要素を **ひとつだけ** 削除する。
    void erase_one(T a) {
        if (auto itr = s0.find(a); itr != s0.end()) {
            s0.erase(itr);
        } else if (auto jtr = s1.find(a); jtr != s1.end()) {
            s1.erase(jtr);
        }
        balance();
    }

    // 他のmedian_multisetと中身をスワップする。
    void swap(median_multiset<T, SetType>& other) {
        s0.swap(other.s0);
        s1.swap(other.s1);
    }

    // 最小要素へのconst左辺値参照を返す。empty()のときUB。
    const T& front() const { return *s0.begin(); }

    // 最大要素へのconst左辺値参照を返す。empty()のときUB。
    const T& back() const { return !s1.empty() ? *s1.rbegin() : *s0.rbegin(); }
    
};