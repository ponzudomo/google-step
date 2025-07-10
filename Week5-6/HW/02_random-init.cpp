/**
 * @file
 * 02_random-init.cpp
 * 
 * @brief
 * 焼きなます前の下処理を追加
 * 
 * @note
 * - まず輪になるように経路を生成
 * - GRASPでほんのり改善
 * - 一番長い辺を切り離す これを初期解とする
 * - 探し方はビームサーチ風味、ランダム化貪欲を上位5レーンくらい保持して走らせてみる
 * 
 * @todo
 * 
 */

#include <bits/stdc++.h>
using namespace std;
#include "io.h"  // 入力・出力のヘッダーファイル
#include "hw.h"  // 関数一覧表のヘッダーファイル

Route random_greedy(map<int, City> &cities) {
    /* デバッグ用出力 */
    cerr << "=== Random Greedy Solution ===" << endl;

    /* 前処理 */
    int city_count = cities.size(); // どうせ使うから書いたけど使わなかったら消す

    /*  */

    // 最長辺を見つけて切り離す
    double max_distance = -1.0;
    int max_index = -1;
    
    for (int i = 0; i < city_count; ++i) {
        int next_index = (i + 1) % city_count; // 循環するように次の都市を決定
        double distance = cities[order[i]].dist(cities[order[next_index]);
        if (distance > max_distance) {
            max_distance = distance;
            max_index = i;
        }
    }

    // 最長辺を切り離す
    vector<int> initial_order;
    for (int i = 0; i < city_count; ++i) {
        if (i != max_index && i != (max_index + 1) % city_count) {
            initial_order.push_back(order[i]);
        }
    }
    
    // 初期解として最長辺を切り離した巡回路を返す
    return Route(max_distance, initial_order);
}
