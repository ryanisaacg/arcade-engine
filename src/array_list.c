#include <stdlib.h>
#include <string.h>

#include "array_list.h"

ArrayList al_new(size_t item_size) {
    return al_new_sized(item_size, 10);
}

ArrayList al_new_sized(size_t item_size, size_t init_capacity) {
	return al_prealloc(item_size, malloc(item_size * init_capacity), init_capacity);
}

ArrayList al_prealloc(size_t item_size, void *buffer, size_t capacity) {
	return (ArrayList){item_size, 0, capacity, buffer};
}

void *al_get(ArrayList list, size_t index) {
    return list.buffer + list.item_size * index;
}

void al_add(ArrayList *list, void *item) {
    if(list->length < list->capacity / 2) {
        al_set(list, list->length, item);
        list->length++;
    } else {
        list->capacity *= 2;
        list->buffer = realloc(list->buffer, list->capacity * list->item_size);
        al_add(list, item);
    }
}

void al_remove_index(ArrayList *list, size_t index) {
    memmove(list->buffer + list->item_size * index, list->buffer + list->item_size * (index + 1), list->length - index);
    list->length -= 1;
}

void al_remove_item(ArrayList *list, void *item) {
    al_remove_index(list, al_find(list, item));
}

void al_remove_unorder(ArrayList *list, size_t index) {
	memcpy(list->buffer + (list->item_size * index), list->buffer + (list->item_size * (list->length - 1)), list->item_size);
	list->length--;
}

size_t al_find(ArrayList *list, void *item) {
    for(size_t i = 0; i < list->length; i++) {
        if(memcmp(al_get(*list, i), item, list->item_size) == 0) {
            return i;
        }
    }
    return -1;
}

void al_set(ArrayList *list, size_t index, void *data) {
    memcpy(list->buffer + index * list->item_size, data, list->item_size);
}

void al_clear(ArrayList *list) {
	list->length = 0;
}

void al_destroy(ArrayList list) {
	free(list.buffer);
}
