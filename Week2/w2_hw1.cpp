/**
 * @file
 * w2_hw1.cpp
 * 
 * @brief
 * 自作ハッシュテーブル + テスト
 * 
 * @note
 * データの追加・取得・削除を高速に行うハッシュテーブルの実装
 * 今回は安全に保存するためのハッシュではなく、indexを数字にして探しやすくするためのハッシュ
 * 
 * 気になったものを3タイプ作る
 * - hash : 再ハッシュなし、テーブルの長さが998244353固定 【17123ms】
 * - rehash : 再ハッシュあり、授業で習ったハッシュ関数 【0ms】
 * - multi_rehash : 再ハッシュあり3次元テーブル、ハッシュ値を3種類用意することで衝突回数を減らしたい 【20ms】
 *  → 書いてから気がついたけどこれ3倍じゃなくて3乗だ 全然ダメかも
 * 
 * 大枠はどれも一緒
 * - keyのハッシュ値をindexとして、tableの中からitemを探す
 * - itemにはkeyとvalueを入れ、衝突時用のアドレスをnextポインタで繋げておく
 * 
 * @todo
 * - メモリ(特にアロケータ)がよくわからない 苦Cを読む
 * - std::moveだけusing namespace stdで省略できなかった 確認
 * - t
 */


#include <bits/stdc++.h>
#include <chrono>  // 時間計測用
using namespace std;
typedef long long ll;
const int mod = 998244353;
struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); } }init;


/* ハッシュ関数 */

/// @brief hash, rehash, で使う
/// @param key ハッシュ化する文字列
/// @param table_size ハッシュテーブルのサイズ or 998244353
/// @return int型のハッシュ値
int calculate_hash(const string& key, int table_size) {
    int ret = 0; 
    for (char c : key) {
        ret = (ret * 31 + c) % table_size; 
    }
    return ret;
}

/// @brief multi_rehash で使う
/// @param key 
/// @param table_size 
/// @return int型のハッシュ値3つをtupleに入れて返す
tuple<int, int, int> calculate_multi_hash(const string& key, int table_size) {
    int hash1 = 0, hash2 = 0, hash3 = 0;
    for (char c : key) {
        hash1 = (hash1 * 31 + c) % table_size;
        hash2 = (hash2 * 37 + c) % table_size;
        hash3 = (hash3 * 41 + c) % table_size;
    }
    return {hash1, hash2, hash3};
}


/* ハッシュテーブル */

