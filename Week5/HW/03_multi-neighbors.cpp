/**
 * @file
 * 01_simulated-annealing.cpp
 * 
 * @brief
 * 貪欲法で初期解を求めた後、訪問順序を2-optの焼きなましで改善する
 * 始点と終点の指示がないので、全体を焼きなましの対象に
 * 
 * @note
 * - なんかもうちょっといいアルゴリズムありそうだけどまずはシンプルに実装
 * - とりあえず最初は5分焼きなます
 * 
 * Bibliography
 *  - AHFというイベントで以前使ったものをかなり参考にしています
 *  https://github.com/ponzudomo/AHF001/blob/main/03_simulated_annealing_answer.cpp
 *  - 鉄則本を読みました
 *  - 蟻本を読みました
 * 
 * @todo
 * - startの決め方をどうにかする
 * - 温度変えてみる
 * - 実行時間を変える
 * - nowで取得した時間がどういう形で格納されるのか調べる 
 * 　- (end_timeとcurrent_timeで比較できたら焼きなましの時間管理が楽だから)
 * 
 */

#include <bits/stdc++.h>
using namespace std;
#include "io.h"  // 入力・出力のヘッダーファイル
#include "hw.h"  // 関数一覧表のヘッダーファイル
