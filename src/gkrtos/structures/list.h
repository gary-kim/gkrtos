#ifndef GKRTOS_STRUCTURES_LIST_H
#define GKRTOS_STRUCTURES_LIST_H

#include <stdint-gcc.h>
struct gkrtos_list_item {
  struct gkrtos_list_item* next;
  struct gkrtos_list_item* prev;
  void* data;
};

struct gkrtos_list {
  struct gkrtos_list_item* head;
  struct gkrtos_list_item* tail;
  uint32_t length;
};

// =========================
// == OS Public Functions ==
// =========================
struct gkrtos_list_item* gkrtos_list_item_new();
struct gkrtos_list* gkrtos_list_append(struct gkrtos_list* list,
                                       struct gkrtos_list_item* item);
struct gkrtos_list* gkrtos_list_new();

#endif
