// #REQ: base_template/575 base_template/debug_mode
#define DUMP(q) DUMP_FUNC(q, #q, __FILE__, __LINE__)
template <typename T> void DUMP_PROC(T x) { if (is_integral<T>() || is_floating_point<T>()) cerr << "\e[32m" << x << "\e[m"; else cerr << x; }
template<> void DUMP_PROC<char>(char x) { cerr << "\e[36m\'" << x << "\'\e[m"; }
template<> void DUMP_PROC<string>(string x) { cerr << "\e[33m\"" << x << "\"\e[m"; }
template <typename T, typename U> void DUMP_PROC(pair<T, U> x) { cerr << "{"; DUMP_PROC(x.first); cerr << ", "; DUMP_PROC(x.second); cerr << "}"; }
template <typename ...T, typename U, U... Seq> void DUMP_PROC(tuple<T...> &x, integer_sequence<U, Seq...>) { (void)(int[]){(cerr << ((const char*[]){"", ", "})[!!Seq] << (DUMP_PROC(get<Seq>(x)), ""), 0)...}; }
template <typename ...T> void DUMP_PROC(tuple<T...> x) {cerr << "{"; DUMP_PROC(x, index_sequence_for<T...>()); cerr << "}";}
template <typename T> void DUMP_PROC(vector<T> x) { cerr << "["; for (auto &xi : x) { DUMP_PROC(xi); cerr << (&xi != &*x.rbegin()?", ":""); } cerr << "]"; }
template <typename T> void DUMP_FUNC(T x, const char* name, const char* fn, int ln) { if (!DEBUG_MODE) return; cerr << "\e[32m[DEBUG]\e[m " << name << ": "; DUMP_PROC(x); cerr << " @ " << fn << "(" << ln << ")" << endl; }
#define DUMPT(...) DUMP(tuple(__VA_ARGS__))