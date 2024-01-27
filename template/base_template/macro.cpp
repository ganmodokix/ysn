// #REQ: base_template/575 base_template/ll
// 汎用マクロ
#define ALL(x) (::std::ranges::begin(x)), (::std::ranges::end(x))
#define ALLOF(x) (::std::ranges::begin(x)), (::std::ranges::end(x))
#define RALL(x) (::std::ranges::rbegin(x)), (::std::ranges::rend(x))
#define RALLOF(x) (::std::ranges::rbegin(x)), (::std::ranges::rend(x))
#define RPE REP
#define ERP REP
#define EPR REP
#define PER REP
#define PRE REP
#define REP(i,n) for (ll i=0, i##_len=(n); i<i##_len; i++)
#define RANGE(i,is,ie) for (ll i=(is), i##_end=(ie); i<=i##_end; i++)
#define DSRNG(i,is,ie) for (ll i=(is), i##_end=(ie); i>=i##_end; i--)
#define STEP(i, is, ie, step) for (ll i=(is), i##_end=(ie), i##_step = (step); i<=i##_end; i+=i##_step)
#define UNIQUE(v) do { sort((v).begin(), (v).end()); (v).erase(unique((v).begin(), (v).end()), (v).end()); } while (false)
#define FOREACH(i,q) for (auto &i : q)
template<typename T, typename U> bool chmax(T &a, const U b) { if (a < b) {a = b; return true;} return false; }
template<typename T, typename U> bool chmin(T &a, const U b) { if (a > b) {a = b; return true;} return false; }
constexpr int INF = numeric_limits<int>::max();
constexpr ll LINF = numeric_limits<ll>::max();
#define Yes(q) ((q) ? "Yes" : "No")
#define YES(q) ((q) ? "YES" : "NO")
#define Possible(q) ((q) ? "Possible" : "Impossible")
#define POSSIBLE(q) ((q) ? "POSSIBLE" : "IMPOSSIBLE")
#define IIF(q,t,f) ((q) ? (t) : (f))