// #REQ: base_template
// 偏角ソート用比較関数 (T: integral/floating point)
// 型により挙動が変わるので注意
struct less_arg {

    template <typename T, typename enable_if<is_integral_v<T>, nullptr_t>::type = nullptr>
    bool operator() (pair<T, T> a, pair<T, T> b) {
        
        const static auto orthant = [](const pair<T, T> &w) {
            const auto &[x, y] = w;
            if (x < 0 && y <= 0) {
                return 0;
            } else if (x >= 0 && y < 0) {
                return 1;
            } else if (x > 0 && y >= 0) {
                return 2;
            } else if (x <= 0 && y > 0) {
                return 3;
            } else {
                return 2;
            }
        };

        const static auto rot90 = [](pair<T, T> &w) {
            swap(w.first, w.second);
            w.first *= -1;
        };

        auto oa = orthant(a);
        auto ob = orthant(b);

        if (oa != ob) {
            return oa < ob;
        } else {
            REP(i, (6 - oa) % 4) {
                rot90(a);
                rot90(b);
            }
            ll op = a.first * b.second - a.second * b.first;
            if (op != 0) {
                return op > 0;
            } else {
                return a.first ? a.first < b.first : a.second < b.second;
            }
        }

    }

    template <typename T, typename enable_if<is_floating_point_v<T>, nullptr_t>::type = nullptr>
    bool operator() (pair<T, T> a, pair<T, T> b) {
        return atan2l(a.second, a.first) < atan2l(b.second, b.first);
    }

};