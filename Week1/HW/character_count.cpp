/**
 * @file 
 * character_count.cpp
 * 
 * @brief
 * inputとして用意されたファイルに含まれる
 *  - 要素数
 *  - 最長の単語の長さ
 *  - 各単語における、最頻アルファベットの出現数の最大値
 * をconstraints.txtに出力
 * 
 * @note
 * 宿題を解く前に、問題の制約を確認しておきたいだけ
 * 宿題の趣旨から逸れるのと、もう十分自力で書けるコードなのとで、Copilotにほとんど書いてもらっちゃいました
 */

#include <bits/stdc++.h>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

// ファイルを解析して情報を取得する関数
void analyze_file(const string &input_file, ofstream &outfile) {
    ifstream infile(input_file);
    if (!infile.is_open()) {
        cerr << "Error opening input file: " << input_file << endl;
        return;
    }

    int num_elements = 0;
    int max_word_length = 0;
    int max_freq = 0;
    string word;

    // Process each word as we read it
    while (infile >> word) {
        num_elements++;
        max_word_length = max(max_word_length, (int)word.size());
        
        array<int, 128> freq = {}; // ASCII文字用
        for (char c : word) {
            freq[c]++;
        }
        
        int word_max_freq = 0;
        for (int count : freq) {
            word_max_freq = max(word_max_freq, count);
        }
        
        max_freq = max(max_freq, word_max_freq);
    }

    // ファイル名のみを抽出（パスを除去）
    string filename = path(input_file).filename().string();
    
    // 区切り線を追加して見やすくする
    outfile << "=======================================" << endl;
    outfile << "File: " << filename << endl;
    outfile << "---------------------------------------" << endl;
    outfile << "Number of elements: " << num_elements << endl;
    outfile << "Longest word length: " << max_word_length << endl;
    outfile << "Maximum frequency of most frequent alphabet: " << max_freq << endl;
    outfile << endl;
}

int main() {
    vector<string> input_files = {
        "input/words.txt",
        "input/small.txt",
        "input/medium.txt",
        "input/large.txt",
    };
    
    string output_file = "constraints.txt";
    
    // 出力ディレクトリを確保
    try {
        path out_path(output_file);
        if (out_path.has_parent_path() && !exists(out_path.parent_path())) {
            create_directories(out_path.parent_path());
        }
    } catch (const filesystem_error& e) {
        cerr << "Error creating directories: " << e.what() << endl;
        return 1;
    }
    
    // 一つのファイルに全ての情報を出力
    ofstream outfile(output_file);
    if (!outfile.is_open()) {
        cerr << "Error opening output file: " << output_file << endl;
        return 1;
    }
    
    outfile << "# 各ファイルの制約情報" << endl;
    outfile << "各入力ファイルから抽出した制約条件です。" << endl << endl;
    
    for (const auto& input_file : input_files) {
        analyze_file(input_file, outfile);
    }
    
    cout << "All constraints have been written to " << output_file << endl;
    return 0;
}