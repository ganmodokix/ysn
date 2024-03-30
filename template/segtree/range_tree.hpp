// #REQ: base_template.hpp number/ceillog2.hpp
// 領域木：矩形領域の総和をオンラインクエリで取る
// compile(): 前処理 O(NlogN)
// fetch(l1, l2, r1, r2): [l1, l2)×[r1, r2) を取得 O((logN)^2)
// join(x, y): 和
// ⚠ 以下の制約を満たさないとき不定
// - (join, T) は可換群であり、id はその単位元、inv は逆元
// ※ 可換半群でもできそうだが log N がもう一つ付くので future work
template <typename T, typename Join = plus<>, typename Inv = negate<>>
struct range_tree {

    // 結局座圧するため符号付きにしておく
    using index_type = ll;
    using value_type = T;
    using index_value_type = pair<index_type, value_type>;

    const Join join = {};
    const Inv inv = {};
    const value_type id = {};

    vector<tuple<index_type, index_type, value_type>> items = {};
    
    vector<vector<pair<index_type, value_type>>> node = {};
    vector<vector<value_type>> node_cs = {};
    index_type binary_size = 0;
    bool compiled = false;

    map<index_type, index_type> sahz_i = {};
    vector<index_type> inv_sahz_i = {};
    
    range_tree(Join join_ = {}, Inv inv_ = {}, value_type id_ = {}):
        join(join_), inv(inv_), id(id_)
    {}

    template <typename VT>
    void add(const index_type i, const index_type j, VT&& x) {
        assert(!compiled);
        items.emplace_back(i, j, forward<VT>(x));
    }

    // 前処理を行う O(NlogN)
    void compile() {
        assert(!compiled);
        compiled = true;

        // index compression
        for (const auto& [i, j, x] : items) {
            inv_sahz_i.push_back(i);
        }
        UNIQUE(inv_sahz_i);
        for (const auto idx : views::iota(index_type{0}, static_cast<index_type>(inv_sahz_i.size()))) {
            sahz_i[inv_sahz_i[idx]] = idx;
        }
        inv_sahz_i.push_back(numeric_limits<index_type>::max());

        // building binary tree
        binary_size = 1LL << ceillog2(ranges::size(inv_sahz_i) - 1);
        node = vector(binary_size * 2 - 1, vector<pair<index_type, value_type>>{});
        for (auto&& [i_, j, x] : items) {
            const auto i = sahz_i[i_];
            node[binary_size - 1 + i].emplace_back(j, forward<decltype(x)>(x));
        }
        for (auto idx = binary_size - 1; idx--; ) {
            const auto& child1 = node[idx * 2 + 1];
            const auto& child2 = node[idx * 2 + 2];
            auto& parent = node[idx];
            for (size_t idx1 = 0, idx2 = 0; idx1 < child1.size() || idx2 < child2.size(); ) {
                if (idx2 >= child2.size() || (idx1 < child1.size() && child1[idx1].first <= child2[idx2].first)) {
                    parent.push_back(child1[idx1++]);
                } else {
                    parent.push_back(child2[idx2++]);
                }
            }
        }

        // cumsum
        node_cs = vector(node.size(), vector{id});
        for (const auto idx : views::iota(index_type{0}, static_cast<index_type>(node.size()))) {
            for (const auto& [j, x] : node[idx]) {
                node_cs[idx].push_back(join(node_cs[idx].back(), x));
            }
        }
    }

    // （要前処理）座圧済みの範囲 [ib,ie)×[jb,je) についての総和をとる O((logN)^2)
    value_type fetch_compressed(index_type ib, index_type ie, index_type jb, index_type je) {
        assert(compiled);
        auto ans = id;
        auto add_node = [&](index_type idx) {
            const auto b = ranges::lower_bound(node[idx], jb, less{}, &pair<index_type, value_type>::first) - ranges::begin(node[idx]);
            const auto e = ranges::lower_bound(node[idx], je, less{}, &pair<index_type, value_type>::first) - ranges::begin(node[idx]);
            ans = join(move(ans), join(node_cs[idx][e], inv(node_cs[idx][b])));
        };
        for (auto offset = binary_size - 1; ib < ie; ib >>= 1, ie >>= 1, --offset /= 2) {
            if (ib & 1) add_node(offset + ib++);
            if (ie & 1) add_node(offset + --ie);
        }
        return ans;
    }

    // （要前処理）範囲 [ib, ie)×[jb, je) についての総和を取る O((logN)^2)
    value_type fetch(index_type ib, index_type ie, index_type jb, index_type je) {
        return fetch_compressed(
            ranges::lower_bound(inv_sahz_i, ib) - ranges::begin(inv_sahz_i),
            ranges::lower_bound(inv_sahz_i, ie) - ranges::begin(inv_sahz_i),
            jb, je
        );
    }

};