/// @class HashTableException
/// @brief ハッシュテーブルの例外処理クラス
class HashTableException : public exception {
private:
    string message;
public:
    HashTableException(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

/// @struct Item
/// @brief ハッシュテーブルの中のアイテムを表す構造体
/// @note string key, string value, Item* next(衝突時に見るべきアドレス)
struct Item {
    string key;
    string value;
    Item* next;

    Item(const string& key, const string& value, Item* next = nullptr)
        : key(key), value(value), next(next) {}
};

/// @class HashTable
/// @brief サイズの固定されたハッシュテーブル
class HashTable {
    public:
        HashTable() {
            buckets = vector<Item*>(mod, nullptr);
        }

        /// @brief アイテムをハッシュテーブルに追加
        /// @param key 追加するアイテムのキー
        /// @param value 追加するアイテムの値
        /// @return 追加したらtrue, 上書きしたらfalse
        /// @throws HashTableException キーが空または内部エラーが発生した場合
        bool put(const string& key, const string& value) {
            if (key.empty()) {
                throw HashTableException("Key cannot be empty");
            }

            try {
                int index = calculate_hash(key, mod);
                Item* item = buckets[index];

                /* 先客がいる */
                while (item) {
                    // 既に同じkeyが存在する場合は上書きしてfalseを返す
                    if (item->key == key) {
                        item->value = value;
                        return false;
                    }
                    // 衝突しているので次の住所を探しに行く
                    item = item->next;
                }

                /* 空いてた */
                Item* new_item = new Item(key, value, buckets[index]);
                buckets[index] = new_item;
                return true;
    
            } catch (const bad_alloc& e) {
                throw HashTableException("Memory allocation failed: " + string(e.what()));
            } catch (const exception& e) {
                throw HashTableException("Error in put operation: " + string(e.what()));
            }
        }

        /// @brief アイテムをハッシュテーブルから取得
        /// @param key 取得するアイテムのキー
        /// @return 取得したvalue, 取得できたか否か
        /// @throws HashTableException キーが空または内部エラーが発生した場合
        pair<string, bool> get(const string& key) {
            if (key.empty()) {
                throw HashTableException("Key cannot be empty");
            }

            try {
                int index = calculate_hash(key, mod);
                Item* item = buckets[index];

                /* 指定したハッシュ値が見つかった */
                while (item) {
                    if (item->key == key) {
                        return {item->value, true};
                    }
                    item = item->next;
                }

                /* ハッシュ値やkeyが見つからなかった */
                return {"", false};

            } catch (const exception& e) {
                throw HashTableException("Error in get operation: " + string(e.what()));
            }
        }

        /// @brief アイテムをハッシュテーブルから削除
        /// @param key 削除するアイテムのキー
        /// @return 削除できたらtrue, できなかったらfalse
        /// @throws HashTableException キーが空または内部エラーが発生した場合
        bool delete_item(const string& key) {
            if (key.empty()) {
                throw HashTableException("Key cannot be empty");
            }

            try {
                int index = calculate_hash(key, mod);
                Item* item = buckets[index];
                Item* prev = nullptr;

                /* 指定したハッシュ値が見つかった */
                while (item) {
                    if (item->key == key) {
                        if (prev) {
                            prev->next = item->next;
                        } else {
                            buckets[index] = item->next;
                        }
                        delete item;
                        
                        return true;
                    }
                    prev = item;
                    item = item->next;
                }

                /* ハッシュ値やkeyが見つからなかった */
                return false;
            } catch (const exception& e) {
                throw HashTableException("Error in delete operation: " + string(e.what()));
            }
        }

    private:
        vector<Item*> buckets;
};

/// @class RehashTable
/// @brief 再ハッシュ可能なハッシュテーブル(宿題)
class RehashTable {
    public:
        // 再ハッシュの閾値を定数として定義 (こういうのは大文字のスネークケースがいいらしい)
        static constexpr double REHASH_GROW_THRESHOLD = 0.7;   // この値を超えたらテーブルを拡大
        static constexpr double REHASH_SHRINK_THRESHOLD = 0.2; // この値を下回ったらテーブルを縮小
        static constexpr int MIN_BUCKET_SIZE = 97;             // 最小バケットサイズ

        RehashTable() {
            bucket_size = MIN_BUCKET_SIZE;
            buckets = vector<Item*>(bucket_size, nullptr);
            item_count = 0;
        }

        // デストラクタを追加 (メモリーリークの防止？)
        ~RehashTable() {
            clear();
        }

        /// @brief アイテムをハッシュテーブルに追加
        /// @param key 追加するアイテムのキー
        /// @param value 追加するアイテムの値
        /// @return 追加したらtrue, 上書きしたらfalse
        /// @throws HashTableException キーが空または内部エラーが発生した場合
        bool put(const string& key, const string& value) {
            if (key.empty()) {
                throw HashTableException("Key cannot be empty");
            }

            try {
                check_size();
                int index = calculate_hash(key, bucket_size);
                Item* item = buckets[index];

                /* 先客がいる */
                while (item) {
                    // 既に同じkeyが存在する場合は上書きしてfalseを返す
                    if (item->key == key) {
                        item->value = value;
                        return false;
                    }
                    // 衝突しているので次の住所を探しに行く
                    item = item->next;
                }

                /* 空いてた */
                Item* new_item = new Item(key, value, buckets[index]);
                buckets[index] = new_item;
                item_count++;

                return true;
            } catch (const bad_alloc& e) {
                throw HashTableException("Memory allocation failed: " + string(e.what()));
            } catch (const exception& e) {
                throw HashTableException("Error in put operation: " + string(e.what()));
            }
        }

        /// @brief アイテムをハッシュテーブルから取得
        /// @param key 取得するアイテムのキー
        /// @return 取得したvalue, 取得できたか否か
        /// @throws HashTableException キーが空または内部エラーが発生した場合
        pair<string, bool> get(const string& key) {
            if (key.empty()) {
                throw HashTableException("Key cannot be empty");
            }

            try {
                int index = calculate_hash(key, bucket_size);
                Item* item = buckets[index];

                /* 指定したハッシュ値が見つかった */
                while (item) {
                    if (item->key == key) {
                        return {item->value, true};
                    }
                    item = item->next;
                }

                /* ハッシュ値やkeyが見つからなかった */
                return {"", false};

            } catch (const exception& e) {
                throw HashTableException("Error in get operation: " + string(e.what()));
            }
        }

        /// @brief アイテムをハッシュテーブルから削除
        /// @param key 削除するアイテムのキー
        /// @return 削除できたらtrue, できなかったらfalse
        /// @throws HashTableException キーが空または内部エラーが発生した場合
        bool delete_item(const string& key) {
            if (key.empty()) {
                throw HashTableException("Key cannot be empty");
            }

            try {
                int index = calculate_hash(key, bucket_size);
                Item* item = buckets[index];
                Item* prev = nullptr;

                /* 指定したハッシュ値が見つかった */
                while (item) {
                    if (item->key == key) {
                        if (prev) {
                            prev->next = item->next;
                        } else {
                            buckets[index] = item->next;
                        }
                        delete item;
                        item_count--;
                        
                        // テーブルサイズを調整するかチェック
                        check_size();
                        return true;
                    }
                    prev = item;
                    item = item->next;
                }

                /* ハッシュ値やkeyが見つからなかった */
                return false;
            } catch (const exception& e) {
                throw HashTableException("Error in delete operation: " + string(e.what()));
            }
        }

        /// @brief ハッシュテーブルを空にする
        /// @note こんな全探索みたいな消し方で大丈夫なのかなあ
        void clear() {
            for (int i = 0; i < bucket_size; i++) {
                Item* item = buckets[i];
                while (item) {
                    Item* next = item->next;
                    delete item;
                    item = next;
                }
                buckets[i] = nullptr;
            }
            item_count = 0;
        }

        /// @brief ハッシュテーブルのサイズを取得
        int size() const {
            return item_count;
        }

        /// @brief ロードファクター？を取得
        double load_factor() const {
            return static_cast<double>(item_count) / bucket_size;
        }
    
    private:
        int bucket_size;
        vector<Item*> buckets;
        int item_count;
    
        /// @brief テーブルサイズを確認し、必要に応じて再ハッシュを行う
        void check_size() {
            double load_factor = static_cast<double>(item_count) / bucket_size;
            
            if (load_factor > REHASH_GROW_THRESHOLD) {
                // テーブルを拡大
                int new_size = bucket_size * 2 + 1; // 奇数サイズにして衝突を減らす
                rehash(new_size);
            } else if (bucket_size > MIN_BUCKET_SIZE && load_factor < REHASH_SHRINK_THRESHOLD) {
                // テーブルを縮小
                int new_size = max(MIN_BUCKET_SIZE, bucket_size / 2);
                rehash(new_size);
            }
        }

        /// @brief テーブルを再ハッシュする
        /// @param new_size 新しいバケットサイズ
        void rehash(int new_size) {
            vector<Item*> new_buckets(new_size, nullptr);
            
            /* 内容物のお引越し */
            for (int i = 0; i < bucket_size; i++) {
                Item* item = buckets[i];
                while (item) {
                    Item* next = item->next;  // 次のitemを保存
                    
                    // 新しいハッシュ値を計算
                    int new_index = calculate_hash(item->key, new_size);
                    
                    // 新しいtableに挿入（先頭挿入法）
                    item->next = new_buckets[new_index];
                    new_buckets[new_index] = item;
                    
                    item = next;  // 次のitemへ
                }
            }
            
            // 新しいtableに切り替え
            buckets = std::move(new_buckets);  // std:: を明示的に追加しないとエラーになるみたいだった
            bucket_size = new_size;
        }
};

/// @class MultiRehashTable
/// @brief 再ハッシュ可能な三次元ハッシュテーブル
class MultiRehashTable {
    public:
        // 再ハッシュの閾値を定数として定義 (こういうのは大文字のスネークケースがいいらしい)
        static constexpr double REHASH_GROW_THRESHOLD = 0.7;   // この値を超えたらテーブルを拡大
        static constexpr double REHASH_SHRINK_THRESHOLD = 0.2; // この値を下回ったらテーブルを縮小
        static constexpr int MIN_BUCKET_SIZE = 97;             // 最小バケットサイズ

        MultiRehashTable() {
            bucket_size = MIN_BUCKET_SIZE;
            buckets = vector<vector<vector<Item*>>>
                (bucket_size, vector<vector<Item*>>(bucket_size, vector<Item*>(bucket_size, nullptr)));
            item_count = 0;
        }

        // デストラクタを追加 (メモリーリークの防止？)
        ~MultiRehashTable() {
            clear();
        }

        /// @brief アイテムをハッシュテーブルに追加
        /// @param key 追加するアイテムのキー
        /// @param value 追加するアイテムの値
        /// @return 追加したらtrue, 上書きしたらfalse
        /// @throws HashTableException キーが空または内部エラーが発生した場合
        bool put(const string& key, const string& value) {
            if (key.empty()) {
                throw HashTableException("Key cannot be empty");
            }

            try {
                check_size();
                auto [index1, index2, index3] = calculate_multi_hash(key, bucket_size);
                Item* item = buckets[index1][index2][index3];

                /* 先客がいる */
                while (item) {
                    // 既に同じkeyが存在する場合は上書きしてfalseを返す
                    if (item->key == key) {
                        item->value = value;
                        return false;
                    }
                    // 衝突しているので次の住所を探しに行く
                    item = item->next;
                }

                /* 空いてた */
                Item* new_item = new Item(key, value, buckets[index1][index2][index3]);
                buckets[index1][index2][index3] = new_item;
                item_count++;

                return true;
            } catch (const bad_alloc& e) {
                throw HashTableException("Memory allocation failed: " + string(e.what()));
            } catch (const exception& e) {
                throw HashTableException("Error in put operation: " + string(e.what()));
            }
        }

        /// @brief アイテムをハッシュテーブルから取得
        /// @param key 取得するアイテムのキー
        /// @return 取得したvalue, 取得できたか否か
        /// @throws HashTableException キーが空または内部エラーが発生した場合
        pair<string, bool> get(const string& key) {
            if (key.empty()) {
                throw HashTableException("Key cannot be empty");
            }

            try {
                auto [index1, index2, index3] = calculate_multi_hash(key, bucket_size);
                Item* item = buckets[index1][index2][index3];

                /* 指定したハッシュ値が見つかった */
                while (item) {
                    if (item->key == key) {
                        return {item->value, true};
                    }
                    item = item->next;
                }

                /* ハッシュ値やkeyが見つからなかった */
                return {"", false};

            } catch (const exception& e) {
                throw HashTableException("Error in get operation: " + string(e.what()));
            }
        }

        /// @brief アイテムをハッシュテーブルから削除
        /// @param key 削除するアイテムのキー
        /// @return 削除できたらtrue, できなかったらfalse
        /// @throws HashTableException キーが空または内部エラーが発生した場合
        bool delete_item(const string& key) {
            if (key.empty()) {
                throw HashTableException("Key cannot be empty");
            }

            try {
                auto [index1, index2, index3] = calculate_multi_hash(key, bucket_size);
                Item* item = buckets[index1][index2][index3];
                Item* prev = nullptr;

                /* 指定したハッシュ値が見つかった */
                while (item) {
                    if (item->key == key) {
                        if (prev) {
                            prev->next = item->next;
                        } else {
                            buckets[index1][index2][index3] = item->next;
                        }
                        delete item;
                        item_count--;
                        
                        // テーブルサイズを調整するかチェック
                        check_size();
                        return true;
                    }
                    prev = item;
                    item = item->next;
                }

                /* ハッシュ値やkeyが見つからなかった */
                return false;
            } catch (const exception& e) {
                throw HashTableException("Error in delete operation: " + string(e.what()));
            }
        }

        /// @brief ハッシュテーブルを空にする
        /// @note 3次元の場合は3重ループで全バケットを探索(これダメかも)
        void clear() {
            for (int i = 0; i < bucket_size; i++) {
                for (int j = 0; j < bucket_size; j++) {
                    for (int k = 0; k < bucket_size; k++) {
                        Item* item = buckets[i][j][k];
                        while (item) {
                            Item* next = item->next;
                            delete item;
                            item = next;
                        }
                        buckets[i][j][k] = nullptr;
                    }
                }
            }
            item_count = 0;
        }

        /// @brief ハッシュテーブルのサイズを取得
        int size() const {
            return item_count;
        }

        /// @brief ロードファクター？を取得
        double load_factor() const {
            return static_cast<double>(item_count) / bucket_size;
        }
    
    private:
        int bucket_size;
        vector<vector<vector<Item*>>> buckets;
        int item_count;
    
        /// @brief テーブルサイズを確認し、必要に応じて再ハッシュを行う
        void check_size() {
            double load_factor = static_cast<double>(item_count) / bucket_size;
            
            if (load_factor > REHASH_GROW_THRESHOLD) {
                // テーブルを拡大
                int new_size = bucket_size * 2 + 1; // 奇数サイズにして衝突を減らす
                rehash(new_size);
            } else if (bucket_size > MIN_BUCKET_SIZE && load_factor < REHASH_SHRINK_THRESHOLD) {
                // テーブルを縮小
                int new_size = max(MIN_BUCKET_SIZE, bucket_size / 2);
                rehash(new_size);
            }
        }

        /// @brief テーブルを再ハッシュする
        /// @param new_size 新しいバケットサイズ
        void rehash(int new_size) {
            vector<vector<vector<Item*>>> new_buckets
                (new_size, vector<vector<Item*>>(new_size, vector<Item*>(new_size, nullptr)));
            
            /* 内容物のお引越し */
            for (int i = 0; i < bucket_size; i++) {
                for (int j = 0; j < bucket_size; j++) {
                    for (int k = 0; k < bucket_size; k++) {
                        Item* item = buckets[i][j][k];
                        while (item) {
                            Item* next = item->next;  // 次のitemを保存
                            
                            // 新しいハッシュ値を計算
                            auto [new_i, new_j, new_k] = calculate_multi_hash(item->key, new_size);
                            
                            // 新しいテーブルに挿入（先頭挿入法）
                            item->next = new_buckets[new_i][new_j][new_k];
                            new_buckets[new_i][new_j][new_k] = item;
                            
                            item = next;  // 次のitemへ
                        }
                    }
                }
            }
            
            // 新しいtableに切り替え
            buckets = std::move(new_buckets);  // std:: を明示的に追加しないとエラーになるみたいだった
            bucket_size = new_size;
        }
};

/* テスト */

// 各ハッシュテーブル実装のテスト関数を分ける
template<typename T>
/// @brief ハッシュテーブルの基本機能をテストする関数
/// @note TはHashTable, RehashTable, MultiRehashTableのいずれか
void test_hash_table() {
    T hash_table;
    
    // 基本機能テスト
    assert(hash_table.put("aaa", "1") == true);
    assert(hash_table.get("aaa") == make_pair("1", true));
    
    // templateで型を確認 → サイズチェックは RehashTable と MultiRehashTable でのみ行う
    if constexpr (is_same_v<T, RehashTable> || is_same_v<T, MultiRehashTable>) {
        assert(hash_table.size() == 1);
    }
    
    assert(hash_table.put("bbb", "2") == true);
    assert(hash_table.put("ccc", "3") == true);
    assert(hash_table.put("ddd", "4") == true);
    assert(hash_table.get("aaa") == make_pair("1", true));
    assert(hash_table.get("bbb") == make_pair("2", true));
    assert(hash_table.get("ccc") == make_pair("3", true));
    assert(hash_table.get("ddd") == make_pair("4", true));
    assert(hash_table.get("a").second == false);
    assert(hash_table.get("aa").second == false);
    assert(hash_table.get("aaaa").second == false);
    
    if constexpr (is_same_v<T, RehashTable> || is_same_v<T, MultiRehashTable>) {
        assert(hash_table.size() == 4);
    }
    
    assert(hash_table.put("aaa", "11") == false);
    assert(hash_table.get("aaa") == make_pair("11", true));
    
    assert(hash_table.delete_item("aaa") == true);
    assert(hash_table.get("aaa").second == false);
    
    assert(hash_table.delete_item("a") == false);
    assert(hash_table.delete_item("aa") == false);
    assert(hash_table.delete_item("aaa") == false);
    assert(hash_table.delete_item("aaaa") == false);
    
    assert(hash_table.delete_item("ddd") == true);
    assert(hash_table.delete_item("ccc") == true);
    assert(hash_table.delete_item("bbb") == true);
    assert(hash_table.get("aaa").second == false);
    assert(hash_table.get("bbb").second == false);
    assert(hash_table.get("ccc").second == false);
    assert(hash_table.get("ddd").second == false);
    
    assert(hash_table.put("abc", "1") == true);
    assert(hash_table.put("acb", "2") == true);
    assert(hash_table.put("bac", "3") == true);
    assert(hash_table.put("bca", "4") == true);
    assert(hash_table.put("cab", "5") == true);
    assert(hash_table.put("cba", "6") == true);
    assert(hash_table.get("abc") == make_pair("1", true));
    assert(hash_table.get("acb") == make_pair("2", true));
    assert(hash_table.get("bac") == make_pair("3", true));
    assert(hash_table.get("bca") == make_pair("4", true));
    assert(hash_table.get("cab") == make_pair("5", true));
    assert(hash_table.get("cba") == make_pair("6", true));
    
    assert(hash_table.delete_item("abc") == true);
    assert(hash_table.delete_item("cba") == true);
    assert(hash_table.delete_item("bac") == true);
    assert(hash_table.delete_item("bca") == true);
    assert(hash_table.delete_item("acb") == true);
    assert(hash_table.delete_item("cab") == true);
    if constexpr (is_same_v<T, RehashTable> || is_same_v<T, MultiRehashTable>) {
        assert(hash_table.size() == 0);
    }
    
    cout << "Functional tests passed!" << endl;
}

void functional_test() {
    cout << "Testing HashTable..." << endl;
    test_hash_table<HashTable>();
    
    cout << "Testing RehashTable..." << endl;
    test_hash_table<RehashTable>();
    
    cout << "Testing MultiRehashTable..." << endl;
    test_hash_table<MultiRehashTable>();
    
    cout << "All functional tests passed!" << endl;
}


/* 時間測る */

/// @brief 処理時間を計測する関数
/// @param func 計測する関数
/// @return 実行時間（ミリ秒）
template <typename Func>
long long measure_time(Func func) {
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}


int main() {
    cout << "=== Functional Test Started ===\n" << endl;
    
    // 各実装のテスト実行時間を個別に計測
    cout << "HashTable test time: ";
    long long hash_table_time = measure_time([]() {
        test_hash_table<HashTable>();
    });
    cout << hash_table_time << " ms" << endl;
    
    cout << "RehashTable test time: ";
    long long rehash_table_time = measure_time([]() {
        test_hash_table<RehashTable>();
    });
    cout << rehash_table_time << " ms" << endl;
    
    cout << "MultiRehashTable test time: ";
    long long multi_rehash_table_time = measure_time([]() {
        test_hash_table<MultiRehashTable>();
    });
    cout << multi_rehash_table_time << " ms" << endl;
    
    cout << "\nAll tests completed!" << endl;
    
    return 0;
}