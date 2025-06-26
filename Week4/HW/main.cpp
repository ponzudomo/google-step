/**
 * @file
 * Week4/HW/main.cpp
 * 
 * @brief
 * 
 * 
 * @note
 * 
 * 
 * @todo
 * - 
 * 
 */


// include
#include <bits/stdc++.h>
using namespace std;
#include <boost/dynamic_bitset.hpp>
using namespace boost;
#include "read_files.h"
#include "HW.h"

// define
#define fore(x, a) for (auto &x : a)
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define repp(i, m, n) for (int i = (int)(m); i < (int)(n); i++)
#define all(v) v.begin(), v.end()
#define rall(v) v.rbegin(), v.rend()

// typedef
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<string> vs;

const double pi = 3.141592653589793238;
const int inf = 1073741823;
const ll infl = 1LL << 60;
const int mod = 998244353;
struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); } }init;



int main() {
    // 入力ファイル選択・グラフ作成
    read_files();
    // 最短経路探索
    find_shortest_path("A", "F");
    return 0;
}