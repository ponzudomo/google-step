/**
 * @file
 * Week3/HW1.cpp
 * 
 * @brief
 * 四則演算のできる電卓
 *
 * @note
 * 基本的に左から線形に見ていくのは変わらない
 * 項は三つずつ保持、左右の演算子の優先順位を確認しながら計算
 * *と/について、下手に逐一計算すると精度を落としかねないので、分子と分母を分けて保存
 * 小数点の位置についても同様、double_countの足し算引き算で管理し、最後に計算
 * 
 * ex) [ 1 - 2 + 3 ] * 4 - 5 * 6 * 7 / 8　← 演算子の優先順位に差がないので左を計算して詰める
 * 　=　[ -1 + 3 * 4 ] - 5 * 6 * 7 / 8　← 掛け算を優先的に計算して詰める
 * 　=　[ -1 + 12 - 5 ] * 6 * 7 / 8　← 右の演算子はさらに右隣の演算子とも比較しなきゃいけないので、原則左から計算
 * 　=　[ 11 - 5 * 6 ] * 7 / 8
 * 　=　[ 11 - 30 * 7 ] / 8
 * 　=　[ 11 - 210 / 8]
 * 　=　[ 11 - 210/8 ]　← 約分はコストがかかるので今はやらない(count_doubleでのみ行う)
 * 　=　[ 88/8 - 210/8 ]　← 通分
 * 　=　[ -122 / 8 ]　← ようやくdoubleに入れる
 * 　=　-15.25 🆗
 * 
 * @todo
 * - オーバーフロー判定
 * - オーバーフローしそうだったら約分
 * - calculate関数にデバッグ用のcerr出力を入れる
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
/// @param symbol 演算子 '+' → 00(0), '-' → 01(1), '*' → 10(2), '/' → 11(3)
/// @param numerator 分子
/// @param denominator 分母
/// @param is_double 小数か否か
/// @param count_double 小数点以下の桁数
struct Term {
    /// @brief 演算子 '+' → 00(0), '-' → 01(1), '*' → 10(2), '/' → 11(3)
    int symbol;
    /// @brief numerator 分子
    ll numerator;
    /// @param denominator 分母
    ll denominator;
    /// @brief is_double 小数か否か
    bool is_double;
    /// @brief count_double 小数点以下の桁数 10^-n のnの部分
    int count_double;

    Term(int symbol, ll numerator, ll denominator, bool is_double, int count_double) 
        : symbol(symbol), numerator(numerator), denominator(denominator), 
          is_double(is_double), count_double(count_double) {}
};

/// @brief 二つ項を並べて計算する関数 関数としては計算の最小単位
/// @param left 
/// @param right 
/// @return Term 計算後の値
Term calc_two(Term left, Term right) {
    /* 小数点の位置を揃える */
    if(left.count_double > right.count_double) {
        right.numerator *= pow(10, left.count_double - right.count_double);
        right.count_double = left.count_double;
        right.is_double = left.is_double || right.is_double;
    } else {
        left.numerator *= pow(10, right.count_double - left.count_double);
        left.count_double = right.count_double;
        left.is_double = left.is_double || right.is_double;
    }
    /* 演算子ごとに計算 */
    if(right.symbol < 2) { // leftとmiddleの間の演算子(middle.symbol)が+or-
        /* まずは通分 */
        if(left.denominator != right.denominator) {
            left.numerator *= right.denominator;
            right.numerator *= left.denominator;
            left.denominator *= right.denominator;
            right.denominator = left.denominator; // 通分したので分母を揃える
        }
        /* +か-の計算 */
        left.numerator += (right.symbol == 0 ? 1 : -1) * right.numerator;
    }
    else {
        if(right.symbol == 3) { // 割り算だった場合
            /* 分子分母を逆にする */
            swap(right.numerator, right.denominator);
            /* 小数点の位置は揃えてあるので相殺できる */
            left.is_double = false;
            left.count_double = 0;
        } else { // 掛け算だった場合
            /* 小数点の位置は揃えてあるので、二倍するだけで良い */
            left.count_double *= 2;
        }
        /* あとはかけるだけ */
        left.numerator *= right.numerator;
        left.denominator *= right.denominator;
    }
    return left;
}

/// @brief 3個ずつ項を並べて計算
/// @param left_middle
/// @param right
/// @return 3個 → 2個 に減らしてpairに入れて返す
pair<Term, Term> calc_three(pair<Term, Term> left_middle, Term right) {
    auto [left, middle] = left_middle;

    /* 計算順序の観点から、まず+と-(弱い演算子)、*と/(強い演算子)に分けたい */
    /* +(00)と-(01)、*(10)と/(11) はそれぞれ1ビット目が同じなので、XORでふるいにかけることができる */
    if(middle.symbol > 1 || middle.symbol ^ right.symbol < 2) { // 左の演算子が強い || 両方弱くて優先順位が同じ
        /* leftとmiddleを計算 */
        return {calc_two(left, middle), right};
    } else { // 左が弱くて右が強いとき
        /* rightとmiddleを計算 */
        return {left, calc_two(middle, right)};
    }
}

/// @brief 与えられた式を計算する関数
/// @param formula 計算する式を表す文字列
/// @return 計算結果を小数で返す
double calc_all(const string &formula) {
    pair<Term, Term> left_middle(Term(0, 0, 1, false, 0), Term(0, 0, 1, false, 0));
    Term right(0, 0, 1, false, 0);
    cerr << "Start cauculation: '" << formula << "'..." << endl;
    for(char c : formula + '+') { // 式の最後に演算子を追加しておくと追加処理が必要なくて楽
        if(isdigit(c)) {
            /* 数字が続く限り、現在の項に追加していく */
            right.numerator = right.numerator * 10 + (c - '0');
            if(right.is_double) {
                right.count_double++;
            }
        } else if(c == '.') {
            /* 小数点が出てきたら、count_doubleの整数フラグを外す */
            right.is_double = true;
        } else {
            /* デバッグ用の出力をここに書く */

            /* 演算子が出てきたら、現在の項を計算結果に加える */
            left_middle = calc_three(left_middle, right);
            /* 現在の項をリセット */
            right = Term((c == '+' ? 0 : (c == '-' ? 1 : (c == '*' ? 2 : 3))), 0, 1, false, 0);
        }
    }
    /* 最後に残った項を計算 */
    Term calculated_term = calc_two(left_middle.first, left_middle.second);
    double calculated_value = static_cast<double>(calculated_term.numerator) / calculated_term.denominator;
    calculated_value /= pow(10, calculated_term.count_double); // 小数点の位置を調整
    cerr << "Final result: " << calculated_value << endl;
    return calculated_value;
}

int main() {
    string formula;
    while(cin >> formula) {
        auto result = calc_all(formula);
        cout << result << endl; // 結果を出力
    }
    return 0;
}