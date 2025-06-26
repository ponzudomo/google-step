/**
 * @file
 * Week4/HW/HW1.cpp
 * 始点と終点を指定されたときに、最短経路長を求めるプログラム
 * 
 * @brief
 * BFSで解く
 * 再帰が深いとエラーが出てしまうのでstackで管理
 * 
 * @note
 * とりあえず普通に実装するが、後々「作成済のstackを再利用する」仕組みを作りたい 何回も回すなら絶対そっちの方がいいと思う
 * 
 * @todo
 * - 作成済のstackを再利用できるようにプログラムをアップデート
 * - 繋がってなかった際のエラーハンドリングを追加
 * 
 */

#include <bits/stdc++.h>
#include "read_files.h"
using namespace std;


/// @brief shortest path を `output/HW1.txt` に出力する関数
/// @note BFSは確定済みの頂点をqueueに入れていく 経路にはその頂点に至るまでの経路が入っており自身を含めない
void find_shortest_path(string start_page_title, string goal_page_title) {
    long long start_page_id = title_to_id[start_page_title];
    long long goal_page_id = title_to_id[goal_page_title];

    /// @brief id_to_dist[id] = 最短距離
    map<long long, long long> id_to_dist;
    // これはid_to_idpathの大きさを取れば良いので要らない

    /// @brief id_to_idpath[id] = idからの最短経路がidで入ったqueue
    /// @note そのidのPage「より前」のpathが入っている 本人は入っていない
    map<long long, queue<long long>> id_to_idpath;

    /* BFSを始める前のセットアップ */
    queue<long long> id_queue;
    id_queue.push(start_page_id);
    id_to_dist[start_page_id] = 0;

    /* BFS */
    while (!id_queue.empty()) {
        long long current_id = id_queue.top();
        id_queue.pop();
        
        /* 見つけたら出力 */
        if (current_id == goal_page_id) {
            cout << "Found the goal page!" << endl;
            ofstream ofs("../Week4/output/HW1.txt");
            ofs << "Shortest path length from " << start_page_title << " to " 
                << goal_page_title << " is: "
                << id_to_dist[current_id] << endl;
            ofs << "Path: ";
            queue<long long> found_path = id_to_idpath[current_id];
            while (!found_path.empty()) {
                long long found_id = found_path.front();
                found_path.pop();
                ofs << id_to_page[found_id].title << " -> ";
            }
            ofs << goal_page_title << endl;
            return;
        }
        
        for (long long neighbor_id : id_to_ids[current_id]) {
            /* 確定済みの頂点はスキップ */
            if(id_to_dist.count(neighbor_id)) {
                continue;
            }
            /* 未確定の頂点は距離と経路を更新してスタックに追加 */
            id_to_dist[neighbor_id] = id_to_dist[current_id] + 1;
            id_to_idpath[neighbor_id] = id_to_idpath[current_id];
            id_to_idpath[neighbor_id].push(current_id);
            id_queue.push(neighbor_id);
            // これは計算量食い過ぎ
            // O(N^2) (n+1) * n/2
        }
    }
}


void output_shortest_path() {
    string start_page_title, goal_page_title;
    cout << "Enter the start page title: ";
    cin >> start_page_title;
    cout << "Enter the goal page title: ";
    cin >> goal_page_title;

    find_shortest_path(start_page_title, goal_page_title);
    return;
}

