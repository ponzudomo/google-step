/**
 * @file
 * io_example.cpp
 * 
 * @brief
 * Geminiに入出力のやり方を教えてもらいました
 * 
 * @note
 * 一切自分で書いていません
 * これを元にio.cppを書いています
 * 
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// 都市の座標を格納するための構造体
struct City {
    double x;
    double y;
};

// 入力CSVファイルを読み込み、都市の座標のリストを返す関数
std::vector<City> read_cities(const std::string& filename) {
    std::vector<City> cities;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open input file " << filename << std::endl;
        return cities;
    }

    // 1行目（ヘッダー "x,y"）を読み飛ばす
    std::getline(file, line);

    // 2行目以降を読み込み、都市の座標を解析する
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        City city;

        // x座標を読み込む
        std::getline(ss, value, ',');
        city.x = std::stod(value);

        // y座標を読み込む
        std::getline(ss, value, ',');
        city.y = std::stod(value);

        cities.push_back(city);
    }

    file.close();
    return cities;
}

// 計算結果の巡回ルート（都市のインデックスのリスト）を出力CSVファイルに書き込む関数
void write_tour(const std::string& filename, const std::vector<int>& tour) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open output file " << filename << std::endl;
        return;
    }

    // 1行目にヘッダー "index" を書き込む
    file << "index" << std::endl;

    // 2行目以降に巡回ルートのインデックスを書き込む
    for (int index : tour) {
        file << index << std::endl;
    }

    file.close();
}

int main() {
    // --- 入力処理の例 ---
    // Challenge 0 の入力ファイルを読み込む
    std::string input_filename = "../materials/input_0.csv";
    std::vector<City> cities = read_cities(input_filename);

    if (cities.empty()) {
        return 1; // ファイル読み込みに失敗した場合
    }

    // 読み込んだ都市の情報を表示（確認用）
    std::cout << "Loaded " << cities.size() << " cities:" << std::endl;
    for (size_t i = 0; i < cities.size(); ++i) {
        std::cout << "City " << i << ": (" << cities[i].x << ", " << cities[i].y << ")" << std::endl;
    }

    // --- 出力処理の例 ---
    // ダミーの巡回ルートを作成（例：0 -> 1 -> 2 -> 3 -> 4）
    std::vector<int> dummy_tour;
    for (size_t i = 0; i < cities.size(); ++i) {
        dummy_tour.push_back(i);
    }

    // ダミーの巡回ルートを出力ファイルに書き込む
    std::string output_filename = "output_0_cpp.csv";
    write_tour(output_filename, dummy_tour);

    std::cout << "\nSuccessfully wrote dummy tour to " << output_filename << std::endl;

    return 0;
}
