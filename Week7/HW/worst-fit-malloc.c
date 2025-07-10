/**
 * @file
 * Week7/HW/worst-fit-malloc.c
 * 
 * @brief
 * Worst Fitのmalloc
 * 
 * @note
 * - best-fit-malloc.cの大小関係をいじっただけ
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

/// @brief Worst Fit方式のメモリ割り当てを行う関数
/// @param alloc_size 割り当てるメモリのサイズ
/// @return 割り当てたメモリ領域のポインタ
void *worst_fit_malloc(size_t alloc_size) {
  /* 空き領域を探す(Worst Fit) */

  /// @brief ここにthe worst free slotの先頭のポインタを入れる
  my_metadata_t *alloc_metadata = my_heap.free_head;

  /// @brief the worst free slotの一つ前のmetadataのポインタ
  my_metadata_t *prev_metadata = NULL;

  /// @brief 今見ているfree slotの先頭のポインタ
  my_metadata_t *current_metadata = my_heap.free_head;

  /// @brief 今見ているfree slotの一つ前のmetadataのポインタ
  my_metadata_t *prev_current_metadata = NULL;

  while (current_metadata) {
    if (current_metadata->size > alloc_metadata->size) {
      alloc_metadata = current_metadata;
      prev_metadata = prev_current_metadata;
    }
    prev_current_metadata = current_metadata;
    current_metadata = current_metadata->next;
  }

  /**
   * now, alloc_metadata points to the worst free slot
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
    /// Now, try worst_fit_malloc() again. This should succeed.
    return worst_fit_malloc(alloc_size);
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