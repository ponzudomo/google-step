/**
 * @file
 * w2_extra.cpp
 * 
 * @brief
 * 自作ハッシュテーブル + テスト
 * 
 * @note
 * セキュリティ対策っぽいハッシュも書いてみる
 * 
 * 気になったものを4タイプ作る
 * - hash : 再ハッシュなし、テーブルの長さが998244353固定
 * - rehash : 再ハッシュあり、恐らく一番シンプルなハッシュ関数
 * - spiced_rehash : 再ハッシュあり、saltやpepperを盛り込んでみる
 * (- my_sha256 : sha256の真似) ← 余裕があったら
 * 
 * 他の安全化アイデア
 * - ストレッチ：高速なハッシュ関数を用いて、何回もハッシュを計算する
 * - ユーザーがログインに成功するたびにハッシュ方式を変える
 */


// include
#include <bits/stdc++.h>
using namespace std;

// define
#define fore(x, a) for (auto &x : a)
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define repp(i, m, n) for (int i = (int)(m); i < (int)(n); i++)
#define all(v) v.begin(), v.end()
#define rall(v) v.rbegin(), v.rend()

// typedef
typedef long long ll;
typedef unsigned long long ull;

const int inf = 1073741823;
const ll infl = 1LL << 60;
const int mod = 998244353;

struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); } }init;


/* ハッシュ関数 */

/// @brief hash, rehash, spiced_rehash で使う
/// @param key ハッシュ化する文字列 (spiceは入れてから渡す)
/// @param table_size ハッシュテーブルのサイズ or 998244353
/// @return ll型のハッシュ値
ll calculate_hash(const string& key, ll table_size) {
    ll ret;
    for (char c : key) {
        ret = (ret * 31 + c) % table_size; 
    }
    return ret;
}

string generate_salt() {
    static random_device rd;
    static mt19937 mt(rd());
    static const string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    uniform_int_distribution<> dist(0, chars.size() - 1);
    string salt;
    for (int i = 0; i < 16; ++i) { // 16文字のsaltを生成
        salt += chars[dist(mt)];
    }
    return salt;
}


/* ハッシュテーブル */

struct Item {
    string key;
    string value;
    Item* next;

    Item(const string& key, const string& value, Item* next = nullptr)
        : key(key), value(value), next(next) {}
};

class HashTable {
    public:
        HashTable() {
            bucket_size = 97;
            buckets = vector<Item*>(bucket_size, nullptr);
            item_count = 0;
        }
    
        bool put(const string& key, const string& value) {
            check_size();
            int index = calculate_hash(key, bucket_size);
            Item* item = buckets[index];
            while (item) {
                if (item->key == key) {
                    item->value = value;
                    return false;
                }
                item = item->next;
            }
            Item* new_item = new Item(key, value, buckets[index]);
            buckets[index] = new_item;
            item_count++;
            return true;
        }
    
        pair<string, bool> get(const string& key) {
            check_size();
            int index = calculate_hash(key, bucket_size);
            Item* item = buckets[index];
            while (item) {
                if (item->key == key) {
                    return {item->value, true};
                }
                item = item->next;
            }
            return {"", false};
        }
    
        bool delete_item(const string& key) {
            int index = calculate_hash(key, bucket_size);
            Item* item = buckets[index];
            Item* prev = nullptr;
    
            while (item) {
                if (item->key == key) {
                    if (prev) {
                        prev->next = item->next;
                    } else {
                        buckets[index] = item->next;
                    }
                    delete item;
                    item_count--;
                    return true;
                }
                prev = item;
                item = item->next;
            }
            return false;
        }
    
        int size() const {
            return item_count;
        }
    
    private:
        int bucket_size;
        vector<Item*> buckets;
        int item_count;
    
        void check_size() const {
            assert(bucket_size < 100 || item_count >= static_cast<int>(bucket_size * 0.3));
        }
};
    
// テスト関数
void functional_test() {
    HashTable hash_table;
    
    assert(hash_table.put("aaa", "1") == true);
    assert(hash_table.get("aaa") == make_pair("1", true));
    assert(hash_table.size() == 1);
    
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
    assert(hash_table.size() == 4);
    
    assert(hash_table.put("aaa", "11") == false);
    assert(hash_table.get("aaa") == make_pair("11", true));
    assert(hash_table.size() == 4);
    
    assert(hash_table.delete_item("aaa") == true);
    assert(hash_table.get("aaa").second == false);
    assert(hash_table.size() == 3);
    
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
    assert(hash_table.size() == 0);
    
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
    assert(hash_table.size() == 6);
    
    assert(hash_table.delete_item("abc") == true);
    assert(hash_table.delete_item("cba") == true);
    assert(hash_table.delete_item("bac") == true);
    assert(hash_table.delete_item("bca") == true);
    assert(hash_table.delete_item("acb") == true);
    assert(hash_table.delete_item("cab") == true);
    assert(hash_table.size() == 0);
    
    cout << "Functional tests passed!" << endl;
}
    

int main() {
    functional_test();
    return 0;
}