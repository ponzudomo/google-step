/**
 * @file
 * Week3/excercise.cpp
 * 
 * @brief
 * 足し算引き算がのみができる電卓
 *
 * @note
 * 詳細はproblem_statement.mdを参照
 * 左から線形に足し算をしていく
 * 
 * @todo
 * - count_doubleに整数判定の機能を持たせてしまったけど、使い方が非自明かつコードの可読性が落ちている
 *  → count_doubleは小数点以下の桁数を表すものにして、整数判定は別のフラグを用意して方が良さそう
 */


// include
#include <bits/stdc++.h>
using namespace std;

// typedef
typedef long long ll;
typedef unsigned long long ull;

struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); } }init;

/// @struct Term
/// @brief 式の項ひとつぶん
/// @param posi_nega 正負のフラグ (1 or -1)
/// @param integer 小数点を無視した場合の数字
/// @param count_double 小数点以下の桁数 兼 整数判定 (初期値は-1)
struct Term {
    /// @brief posi_nega 正負のフラグ (1 or -1)
    int posi_nega;
    /// @brief integer 小数点を無視した場合の数字
    ll integer;
    /// @brief count_double 小数点以下の桁数 兼 整数判定 (整数なら-1, 小数なら0以上)
    int count_double;

    Term(int posi_nega, ll integer, int count_double) 
        : posi_nega(posi_nega), integer(integer), count_double(count_double) {}
};

/// @brief 二つ項を並べて、計算する関数
/// @param ans 現在の計算結果
/// @param current 現在の項
/// @return 計算結果を返す
Term calc_each(Term ans, Term current) {
    Term ret = ans;
    /* 筆算で小数点の位置を揃えるのと同じ感覚で作業を行う */
    if(ans.count_double > current.count_double) {
        ret.integer += current.integer * pow(10, ans.count_double - max(current.count_double, 0)) * current.posi_nega;
    } else {
        ret.integer *= pow(10, max(current.count_double, 0) - max(ans.count_double, 0));
        ret.integer += current.integer * current.posi_nega;
        ret.count_double = current.count_double;
    }
    /* 計算後に減らせそうな情報は減らす: 0.32+0.28=0.60 を 0.6 にする作業 */
    while(ret.integer % 10 == 0 && ret.count_double != -1) {
        ret.integer /= 10;
        ret.count_double--;
        if(ret.count_double == 0) {
            ret.count_double = -1; // 小数点以下がなくなったら整数判定のフラグを立てる(-1にする)
        }
    }
    cerr << ans.integer;
    if(ans.count_double != -1) cerr << " * 10^(-" << ans.count_double << ")";
    cerr << (current.posi_nega == 1 ? " + " : " - ") << current.integer;
    if(current.count_double != -1) cerr << " * 10^(-" << current.count_double << ")";
    cerr << " = " << ret.integer;
    if(ret.count_double != -1) cerr << "* 10^(-" << ret.count_double << ')';
    cerr << endl;
    return ret;
}

/// @brief 与えられた式を計算する関数
/// @param formula 計算する式を表す文字列
/// @return 計算結果を文字列で返す (数字より小数点の挿入が簡単そうなので)
string calculate(const string &formula) {
    Term ans(1, 0, -1), current(1, 0, -1);
    cerr << "Start cauculation: '" << formula << "'..." << endl;
    for(char c : formula + '+') { // 式の最後に演算子を追加しておくと追加処理が必要なくて楽
        if(isdigit(c)) {
            /* 数字が続く限り、現在の項に追加していく */
            current.integer = current.integer * 10 + (c - '0');
            if(current.count_double != -1) {
                current.count_double++;
            }
        } else if(c == '.') {
            /* 小数点が出てきたら、count_doubleの整数フラグを外す */
            current.count_double = 0;
        } else {
            /* 演算子が出てきたら、現在の項を計算結果に加える */
            ans = calc_each(ans, current);
            /* 現在の項をリセット */
            current = Term((c == '+' ? 1 : -1), 0, -1);
        }
    }
    string ret = to_string(ans.integer);
    if(ans.count_double != -1) {
        /* 小数点以下がある場合は、count_doubleの値を使って小数点を挿入 */
        ret.insert(ret.size() - ans.count_double -1, ".");
    }
    cerr << "Final result: " << ret << endl;
    return ret;
}

int main() {
    string formula;
    while(cin >> formula) {
        string result = calculate(formula);
        cout << result << endl; // 結果を出力
    }
    return 0;
}