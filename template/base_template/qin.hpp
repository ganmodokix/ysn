// #REQ: base_template/575 base_template/ll base_template/macro
// 標準入出力
struct qin { // query input
    size_t sz;
    qin(size_t _sz = 1) : sz(_sz) {}
    template <typename T> operator T () const { T a; cin >> a; return a; }
    template <typename T> operator vector<T> () const { vector<T> a(sz); for (size_t i = 0; i < sz; i++) cin >> a[i]; return a; }
    template <typename T, typename U> operator pair<T, U> () const { T f; U s; cin >> f >> s; return pair<T, U>(f, s); }
};
qin in1; // input one
// input args
template <typename... Args> void ins(Args&... args) { (cin >> ... >> args); }
template <typename T> void say(const T x, const char* end = "\n") { cout << x << end; }
void say(const ld x, const char* end = "\n") { cout << setprecision(30) << x << end; }
template <typename T> void say(const vector<T> x, const char* sep = " ", const char* end = "\n") { REP(i, x.size()) { cout << x[i] << (i+1 == i_len ? end : sep); } }
template <typename T> void say(const vector<vector<T>> x, const char* sep = " ", const char* end = "\n") { REP(i, x.size()) { say(x[i], sep, end); } }