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

#ifndef GKRTOS_STRUCTURES_LIST_H
#define GKRTOS_STRUCTURES_LIST_H

#include <stdint-gcc.h>
struct gkrtos_list_item {
  struct gkrtos_list_item *next;
  struct gkrtos_list_item *prev;
  void *data;
};

struct gkrtos_list {
  struct gkrtos_list_item *head;
  struct gkrtos_list_item *tail;
  uint32_t length;
};

// =========================
// == OS Public Functions ==
// =========================
struct gkrtos_list_item *gkrtos_list_item_new();

struct gkrtos_list *gkrtos_list_append(struct gkrtos_list *list, void *item);

struct gkrtos_list *gkrtos_list_prepend(struct gkrtos_list *list, void *item);

struct gkrtos_list *gkrtos_list_remove(struct gkrtos_list *list,
                                       struct gkrtos_list_item *item);

struct gkrtos_list *gkrtos_list_new();

void *gkrtos_list_get_tail(struct gkrtos_list *list);

struct gkrtos_list_item *gkrtos_list_get_tail_item(struct gkrtos_list *list);

struct gkrtos_list_item *gkrtos_list_get_head_item(struct gkrtos_list *list);

struct gkrtos_list_item *gkrtos_list_get_item_with_data(
    struct gkrtos_list *list, void *data);

void *gkrtos_list_rotate(struct gkrtos_list *list);

void *gkrtos_list_get_head(struct gkrtos_list *list);

#endif
