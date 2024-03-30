// #REQ: base_template.cpp

// 可逆の関数 f について f^(i)(x) = y を満たす最小の整数 0 <= i < sup_steps を求める
// 返値は {そのような i が存在するか, 存在するならばその i}
// ただし、
// T: 整数型
// giantstep(T x, ll m) -> T: x に f^m を適用
// babystep_inv(T x) -> T: x に f^(-1) を適用
template <typename T, typename GiantStep, typename BabyStepInv, typename enable_if<is_integral_v<T>, nullptr_t>::type = nullptr>
pair<bool, ll> bsgs(T x, T y, ll sup_steps, GiantStep giantstep, BabyStepInv babystepinv) {
    
    // 平方分割
    ll step_middle = roundl(sqrtl(sup_steps));
    while (step_middle > 0 && step_middle * step_middle > sup_steps) step_middle--;
    
    // baby step
    unordered_map<T, ll> babies;
    T current_baby = y;
    REP(i, step_middle) {

        // 同じところに戻ってきたらこれ以上やっても同じになるのでおわり
        if (babies.find(current_baby) != babies.end()) break;

        babies[current_baby] = i;
        if (i + 1 < i_len) {
            current_baby = babystepinv(current_baby);
        }

    }

    // giant step
    T current_giant = x;
    REP(i, (sup_steps + step_middle - 1) / step_middle) {

        if (babies.find(current_giant) != babies.end()) {
            // 最後の 1 ステップだけは i の範囲をはみ出る可能性がある
            const ll idx = i * step_middle + babies[current_giant];
            if (idx < sup_steps) {
                return {true, idx};
            }
        }

        if (i + 1 < i_len) {
            current_giant = giantstep(current_giant, step_middle);
        }

    }

    return {false, 0};
}