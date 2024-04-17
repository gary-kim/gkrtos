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
