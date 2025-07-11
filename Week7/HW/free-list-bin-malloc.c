/**
 * @file
 * Week7/HW/free-list-bin-malloc.c
 * 
 * @brief
 * Free Listを大きさ別に管理
 * 
 * @note
 * - 断片化を防ぐために、Free Listを大きさ別に管理する
 * - ベースはBest Fitのmalloc
 * 
 * @todo
 * - hilkalium-san/malloc/malloc.c に適用させる前に関数名を元に戻す必要がある
 * 
 * @結果
 * 
 * 
 * 
 */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.c"

/**
 * 下準備
 */

/** 
 * @brief Free List Binたち
 * @note 
 * [0]: 0-63バイト, [1]: 64-127バイト, [2]: 128-255バイト, 
 * [3]: 256-511バイト, [4]: 512-1023バイト, [5]: 1024-2047バイト, 
 * [6]: 2048-4095バイト, [7]: 4096-8191バイト, [8]: 8192-16383バイト, [9]: 16384以上
 */
my_heap_t free_list_bins[10];

/**
 * @brief my_add_to_free_list()のfree list bin版実装例
 * @note 使い終わった、または余った分を「先頭に」追加
 * @param metadata 追加するメタデータ
 */
void my_add_to_free_list_bin(my_metadata_t *metadata) {
  int bin_index = get_bin_index(metadata->size);
  assert(!metadata->next);
  metadata->next = free_list_bins[bin_index].free_head;
  free_list_bins[bin_index].free_head = metadata;
}

/** 
 * @brief my_remove_from_free_list()のfree list bin版実装例
 * @note 割り当てたいメモリ領域を取り除く
 * @param metadata 削除するメタデータ
 * @param prev 前のメタデータ
 */
void my_remove_from_free_list_bin(my_metadata_t *metadata, my_metadata_t *prev) {
  int bin_index = get_bin_index(metadata->size);
  if (prev) {
    prev->next = metadata->next;
  } else {
    free_list_bins[bin_index].free_head = metadata->next;
  }
  metadata->next = NULL;
}

/** 
 * @brief my_initialize()のfree list bin版実装例
 * @note
 *   mallocの仕組みを使い始める前に、heapの状態を初期化する関数
 *   空きメモリリストを空の状態(ダミーノードのみ)にしている
 */
void my_initialize_bin() {
  for(int i = 0; i < 10; i++) {
    free_list_bins[i].free_head = &free_list_bins[i].dummy;
    free_list_bins[i].dummy.size = 0;
    free_list_bins[i].dummy.next = NULL;
  }
}


/**
 * @brief にぶたんでFree List Binのインデックスを計算する関数
 * @param alloc_size 割り当てるメモリのサイズ
 * @return Free List Binのインデックス
 * @note
 * [0]: 0-63バイト, [1]: 64-127バイト, [2]: 128-255バイト, 
 * [3]: 256-511バイト, [4]: 512-1023バイト, [5]: 1024-2047バイト, 
 * [6]: 2048-4095バイト, [7]: 4096-8191バイト, [8]: 8192-16383バイト, [9]: 16384以上
 */
int get_bin_index(size_t alloc_size) {
  /*
  int left = 0, right = 9;
  int bin_index = 0;
  while (left <= right) {
    bin_index = (left + right) / 2;
    if (alloc_size < (1 << (bin_index + 6))) {
      if(alloc_size > (1 << (bin_index + 5))) {
        // alloc_sizeはbin_indexの範囲内なのでindex確定
        break;
      }
      right = bin_index - 1;
    } else {
      left = bin_index + 1;
    }
  }
  return bin_index;
  */

  /// なんか怪しかったので愚直に変更
  if (alloc_size <= 63) return 0;
  if (alloc_size <= 127) return 1;
  if (alloc_size <= 255) return 2;
  if (alloc_size <= 511) return 3;
  if (alloc_size <= 1023) return 4;
  if (alloc_size <= 2047) return 5;
  if (alloc_size <= 4095) return 6;
  if (alloc_size <= 8191) return 7;
  if (alloc_size <= 16383) return 8;
  return 9;
}

/**
 * mallocの実装
 */

/**
 * @brief 空き領域を探す関数
 * @note 
 *   free list bin対応、Best Fit方式で空き領域を探索
 *   Cはpairがないらしい？ので、引数をいじる形で値を渡す
 *   さらにポインタの参照渡しはなんかヤバそうだったのでダブルポインタを使う
 * @param alloc_size 割り当てるメモリのサイズ
 * @param bin_index どのbinから探すか
 * @param prev_metadata_ptr prev_metadataのポインタ
 * @return alloc_metadata
 * @return はしないけどprev_metadata_ptrをいじる
 */
