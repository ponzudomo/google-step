/**
 * @file
 * Week4/HW/HW2.cpp
 * ページランクの計算, 出力
 * 
 * @brief
 * 見よう見まねでクラススライドのまま実装する
 * Random Surfer Modelを用いてランクを分配していく
 * 収束条件を作る
 *  ex) ∑(new_pagerank[i] - old_pagerank[i])^2 < 0.01
 * 
 * @note
 * ページランク、後から使うと思うのでどこかに保存しておきたい
 * 
 * @todo
 * - 収束条件いじってみる
 * 
 */

#include <bits/stdc++.h>
using namespace std;
