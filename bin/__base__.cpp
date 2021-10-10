// May this submission get accepted
#include <bits/stdc++.h>

// エイリアス
using  ll = long signed long;
using ull = long unsigned long;
using  ld = long double;
using namespace std;

// AtCoder/Codeforces 用 デバッグ検知
#ifdef ONLINE_JUDGE
constexpr bool DEBUG_MODE = false;
#else
constexpr bool DEBUG_MODE = true;
#endif

// エイリアス (補完・コンパイルが重くなる)
// #include <boost/multiprecision/cpp_int.hpp>
// using mll = boost::multiprecision::cpp_int;

// 汎用マクロ
#define ALL(x) (begin(x)), (end(x))
#define RALL(x) (rbegin(x)), (rend(x))
#define RPE REP
#define ERP REP
#define EPR REP
#define PER REP
#define PRE REP
#define REP(i,n) for (long long i=0, i##_len=(n); i<i##_len; i++)
#define RANGE(i,is,ie) for (long long i=(is), i##_end=(ie); i<=i##_end; i++)
#define DSRNG(i,is,ie) for (long long i=(is), i##_end=(ie); i>=i##_end; i--)
#define STEP(i, is, ie, step) for (long long i=(is), i##_end=(ie), i##_step = (step); i<=i##_end; i+=i##_step)
#define UNIQUE(v) do { sort((v).begin(), (v).end()); (v).erase(unique((v).begin(), (v).end()), (v).end()); } while (false)
#define FOREACH(i,q) for (auto &i : q)
template<typename T, typename U> bool chmax(T &a, const U b) { if (a < b) {a = b; return true;} return false; }
template<typename T, typename U> bool chmin(T &a, const U b) { if (a > b) {a = b; return true;} return false; }
constexpr int INF = numeric_limits<int>::max();
constexpr long long LINF = numeric_limits<long long>::max();
constexpr long double EPS = 1e-10L;
#define Yes(q) ((q) ? "Yes" : "No")
#define YES(q) ((q) ? "YES" : "NO")
#define Possible(q) ((q) ? "Possible" : "Impossible")
#define POSSIBLE(q) ((q) ? "POSSIBLE" : "IMPOSSIBLE")
#define IIF(q,t,f) ((q) ? (t) : (f))
#define DUMP(q) DUMP_FUNC(q, #q, __FILE__, __LINE__)
template <typename T> void DUMP_PROC(T x) { if (is_integral<T>() || is_floating_point<T>()) cerr << "\e[32m" << x << "\e[m"; else cerr << x; }
template<> void DUMP_PROC<char>(char x) { cerr << "\e[36m\'" << x << "\'\e[m"; }
template<> void DUMP_PROC<string>(string x) { cerr << "\e[33m\"" << x << "\"\e[m"; }
template <typename T, typename U> void DUMP_PROC(pair<T, U> x) { cerr << "{"; DUMP_PROC(x.first); cerr << ", "; DUMP_PROC(x.second); cerr << "}"; }
template <typename ...T, typename U, U... Seq> void DUMP_PROC(tuple<T...> &x, integer_sequence<U, Seq...>) { (void)(int[]){(cerr << ((const char*[]){"", ", "})[!!Seq] << (DUMP_PROC(get<Seq>(x)), ""), 0)...}; }
template <typename ...T> void DUMP_PROC(tuple<T...> x) {cerr << "{"; DUMP_PROC(x, index_sequence_for<T...>()); cerr << "}";}
template <typename T> void DUMP_PROC(vector<T> x) { cerr << "["; for (auto &xi : x) { DUMP_PROC(xi); cerr << (&xi != &*x.rbegin()?", ":""); } cerr << "]"; }
template <typename T> void DUMP_FUNC(T x, const char* name, const char* fn, int ln) { cerr << "\e[32m[DEBUG]\e[m " << name << ": "; DUMP_PROC(x); cerr << " @ " << fn << "(" << ln << ")" << endl; }
#define DUMPT(...) DUMP(tuple(__VA_ARGS__))

// gcc拡張マクロ
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll

// 標準入出力
struct qin { // query input
    size_t sz;
    qin(size_t _sz = 1) : sz(_sz) {}
    template <typename T> operator T () const { T a; cin >> a; return a; }
    template <typename T> operator vector<T> () const { vector<T> a(sz); for (size_t i = 0; i < sz; i++) cin >> a[i]; return a; }
    template <typename T, typename U> operator pair<T, U> () const { T f; U s; cin >> f >> s; return pair<T, U>(f, s); }
};
qin in1; // input one
template <typename T> void say(const T x, const char* end = "\n") { cout << x << end; }
void say(const ld x, const char* end = "\n") { cout << setprecision(30) << x << end; }
template <typename T> void say(const vector<T> x, const char* sep = " ", const char* end = "\n") { REP(i, x.size()) { cout << x[i] << (i+1 == i_len ? end : sep); } }
template <typename T> void say(const vector<vector<T>> x, const char* sep = " ", const char* end = "\n") { REP(i, x.size()) { say(x[i], sep, end); } }

// モジュール
// #REQ:
// #END

// 処理内容
int main() {
    
    ios::sync_with_stdio(false); // stdioを使うときはコメントアウトすること
    cin.tie(nullptr);            // インタラクティブ問題ではコメントアウトすること
    
    
    
}