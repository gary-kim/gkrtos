// Copyright (C) 2024 Gary Kim <gary@garykim.dev>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
                                       void* raw_item) {
  struct gkrtos_list_item* item = gkrtos_list_item_new();
  item->data = raw_item;

  if (list->length == 0) {
    // First item in list
    list->head = item;
    item->next = item;
    item->prev = item;
    list->length = 1;
    return list;
  }
  // Append item to list
  struct gkrtos_list_item* tail = list->head->prev;
  tail->next = item;
  item->prev = tail;
  item->next = list->head;
  list->head->prev = item;

  list->length++;
  return list;
}

// Expects a sort_fn with the following signature:
// int sort_fn(void* raw_item, void* previous, void* next);
struct gkrtos_list* gkrtos_list_insert_sorted(struct gkrtos_list* list,
                                              void* raw_item,
                                              bool (*sort_fn)(void*, void*,
                                                              void*)) {
  if (list->length <= 1) {
    return gkrtos_list_append(list, raw_item);
  }
  struct gkrtos_list_item* previous = list->head;
  if (!sort_fn(raw_item, previous, previous->next)) {
    previous = previous->next;
  }

  // Append item to list behind previous
  struct gkrtos_list_item* new_item = gkrtos_list_item_new();
  new_item->data = raw_item;

  struct gkrtos_list_item* next_item = previous->next;

  previous->next = new_item;
  new_item->prev = previous;
  new_item->next = next_item;
  next_item->prev = new_item;

  list->length++;
  return list;
}

struct gkrtos_list* gkrtos_list_prepend(struct gkrtos_list* list,
                                        void* raw_item) {
  struct gkrtos_list_item* item = gkrtos_list_item_new();
  item->data = raw_item;

  if (list->length == 0) {
    // First item in the list
    list->head = item;
    item->next = item;
    item->prev = item;
    list->length = 1;
    return list;
  }
  // Prepend the new item to the list (right behind head)
  item->next = list->head->next;
  item->prev = list->head;
  item->next->prev = item;
  list->head->next = item;
  list->length++;

  return list;
}

struct gkrtos_list* gkrtos_list_insert_head(struct gkrtos_list* list,
                                            void* raw_item) {
  struct gkrtos_list_item* item = gkrtos_list_item_new();
  item->data = raw_item;

  if (list->length == 0) {
    // First item in the list
    list->head = item;
    item->next = item;
    item->prev = item;
    list->length = 1;
    return list;
  }
  // Prepend the new item to the list at the head
  item->next = list->head;
  item->prev = list->head->prev;
  list->head->prev->next = item;
  list->head->prev = item;
  list->head = item;
  list->length++;

  return list;
}

struct gkrtos_list* gkrtos_list_remove(struct gkrtos_list* list,
                                       struct gkrtos_list_item* item) {
  if (list->length == 0) {
    return list;
  }

  if (list->head == item) {
    list->head = item->next;
    list->head->prev = item->prev;
    item->prev->next = item->next;

    if (list->head->next == item) {
      list->head->next = list->head;
    }
    goto SUCCESS;
  }

  // Standard remove
  item->prev->next = item->next;
  item->next->prev = item->prev;

  if (item->next->next == item) {
    item->next->next = item->next;
  }
  if (item->prev->prev == item) {
    item->prev->prev = item->prev;
  }

SUCCESS:
  list->length--;
  free(item);
  return list;
}

struct gkrtos_list* gkrtos_list_new() {
  struct gkrtos_list* list = malloc(sizeof(struct gkrtos_list));
  list->head = NULL;
  list->length = 0;
  return list;
}

void* gkrtos_list_get_head(struct gkrtos_list* list) {
  return list->head->data;
}

void* gkrtos_list_get_following_head_item(struct gkrtos_list* list) {
  return list->head->next->data;
}

void* gkrtos_list_get_tail(struct gkrtos_list* list) {
  return list->head->prev->data;
}

struct gkrtos_list_item* gkrtos_list_get_item_with_data(
    struct gkrtos_list* list, void* data) {
  if (list->length == 0) return NULL;

  if (list->head->data == data) {
    return list->head;
  }

  struct gkrtos_list_item* item = list->head->next;

  while (item != list->head) {
    if (item->data == data) return item;
    item = item->next;
  }

  return NULL;
}

struct gkrtos_list_item* gkrtos_list_get_tail_item(struct gkrtos_list* list) {
  return list->head->prev;
}

struct gkrtos_list_item* gkrtos_list_get_head_item(struct gkrtos_list* list) {
  return list->head;
}

void* gkrtos_list_rotate(struct gkrtos_list* list) {
  if (list->length > 0) {
    list->head = list->head->next;
    return list->head->data;
  }
  return NULL;
}
bool gkrtos_list_is_empty(struct gkrtos_list* list) {
  return list->length == 0;
}
