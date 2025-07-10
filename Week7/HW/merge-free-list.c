/**
 * @file
 * Week7/HW/merge-free-list.c
 * 
 * @brief
 * Free Listをマージするmalloc
 * 
 * @note
 * - 断片化を防ぐために、Free Listをマージする
 * - ベースはfree-list-bin-malloc.c
 * - 挿入時にsort&mergeしてしまうので、常にsortもmergeもされている前提で話が進んでいる
 *   (要するに授業スライドの156枚目みたいな例外処理はしてない)
 * 
 * @todo
 * - hilkalium-san/malloc/malloc.c に適用させる前に関数名を元に戻す必要がある
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
#include "free-list-bin-malloc.c"

/// @brief my_add_to_free_list()のmerge free list版実装例
/// @note 
///   ただの挿入(要素数+1) || 左右いずれかのfree listとmerge(±0) || 左右両方とmerge(-1)
///   if(前と繋がるかな)とif(後ろと繋がるかな)で2回同じような操作をします
/// @todo 後々二分探索木みたいにしたい
/// @param metadata 追加するメタデータ
void my_add_to_free_list_merge(my_metadata_t *metadata) {
  int bin_index = get_bin_index(metadata->size);
  assert(!metadata->next);

  /// @brief 挿入位置の一つ前のメタデータを指すポインタ
  /// @note metadataよりもアドレスが小さい
  my_metadata_t *prev_metadata = NULL;

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
}