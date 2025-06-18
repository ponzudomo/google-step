/**
 * @file
 * Week4/HW/create_graph.cpp
 * input(wikipedia_dataset)からグラフを作成するプログラム
 * 
 * @brief
 * 普通に読み込むだけ
 * Page(ノード), Link(エッジ)をそれぞれ構造体に入れ、そこからGraphを作る丁寧な仕様
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


/// @brief ノード(Page)同士を繋ぐエッジ(Link)の構造体
/// @note Graph[Page]にLinkを置いていきたい(posはindexにしたい)ので、ひとまず行き先だけ
struct Link {
    Page to;

};

struct Page {
    long long id;
    string title;
};

struct Graph {

};

Graph Read_files() {

}