#include "list.h"

#include <malloc.h>

struct gkrtos_list_item* gkrtos_list_item_new() {
  struct gkrtos_list_item* item = malloc(sizeof(struct gkrtos_list_item));
  item->next = NULL;
  item->prev = NULL;
  item->data = NULL;
  return item;
}

struct gkrtos_list* gkrtos_list_append(struct gkrtos_list* list,
                                       struct gkrtos_list_item* item) {
  if (list->length == 0) {
    // First item in list
    list->head = item;
    list->tail = item;
    item->next = item;
    item->prev = item;
    list->length = 1;
    return list;
  }

  list->tail->next = item;
  item->prev = list->tail;
  item->next = list->head;
  list->tail = item;
  list->length++;
  return list;
}

struct gkrtos_list* gkrtos_list_new() {
  struct gkrtos_list* list = malloc(sizeof(struct gkrtos_list_item));
  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
  return list;
}
