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

const int inf = 1073741823;


/// @brief 問題を貪欲法で解く関数
/// @param vector<Point> cities
/// @param int start スタート地点のindex
/// @return Route ret_route
Route solve_greedy(const vector<City>& cities, const int& start_index) {
    /* デバッグ用出力 */
    cerr << "=== Greedy Solution ===" << endl;

    vector<int> ret_order;
    int city_count = cities.size();

    /* startからスタート */
    ret_order.push_back(start_index);
    City current_city = cities[start_index]; // 現在地
    int total_dist = 0; // 総移動距離

    /* 訪問箇所がcity_count箇所に達するまで、今いる場所から一番近いCityに移動することを繰り返す */
    vector<bool> visited_cities(city_count, false);
    visited_cities[start_index] = true; // スタート地点は訪問済み

    // city_count回ループ
    for (int i = 0; i < city_count; i++) {
        int nearest_index = 0; // 最も近いCityのindex
        double min_dist = double(inf); // 最も近いCityの距離

        for (int j = 0; j < city_count; j++) {
            // 未訪問のCityのみ訪問
            if(visited_cities[j]) continue;

            // 最短距離が更新されたら記録
            double current_dist = current_city.dist(cities[j]);

            if (current_dist < min_dist) {
                min_dist = current_dist;
                nearest_index = j;
            }
        }

        // 最も近いCity(City[nearest_index])に移動する
        // 現在位置を最も近いCityに更新
        current_city = cities[nearest_index];

        // 訪問ルートに現在の位置を追加
        ret_order.push_back(nearest_index);

        // 訪問済みレストランの配列にtrueをセット
        visited_cities[nearest_index] = true;

        // 総移動距離の更新
        total_dist += min_dist;

        // デバッグしやすいよう、標準エラー出力に訪問先を出力
        cerr << i << "番目の訪問先: " << nearest_index << " = (" << current_city.x << ", "
                  << current_city.y << ")" << endl;
    }

    /* 合計距離を標準エラー出力に出力 */
    cerr << "total distance: " << total_dist << endl;

    return Route(total_dist, ret_order);
}


/// @brief 配達先の訪問順序を焼きなまし法で改善する関数
/// @param vector<City> cities 入力データ
/// @param Route greedy_route 貪欲法で求めたRoute
/// @return Route output_route 焼きなまし法で改善されたRoute
Route solve_simulated_annealing(const vector<City>& cities, const Route& greedy_route) {
    /* 準備 */

    /* 初期解を分解 */
    vector<int> current_order = greedy_route.order;
    double current_dist = greedy_route.distance;
    int city_count = cities.size();

    /* 乱数生成器を用意 */
    // 乱数のシード値は固定のものにしておくと、デバッグがしやすくなるんだそう
    mt19937 rand{42};
    uniform_real_distribution<double> zero_one_dist(0.0, 1.0);

    /* 時間管理 */
    // nowで取得したやつは比較できないっぽい？
    auto start_time = chrono::system_clock::now();
    const int time_limit = 5 * 60 * 1000; // 5分をミリ秒に変換

    /* 温度管理 */
    const double start_temperature = 5e3;
    const double end_temperature = 1e0;
    double current_temperature = start_temperature;

    // 試行回数
    int iteration = 0;

    /* 焼きなます */
    while (true) {
        /* 時間管理 */
        auto current_time = chrono::system_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count() >= time_limit) {
            break;
        }

        /* 2-opt */

        // 訪問先が配達先であるようなインデックスの中から i, j をランダムに選ぶ
        // 末尾(=後に何も続かない)だと困るので、modはcity_count-1でとうr
        int order_index_1 = rand() % (city_count-1); // route内での入れ替え位置
        int order_index_2 = rand() % (city_count-1); // route内での入れ替え位置

        // 変更前と変更後の結果を比較 (比較対象が同じだった場合はスキップ)
        if (order_index_1 == order_index_2) continue;
        int cities_index_1 = current_order[order_index_1]; // 入れ替えるCityの番号
        int cities_index_2 = current_order[order_index_2]; // 入れ替えるCityの番号
        City city_1 = cities[cities_index_1]; // 入れ替えるCity
        City city_2 = cities[cities_index_2]; // 入れ替えるCity

        double new_dist = current_dist - city_1.dist(cities[cities_index_1+1]) // i番目の結合を切り離す
                   - city_2.dist(cities[cities_index_2+1]) // j番目の結合を切り離す
                   + city_1.dist(cities[cities_index_2+1]) // i番目のCityとj番目の結合先をくっつける
                   + city_2.dist(cities[cities_index_1+1]); // j番目のCityとi番目の結合先をくっつける

        /* 焼きなまし */
        // 操作後の距離が操作前以下なら採用する
        // 操作前より悪化していても、確率で採用する(悪化度合いが小さく、温度が高いほど採用されやすい)
        // 採用確率(0.0以上1.0未満)は exp((current_dist - new_dist) / current_temperature) = e^(-落差/温度)で計算
        // zero_one_dist(rand) と書くと、0.0以上1.0未満の乱数が得られる
        if (new_dist <= current_dist || zero_one_dist(rand) < exp((current_dist - new_dist) / current_temperature)) {
            // 採用されたら、現在の経路と距離を更新
            current_dist = new_dist;
            current_order.erase(current_order.begin() + order_index_1);
            current_order.insert(current_order.begin() + order_index_2, cities_index_1);
        } // else {
            // 採用されなかったら何もしない
            
        // }

        // 試行回数のカウントを増やす
        iteration++;

        /* 温度管理 */
        // 現在の経過時間の割合を計算する
        double progress = (double) chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count() / (double) time_limit;
        // 温度の更新
        // 現在の経過時間の割合に対する温度は pow(start_temperature, 1.0 - progress) * pow(end_temperature, progress) で計算できる
        current_temperature = pow(start_temperature, 1.0 - progress) * pow(end_temperature, progress);
    }

    /* 試行回数と合計距離を標準エラー出力に出力 */
    cerr << "--- Result ---" << endl;
    cerr << "iteration: " << iteration << endl;
    cerr << "total distance: " << current_dist << endl;

    return Route(current_dist, current_order);
}