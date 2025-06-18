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
/// @note Graph[Page.id]にLinkを置いておきたい(リンク元のページはindexにしたい)ので、ひとまず行き先だけ
struct Link {
    Page to;
    long long flow_cap; // エッジに流せる残量 初期値1, フローを流したら0

    /// @brief 逆辺のGraph内のindex 
    long long rev_link_index;
    // Graph[Page.id][rev_link_index] = Link(to=Page, flow_cap=0, rev_link_index=index)
};

/// @brief ノード(Page)の構造体
/// @param 
struct Page {
    long long id;
    string title;
    double pagerank;
};

/// @brief 
/// @note Graph[Page_A.id][index] = ページAにおける、index番目のLink
struct Graph {

};

Graph Read_files() {

}