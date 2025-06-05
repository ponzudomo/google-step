/**
 * @file
 * w1_hw_2.cpp
 * 
 * @brief
 * 与えられた文字列から好きなだけ文字を選び、並び替え、辞書内の単語と一致したアナグラムのうちスコアが最も高いものを出力
 *
 * @note
 * 辞書にもうスコアを記録しちゃう
 * それぞれの単語について、文字の出現回数をa-z分26種類管理
 *  → 26種類のアルファベット全てにおいて、出現回数がrandom_wordを下回る辞書内の単語を探す
 * そこからスコアが最も高いものを出力
 * 
 * @todo
 * 方針が固まりきっていない
 * - 構造体の使い方がよくわかっていなさそうなので、他の人のコードを参考にひとまず疑似コードを書いてみる
 */


// include
#include <bits/stdc++.h>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

// define
#define fore(x, a) for (auto &x : a)
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define repp(i, m, n) for (int i = (int)(m); i < (int)(n); i++)
#define all(v) v.begin(), v.end()
#define rall(v) v.rbegin(), v.rend()

// typedef
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<string> vs;

const double pi = 3.141592653589793238;
const int inf = 1073741823;
const ll infl = 1LL << 60;
const int mod = 998244353;
struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); } }init;

/// @struct Word
/// @brief 単語を表す構造体
/// @note actual_word(string), score_of_word(int), count_char(vector<int>)
struct Word {
    string actual_word; // 単語そのもの
    int score_of_word;
    // map<char, int> count_char;
    /// @brief 文字の出現回数を累積和で管理
    vector<int> count_char; 

    Word(int score_of_word, vector<int> count_char) : score_of_word(score_of_word), count_char(count_char) {}

    /// @brief スコアを計算する関数
    /// @param map<char, int> &scores_of_char 各文字のスコアを格納したマップ
    /// @note 辞書の単語に含まれる文字の出現回数と、scores_of_charを掛け合わせてスコアを計算
    /// @return int score_of_word
    int calculate_score(vector<int> &scores_of_char) {
        int score = 0;
        rep(i, 26) { // a-zの文字数分ループ
            score += scores_of_char['a'+i] * count_char[i]; // 各文字のスコアを計算
        }
        return score;
    }


};

/// @brief 入力ファイルから単語を読み込む関数
/// @param string filepath 入力ファイルのパス
/// @return vs 単語リスト
vs load_files(const string &filepath) {
    vs words;
    ifstream input_file(filepath);
    
    if (!input_file.is_open()) {
        cerr << "Error: Could not open input file: " << filepath << endl;
        return words;
    }
    
    string word;
    while (input_file >> word) {
        words.push_back(word);
    }
    input_file.close();
    if (words.empty()) {
        cerr << "Warning: No words found in input file." << endl;
    }

    return words;
}


int main(int argc, char* argv[]) {
    // コマンドライン引数の確認
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <dictionary_file> <input_file>" << endl;
        cerr << "Example: " << argv[0] << " input/words.txt input/small.txt" << endl;
        return 1;
    }

    // 辞書ファイルとテスト単語ファイルのパス
    string dict_file = argv[1];
    string input_file = argv[2];
    
    // 出力ファイルのパスを生成
    string output_file = "output/w1_output_2.txt";
    
    // 出力ディレクトリを確保
    try {
        path out_path(output_file);
        if (out_path.has_parent_path() && !exists(out_path.parent_path())) {
            create_directories(out_path.parent_path());
        }
    } catch (const filesystem_error& e) {
        cerr << "Error creating output directory: " << e.what() << endl;
        return 1;
    }
    
    // 出力ファイルを開く
    ofstream outfile(output_file);
    if (!outfile.is_open()) {
        cerr << "Error: Could not open output file: " << output_file << endl;
        return 1;
    }
    
    // 辞書を読み込み
    vs dictionary = load_files(dict_file);
    if (dictionary.empty()) {
        cerr << "Error: Dictionary is empty." << endl;
        return 1;
    }

    // テスト対象の単語を読み込み
    vs random_words = load_files(input_file);
    if (random_words.empty()) {
        cerr << "Error: No words to check." << endl;
        return 1;
    }

    /// @brief 文字をindexとして点数を調べられたら便利そうだなと思いました
    map<char, int> scores_of_char;
    vector<vector<char>> tmp = {{'a', 'e', 'h', 'i', 'n', 'o', 'r', 's', 't'},
                                {'c', 'd', 'l', 'm', 'u'},
                                {'b', 'f', 'g', 'p', 'v', 'w', 'y'},
                                {'j', 'k', 'q', 'x', 'z'}};
    rep(i, 4) fore(c, tmp[i]) scores_of_char[c] = i+1;

    // 

    return 0;
}
