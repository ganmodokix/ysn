// #REQ: base_template
// 中央値付き multiset
template <typename T>
struct median_multiset {

    multiset<T> s, t;
    median_multiset<T>() = default;

    void regularize() {
        while (s.size() && t.size() && *s.rbegin() > *t.begin()) {
            auto x = *s.rbegin();
            s.erase(prev(s.end()));
            t.emplace(x);
        }
        while (s.size() > t.size()) {
            auto x = *s.rbegin();
            s.erase(prev(s.end()));
            t.emplace(x);
        }
        while (s.size()+1 < t.size()) {
            auto x = *t.begin();
            t.erase(t.begin());
            s.emplace(x);
        }
    }

    void push(T x) {
        s.emplace(x);
        regularize();
    }

    void pop(T x) {
        if (auto itr = s.find(x); itr != s.end()) {
            s.erase(itr);
        } else if (auto itr = t.find(x); itr != t.end()) {
            t.erase(itr);
        } else {
            assert(false);
        }
        regularize();
    }

    T median() const {
        if (s.size() == t.size()) {
            return (*s.rbegin() + *t.begin()) / 2;
        } else {
            return *t.begin();
        }
    }

};