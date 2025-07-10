/**
 * @file
 * main.cpp
 * 
 * @brief
 * まとめて実行
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
#include "io.h"
#include "hw.h"

int main() {

    /* Challenge 0 から 6 までをループ */
    for (int i = 0; i <= 6; ++i) {
        string input_filename = "../materials/input_" + to_string(i) + ".csv";
        string output_filename = "../materials/output_" + to_string(i) + ".csv";

        cout << "--- Processing Challenge " << i << " ---" << endl;
        cout << "Input: " << input_filename << endl;

        /* 1. 都市データを読み込む */
        vector<City> cities = read_cities(input_filename);
        if (cities.empty()) {
            cerr << "Failed to read cities from " << input_filename << endl;
            continue; // 次のチャレンジへ
        }
        cout << "Loaded " << cities.size() << " cities." << endl;

        /* 2. 貪欲法で初期解を求める (スタート地点は0番目の都市とする) */
        Route greedy_route = solve_greedy(cities, 0);

        /* 3. 焼きなまし法で解を改善する */
        Route final_route = solve_simulated_annealing(cities, greedy_route);

        /* 4. 結果をファイルに書き出す */
        write_route(output_filename, final_route);
        cout << "Successfully wrote tour to " << output_filename << endl;
        cout << endl;
    }

    cout << "All challenges processed." << endl;

    return 0;
}
