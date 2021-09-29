// 中身なしランレングス圧縮 O(N*(中身の比較))
template <class Iterator>
vector<size_t> run_length(Iterator first, Iterator last) {
    vector<size_t> rl;
    for (Iterator itr = first; itr != last; ) {
        size_t current_length = 1;
        Iterator jtr = itr++;
        while ((itr != last && *itr == *(itr-1))) itr++, current_length++;
        rl.emplace_back(current_length);
    }
    return rl;
}