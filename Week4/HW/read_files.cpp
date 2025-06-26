/**
 * @file
 * Week4/HW/create_graph.cpp
 * input(wikipedia_dataset)からグラフを作成するプログラム
 * 
 * @brief
 * 普通に読み込むだけ
 * Page(ノード)の情報を構造体にまとめ、そこからLinksを作る丁寧な仕様
 * 使いづらかったらもう少し平面的な構造にする
 * 
 * @note
 * 少し冗長な気もする
 * 使いづらかったらもう少し平面的な構造にする
 * 
 * @todo
 * - HW3で使うので、最終的にはFold-Fulkersonっぽく逆流可能なグラフを作りたい
 * 
 */


#include <bits/stdc++.h>
using namespace std;


/// @brief ノード(Page)の構造体
/// @param ll id
/// @param string title
/// @param double pagerank
/// @param bool visited
struct Page {
    /// @brief ページのID
    long long id;

    /// @brief ページのタイトル
    string title;

    /// @brief ページランク
    double pagerank;

    /// @brief そのページを通ったか否か
    bool visited = false;
};


/*
 
 /// @brief Page名からIDを取得できるマップ
 /// @note title_to_id[page_title] = id
 map<string, long long> title_to_id;

 OH:
 title_to_id はたかだか2回しか呼び出さないので、mapで十分
 時間計算量よりも空間計算量の方が重い よく考えるとコスパが悪い
 つい競プロの手癖でマップを用意してしまったが、よく考えると要らなかったかも
 モジュール化みたいな考え方がまだ微妙
 
*/

/// @brief PageのIDからPage情報を取得できるマップ
/// @note id_to_page[id] = Page
map<long long, Page> id_to_page;


void read_pages(const string& filename) {
    ifstream ifs(filename);
    long long id;
    string title;
    while (ifs >> id >> title) {
        title_to_id[title] = id;
        Page p;
        p.id = id;
        p.title = title;
        p.pagerank = 1.0;
        p.visited = false; 
        id_to_page[id] = p;
    }
    return;
}


/// @brief Linkの集合
/// @note id_to_ids[pos.id] のなかに to.id がたくさん入ってる
map<long long, set<long long>> id_to_ids;

// 数えるだけ 外に出さない
long long count_links = 0;


void read_links(const string& filename) {
    ifstream ifs(filename);
    long long pos, to;
    while (ifs >> pos >> to) {
        id_to_ids[pos].insert(to);
        count_links++;
    }
    return;
}


/// @brief 入力ファイルを指定してもらい、id_to_pageとtitle_to_idとLinksを作成する関数
void read_files() {
    string size;
    cout << "Select dataset size (small, medium, large): ";
    cin >> size;
    string base = "input/wikipedia_dataset/";
    string pages_file = base + size + "/pages.txt";
    string links_file = base + size + "/links.txt";

    read_pages(pages_file);
    read_links(links_file);

    cout << "Loaded " << id_to_page.size() << " pages and " << count_links << " links." << endl;
    return;
}

