/**
 * @file
 * hw.h
 * 
 * @brief
 * 関数一覧表
 * 
 * @note
 * - 
 * 
 * @todo
 * - 
 * 
 */

#include <bits/stdc++.h>
using namespace std;
#include "io.h"  // 入力・出力のヘッダーファイル


/* 01_simulated-annealing */

/// @brief 問題を貪欲法で解く関数
/// @param vector<Point> cities
/// @param int start スタート地点のindex
/// @return Route ret_route
Route solve_greedy(const vector<City>& cities, const int& start_index);

/// @brief 配達先の訪問順序を焼きなまし法で改善する関数
/// @param vector<City> cities 入力データ
/// @param Route greedy_route 貪欲法で求めたRoute
/// @return Route output_route 焼きなまし法で改善されたRoute
Route solve_simulated_annealing(const vector<City>& cities, const Route& greedy_route);