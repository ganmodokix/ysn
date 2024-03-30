// #REQ: base_template
// 中身付きランレングス圧縮 O(N * (中身の比較+コピー))
template <class Iterator>
vector<pair<size_t, typename remove_reference<decltype(*Iterator())>::type>> run_length(Iterator first, Iterator last) {
    vector<pair<size_t, typename remove_reference<decltype(*Iterator())>::type>> rl;
    for (Iterator itr = first; itr != last; ) {
        size_t current_length = 1;
        Iterator jtr = itr++;
        while ((itr != last && *itr == *(itr-1))) itr++, current_length++;
        rl.emplace_back(current_length, *jtr);
    }
    return rl;
}