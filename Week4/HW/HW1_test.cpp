/**
 * @file
 * Week4/HW/HW1_test.cpp
 * 
 * @brief
 * find_shortest_path関数のテストを行うプログラム
 * Google Testの力を借りる
 * 
 * @note
 * 
 * 
 * @todo
 * - 
 * 
 */


// include
#include <bits/stdc++.h>
using namespace std;
#include "read_files.h"
#include "HW.h"
#include <gtest/gtest.h>

TEST(HW1Test, ShortestPath_AtoF) {
    // smallデータセットを直接指定
    read_pages("../Week4/input/wikipedia_dataset/small/pages.txt");
    read_links("../Week4/input/wikipedia_dataset/small/links.txt");
    // 始点と終点を設定
    
    find_shortest_path();
    // 必要ならASSERTで経路長や出力内容を検証
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

