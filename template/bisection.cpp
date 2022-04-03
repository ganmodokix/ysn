// #REQ: base_template.cpp

template <typename T, typename enable_if<is_integral_v<T>, nullptr_t>::type = nullptr>
T bisection_mid(T false_side, T true_side) {
    return false_side / 2 + true_side / 2 + ("AABBC"[2 + false_side % 2 + true_side % 2] - 'B');
}

template <typename T, typename enable_if<is_floating_point_v<T>, nullptr_t>::type = nullptr>
T bisection_mid(T false_side, T true_side) {
    return (false_side + true_side) * 0.5;
}

// 二分法 O(log(true_side - false_side) * predの時間計算量)
// false_sideを含まずtrue_sideを含む区間でpredがtrueとなる最もtrue_sideに近い値を返す
// pred(false_side) == false かつ pred(true_side) == true かつ pred が広義単調　が必要
// ⚠️ pred(false_side) および pred(true_side) は呼ばれない
// T が整数型かつ tolerant == 1 のとき厳密解を返す
// T が浮動小数点数型のときはtolerant未満だけ真の境界からtrue_sideにずれる
template <typename T, typename Predicate>
T bisection(T false_side, T true_side, Predicate pred, T tolerant = 1) {
    assert(!is_integral_v<T> || tolerant == 1);
    while ((false_side > true_side ? false_side - true_side : true_side - false_side) > tolerant) {
        T mid_value = bisection_mid(false_side, true_side);
        (pred(mid_value) ? true_side : false_side) = mid_value;
    }
    return true_side;
}

// 指数探索 O(log(result - false_side) * predの時間計算量)
// true_side が不明な時に使う
// ⚠️ pred が true_side_direction の符号の方向に単調な必要がある
template <typename T, typename Predicate>
T exponential_search(T false_side, T true_side_direction, Predicate pred, T tolerant = 1) {
    assert(true_side_direction != 0);
    assert(!is_integral_v<T> || tolerant == 1);
    ll true_side = false_side;
    ll offset = (true_side_direction > 0) - (true_side_direction < 0);
    while (!pred(false_side + offset)) offset *= 2;
    return bisection(false_side, false_side + offset, pred, tolerant);
}