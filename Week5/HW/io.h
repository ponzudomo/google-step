/**
 * @file
 * io.h
 * 
 * @brief
 * 面倒なファイルの入出力を使い回すためのヘッダファイル
 * 
 */

#ifndef IO_H
#define IO_H

#include <bits/stdc++.h>
using namespace std;

/// @brief 2次元座標上の点を表す構造体
/// @note a.dist(city) でユークリッド距離を計算できる
struct City {
    double x;
    double y;

    City(double x, double y) : x(x), y(y) {}

    /// @brief 2点間のユークリッド距離を計算する
    /// @param city 距離を計算する点
    /// @return 2点間のユークリッド距離の2乗 (比較用なのでわざわざ平方根を取る必要がない)
    double dist(const City& city) const { return pow(x - city.x, 2.0) + pow(y - city.y, 2.0); }
};

/// @brief 訪問順序を表す構造体
/// @param double distance
/// @param vector<int> order 訪問順序のindex
struct Route {
    double distance; // 合計距離
    vector<int> order; // 訪問順序のindex

    Route(double distance, const vector<int>& order) : distance(distance), order(order) {}
};

vector<City> read_cities(const string& filename);

void write_route(const string& filename, const Route& route);

#endif // IO_H