my_metadata_t *find_free_slot(size_t alloc_size, int bin_index, 
                              my_metadata_t **prev_metadata_ptr) {

  /// @brief ここにthe best free slotの先頭のポインタを入れる
  my_metadata_t *alloc_metadata = NULL;

  /// @brief the best free slotの一つ前のmetadataのポインタ
  my_metadata_t *prev_metadata = NULL;

  /// @brief 今見ているfree slotの先頭のポインタ
  my_metadata_t *current_metadata = free_list_bins[bin_index].free_head;

  /// @brief 今見ているfree slotの一つ前のmetadataのポインタ
  my_metadata_t *prev_current_metadata = NULL;

  while (current_metadata) {
    if (current_metadata->size > alloc_size && 
        (alloc_metadata == NULL || current_metadata->size < alloc_metadata->size)) {
      alloc_metadata = current_metadata;
      prev_metadata = prev_current_metadata;
    }
    prev_current_metadata = current_metadata;
    current_metadata = current_metadata->next;
  }

  *prev_metadata_ptr = prev_metadata;
  return alloc_metadata;
}

/**
 * @brief Free List Binを用いてBest Fit方式のメモリ割り当てを行う関数
 * @param alloc_size 割り当てるメモリのサイズ
 * @return 割り当てたメモリ領域のポインタ
 */
void *free_list_bin_malloc(size_t alloc_size) {
  /* 空き領域を探す(Best Fit) */
  int bin_index = get_bin_index(alloc_size);

  /// @brief ここにthe best free slotの先頭のポインタを入れる
  my_metadata_t *alloc_metadata = NULL;

  /// @brief the best free slotの一つ前のmetadataのポインタ
  my_metadata_t *prev_metadata = NULL;

  alloc_metadata = find_free_slot(alloc_size, bin_index, &prev_metadata);

  /**
   * now, alloc_metadata points to the best free slot
   * and prev_metadata is the previous entry.
   */

  /** 
   * 空き領域がなかった場合 
   */
  while (!alloc_metadata) {
    /**
     * There was no free slot available. We need to request a new memory region
     * from the system by calling mmap_from_system().
     *
     *     | metadata | free slot |
     *     ^
     *     metadata
     *     <---------------------->
     *            buffer_size
     */

    /**
     * 今見たbinが最大のbinでなかった場合は、一つ大きいbinを探しにいく
     */

    if (bin_index < 9) {
      bin_index++;
      alloc_metadata = find_free_slot(alloc_size, bin_index, &prev_metadata);
      break;
    }

    /**
     * 今見たbinが最大のbinだった場合は、
     * mmap_from_system()を呼び出して新しいメモリ領域を確保する
     */

    size_t buffer_size = 4096;
    alloc_metadata = (my_metadata_t *)mmap_from_system(buffer_size);
    alloc_metadata->size = buffer_size - sizeof(my_metadata_t);
    alloc_metadata->next = NULL;
    /// Add the memory region to the free list.
    my_add_to_free_list_bin(alloc_metadata);
    /// Now, try free_list_bin_malloc() again. This should succeed.
    return free_list_bin_malloc(alloc_size);
  }

  /** 
   * 空き領域が見つかった場合(領域の分割)
   *
   * ... | metadata | object | ...
   *     ^          ^
   *     metadata   ptr
   */

  /// @brief メタデータを除いた、実際に使える最初のポインタ
  void *alloc_ptr = alloc_metadata + 1;

  /// @brief 余っちゃったメモリ領域のサイズ
  size_t remaining_size = alloc_metadata->size - alloc_size;

  /// Remove the free slot from the free list.
  my_remove_from_free_list_bin(alloc_metadata, prev_metadata);

  /**
   * 当てがったメモリ領域が余ったら、貸しスペース一覧に追加する
   * (ここでmy_metadata_tのsize上限と比較してるのは、
   * 最低限metadataがおける余裕がなければ空きスペースとは呼ばないから)
   */
  if (remaining_size > sizeof(my_metadata_t)) {
    alloc_metadata->size = alloc_size;
    /**
     * Create a new metadata for the remaining free slot.
     *
     * ... | metadata | object | metadata | free slot | ...
     *     ^          ^        ^
     *     metadata   ptr      remaining_metadata
     *                 <------><---------------------->
     *                   size       remaining size
     */
    my_metadata_t *remaining_metadata = (my_metadata_t *)((char *)alloc_ptr + alloc_size);
    remaining_metadata->size = remaining_size - sizeof(my_metadata_t);
    remaining_metadata->next = NULL;
    /// Add the remaining free slot to the free list.
    my_add_to_free_list_bin(remaining_metadata);
  }
  return alloc_ptr;
}

/**
 * 後処理
 */

/// @brief my_free()のfree list bin版実装例
/// @param alloc_ptr 解放するメモリ領域のポインタ
void my_free_from_bin(void *alloc_ptr) {
  /*
   * Look up the metadata. The metadata is placed just prior to the object.
   *
   * ... | metadata | object | ...
   *     ^          ^
   *     metadata   ptr
   */
  my_metadata_t *alloc_metadata = (my_metadata_t *)alloc_ptr - 1;
  /// 空きリストにぶち込む
  my_add_to_free_list_bin(alloc_metadata);
}

/// @brief 最後のクリーンアップ処理を行う
void my_finalize() {
  // Nothing is here for now.
  // feel free to add something if you want!
}