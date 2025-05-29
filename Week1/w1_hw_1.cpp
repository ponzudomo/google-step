/**
 * @file 
 * w1_hw_1.cpp
 * 
 * @brief
 * Checks whether the given string is an anagram of any word in the dictionary using a hash-based method.
 *
 * @note
 * w1_output_1.txt に「与えられた文字列」と「与えられた文字列を並び替えてできる単語」を書き込む
 * 並び替えても単語が成立しなかったものについては、何も書き込まない
 * 
 * @todo
 * ハッシュの衝突が起こっていてヤバい
 *  - 単語の長さが一致しているか確認するとか...？ ← やった ひとまずマシになった これで行ってみる
 *  - boostで多倍長整数を持ち出してみる...？ ← 最終手段
 * エラーハンドリングの部分は自分で書いてない　Copilotにコードレビュー頼んだら書き足してくれた
 *  - そういえばエラーの出し方とかお作法を全然知らない、勉強しておく
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


/// @brief 単語をハッシュ値に変換する関数
/// @param string random_word ハッシュ化する単語
/// @return ll ハッシュ値（アナグラムは同じハッシュ値を持つ）
/// @note 10進法でハッシュを取ったあと、998244353でmodをとる
ll hash_word(string random_word) {
    ll hashed_word = 0;

    // アナグラム検出のためには、単語中の各文字の出現回数が同じであることが重要
    // 単語内で同じ文字は9回までしか登場しないようなので、ひとまず各アルファベットの登場回数を10進法で一つのハッシュ値に入れちゃう
    // 26桁はlong longに入らないので、998244353でmodをとる ← これほんとに大丈夫？
    vll pow10(26); 
    pow10[0] = 1;
    rep(i, 26-1) {
        pow10[i+1] = pow10[i] * 10; // 25回なら愚直でも大丈夫でしょう 極端に文字の種類が多い言語なら繰り返し2乗法とか使ってもいい
        pow10[i+1] %= mod; // ハッシュ値が大きくなりすぎないようにmodを適用
    }

    fore(c, random_word) {
        hashed_word += pow10[c - 'a'];
        hashed_word %= mod;
    }
    
    return hashed_word;
}

/// @brief 辞書内の単語をハッシュ化する関数
/// @param vs dictionary 辞書
/// @return map<ll, set<ll>> ハッシュ値をキーとし、該当する単語のインデックスを値とするマップ
map<ll, set<ll>> hash_dictionary(vs dictionary) {
    /// @brief key = 辞書内の単語のハッシュ値, value = そのハッシュ値を持つ単語のdictionary内でのindex
    map<ll, set<ll>> hashed_dictionary;

    ll index = 0;
    fore(each_word, dictionary) {
        hashed_dictionary[hash_word(each_word)].insert(index); // ハッシュ値とdictionary内の住所を紐付け
        index++;
    }

    return hashed_dictionary;
}

/// @brief 与えられた単語のアナグラムを辞書から探す関数
/// @param string random_word 検索する単語
/// @param vs dictionary 辞書単語のリスト
/// @param map<ll, set<ll>> hashed_dictionary ハッシュ化された辞書
/// @return set<string> 見つかったアナグラム
/// @note ここでrandom_wordとdictionary[index]の文字数を比較しておくことで、ハッシュの衝突を少し予防
set<string> find_anagrams(string random_word, vs dictionary, map<ll, set<ll>> hashed_dictionary) {
    set<string> found_anagrams;
    
    ll hashed_word = hash_word(random_word);
    fore(index, hashed_dictionary[hashed_word]) {
        if(random_word.size() == dictionary[index].size())
            found_anagrams.insert(dictionary[index]);
    }

    return found_anagrams;
}

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
    string output_file = "output/w1_output_1.txt";
    
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
    
    // 辞書をハッシュ化
    cout << "Hashing dictionary..." << endl;
    map<ll, set<ll>> hashed_dictionary = hash_dictionary(dictionary);
    cout << "Dictionary hashed." << endl;
    
    // テスト対象の単語を読み込み
    vs random_words = load_files(input_file);
    if (random_words.empty()) {
        cerr << "Error: No words to check." << endl;
        return 1;
    }
    
    // 各単語についてアナグラムを検索し、結果を出力
    cout << "Searching for anagrams..." << endl;
    int found_count = 0;
    
    for (const string& random_word : random_words) {
        set<string> anagrams = find_anagrams(random_word, dictionary, hashed_dictionary);
        
        // アナグラムが見つかった場合のみ出力
        if (!anagrams.empty()) {
            outfile << random_word << ": ";
            bool first = true;
            for (const string& anagram : anagrams) {
                if (!first) outfile << ", ";
                outfile << anagram;
                first = false;
            }
            outfile << endl;
            found_count++;
        }
    }
    
    cout << "Found anagrams for " << found_count << " out of " << random_words.size() << " words." << endl;
    cout << "Results written to " << output_file << endl;
    
    return 0;
}
