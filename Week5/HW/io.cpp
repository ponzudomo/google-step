/**
 * @file
 * io.cpp
 * 
 * @brief
 * 面倒なファイルの入出力を使い回すための関数群
 * 
 * @todo
 * - boostライブラリもうちょい調べる
 * 
 */

#include <bits/stdc++.h>
using namespace std;
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp> // is_any_of
using namespace boost;
#include "io.h"


/// @brief 入力CSVファイルを読み込み、都市の座標のリストを返す関数 
/// @param filename 入力ファイル名
/// @return vector<City>
vector<City> read_cities(const string& filename) {
    vector<City> cities;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error: Could not open input file " << filename << endl;
        return cities;
    }

    /* 1行目（ヘッダー "x,y"）を読み飛ばす */
    getline(file, line);

    /* 2行目以降を読み込み、都市の座標を解析する */
    while (getline(file, line)) {
        vector<string> city;
        algorithm::split(city, line, is_any_of(","));

        cities.emplace_back(stod(city[0]), stod(city[1]));
    }

    file.close();
    return cities;
}

/// @brief 計算結果の巡回ルート（都市のインデックスのリスト）を出力CSVファイルに書き込む関数
/// @param filename 出力ファイル名
/// @param route 巡回ルート
/// @note 出力ファイルは1行目にヘッダー "index" を書き込み、2行目以降に巡回ルートのインデックスを書き込む
void write_route(const string& filename, const Route& route) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open output file " << filename << endl;
        return;
    }

    // 1行目にヘッダー "index" を書き込む
    file << "index" << endl;

    // 2行目以降に巡回ルートのインデックスを書き込む
    for (int index : route.order) {
        file << index << endl;
    }

    file.close();
}