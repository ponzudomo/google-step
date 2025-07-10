/**
 * @file
 * Week7/HW/common.c
 * 
 * @brief
 * Common functions for memory allocation
 * 
 * @note
 * - hikalium大先生のコードに学習用のコメントを追加したもの
 * - 自分が分かりやすいように、変数名も少し変えた
 * 
 */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Interfaces to get memory pages from OS
 */

void *mmap_from_system(size_t size);
void munmap_to_system(void *ptr, size_t size);

/**
 * Struct definitions
 */

/// @brief 確保したメモリブロックを管理するためのメタデータ(付加情報)を格納する
/// @note 連結リストっぽくなってる サイズとnextポインタが入ってる
typedef struct my_metadata_t {
  /// @brief このメタデータが管理しているメモリブロックのサイズ
  size_t size;
  /// @brief このメタデータの次のメタデータを指すポインタ
  struct my_metadata_t *next;
} my_metadata_t;

/// @brief ヒープ(プログラムが動的に確保するメモリ領域)全体を管理するための構造体
typedef struct my_heap_t {
  /// @brief 現在利用可能な空き領域の先頭を指すポインタ
  my_metadata_t *free_head;
  /// @brief リストの末尾を示すためのダミーのメタデータ。
  /// @note ヒープの初期化時に使用される 連結リストの実装を楽にしてくれる
  my_metadata_t dummy;
} my_heap_t;

/**
 * Static variables (DO NOT ADD ANOTHER STATIC VARIABLES!)
 */
my_heap_t my_heap;

/**
 * Helper functions (feel free to add/remove/edit!)
 */

/// @brief 空き領域リストにメタデータを追加する
/// @note 使い終わった、または余った分を「先頭に」追加
/// @param metadata 追加するメタデータ
void my_add_to_free_list(my_metadata_t *metadata) {
  assert(!metadata->next);
  metadata->next = my_heap.free_head;
  my_heap.free_head = metadata;
}

/// @brief 空き領域リストからメタデータを削除する
/// @note 割り当てたいメモリ領域を取り除く
/// @param metadata 削除するメタデータ
/// @param prev 前のメタデータ
void my_remove_from_free_list(my_metadata_t *metadata, my_metadata_t *prev) {
  if (prev) {
    prev->next = metadata->next;
  } else {
    my_heap.free_head = metadata->next;
  }
  metadata->next = NULL;
}

/**
 * Interfaces of malloc (DO NOT RENAME FOLLOWING FUNCTIONS!)
 */

/// @brief 空き領域リストを初期化する関数
/// @note 
///   mallocの仕組みを使い始める前に、heapの状態を初期化する関数
///   空きメモリリストを空の状態(ダミーノードのみ)にしている
void my_initialize() {
  my_heap.free_head = &my_heap.dummy;
  my_heap.dummy.size = 0;
  my_heap.dummy.next = NULL;
}

/// @brief mallocが返したポインタptrを受け取り、その領域を解放する
/// @param alloc_ptr 解放するメモリ領域のポインタ
void my_free(void *alloc_ptr) {
  /*
   * Look up the metadata. The metadata is placed just prior to the object.
   *
   * ... | metadata | object | ...
   *     ^          ^
   *     metadata   ptr
   */
  my_metadata_t *alloc_metadata = (my_metadata_t *)alloc_ptr - 1;
  /// 空きリストにぶち込む
  my_add_to_free_list(alloc_metadata);
}

/// @brief 最後のクリーンアップ処理を行う
void my_finalize() {
  // Nothing is here for now.
  // feel free to add something if you want!
}

void test() {
  // Implement here!
  assert(1 == 1); /* 1 is 1. That's always true! (You can remove this.) */
}
