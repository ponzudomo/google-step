//
// >>>> malloc challenge! <<<<
//
// Your task is to improve utilization and speed of the following malloc
// implementation.
// Initial implementation is the same as the one implemented in simple_malloc.c.
// For the detailed explanation, please refer to simple_malloc.c.

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Interfaces to get memory pages from OS
//

void *mmap_from_system(size_t size);
void munmap_to_system(void *ptr, size_t size);

//
// Struct definitions
//

typedef struct my_metadata_t {
  size_t size;
  struct my_metadata_t *next;
} my_metadata_t;

typedef struct my_heap_t {
  my_metadata_t *free_head;
  my_metadata_t dummy;
} my_heap_t;

//
// Static variables (DO NOT ADD ANOTHER STATIC VARIABLES!)
//
my_heap_t my_heap;

my_heap_t free_list_bins[10];

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

void my_remove_from_free_list(my_metadata_t *metadata, my_metadata_t *prev) {
  int bin_index = get_bin_index(metadata->size);
  if (prev) {
    prev->next = metadata->next;
  } else {
    free_list_bins[bin_index].free_head = metadata->next;
  }
  metadata->next = NULL;
}

/// @brief my_add_to_free_list()のmerge free list版実装例
/// @note 
///   ただの挿入(要素数+1) || 左右いずれかのfree listとmerge(±0) || 左右両方とmerge(-1)
///   if(前と繋がるかな)とif(後ろと繋がるかな)で2回同じような操作をします
/// @todo 後々二分探索木みたいにしたい
/// @param metadata 追加するメタデータ
void my_add_to_free_list(my_metadata_t *metadata) {
  int bin_index = get_bin_index(metadata->size);
  assert(!metadata->next);

  /// @brief 挿入位置の一つ前のメタデータを指すポインタ
  /// @note metadataよりもアドレスが小さい
  my_metadata_t *prev_metadata = NULL;

  /// @brief prevのprev
  /// @note prev_metadataとmergeしてしまった場合、prevがこいつになる
  my_metadata_t *prev_prev_metadata = NULL;

  /// @brief 挿入位置の次のメタデータを指すポインタ
  /// @note metadataよりもアドレスが大きい
  my_metadata_t *next_metadata = free_list_bins[bin_index].free_head;

  /// 挿入位置を見つける
  while (next_metadata && (char *)next_metadata < (char *)metadata) {
    prev_metadata = next_metadata;
    next_metadata = next_metadata->next;
  }
  /// ここでprev_metadataとnext_metadataを使って、metadataを適切な位置に挿入する
  if (prev_metadata) {
    if ((char *)prev_metadata + prev_metadata->size + sizeof(my_metadata_t) == (char *)metadata) {
      /// mergeする with previous metadata
      prev_metadata->size += metadata->size + sizeof(my_metadata_t);
      prev_metadata->next = next_metadata;
      metadata = prev_metadata;
      prev_metadata = prev_prev_metadata;
    } else {
      prev_metadata->next = metadata;
    }
  } else {
    free_list_bins[bin_index].free_head = metadata;
  }
  if(next_metadata && (char *)metadata + metadata->size + sizeof(my_metadata_t) == (char *)next_metadata) {
    /// mergeする with next metadata
    metadata->size += next_metadata->size + sizeof(my_metadata_t);
    metadata->next = next_metadata->next;
  } else {
    metadata->next = next_metadata;
  }

  /// もしmergeによってサイズがbinにそぐわないものになってしまったら、お引越し
  if (metadata->size > (1 << (bin_index + 6))) {
    my_remove_from_free_list(metadata, prev_metadata);
    my_add_to_free_list(metadata);
  }
}

//
// Interfaces of malloc (DO NOT RENAME FOLLOWING FUNCTIONS!)
//

// This is called at the beginning of each challenge.
void my_initialize() {
  for(int i = 0; i < 10; i++) {
    free_list_bins[i].free_head = &free_list_bins[i].dummy;
    free_list_bins[i].dummy.size = 0;
    free_list_bins[i].dummy.next = NULL;
  }
}

void *my_malloc(size_t alloc_size) {
  /* 空き領域を探す(Best Fit) */
  int bin_index = get_bin_index(alloc_size);

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

  /**
   * now, alloc_metadata points to the best free slot
   * and prev_metadata is the previous entry.
   */

  /** 空き領域がなかった場合 */
  if (!alloc_metadata) {
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
    size_t buffer_size = 4096;
    my_metadata_t *alloc_metadata = (my_metadata_t *)mmap_from_system(buffer_size);
    alloc_metadata->size = buffer_size - sizeof(my_metadata_t);
    alloc_metadata->next = NULL;
    /// Add the memory region to the free list.
    my_add_to_free_list(alloc_metadata);
    /// Now, try free_list_bin_malloc() again. This should succeed.
    return my_malloc(alloc_size);
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
  my_remove_from_free_list(alloc_metadata, prev_metadata);

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
    my_add_to_free_list(remaining_metadata);
  }
  return alloc_ptr;
}

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