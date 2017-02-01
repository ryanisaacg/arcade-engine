#pragma once

#include <stdbool.h>

#include "util.h"

typedef struct ArrayList {
    size_t item_size, length, capacity;
    char *buffer;
} ArrayList;

//Create a new array list and allocate its buffer
ArrayList al_new(size_t item_size);
//Create a new array list and give a number of elements to allocate
ArrayList al_new_sized(size_t item_size, size_t init_capacity);
//Create a new array with a preallocated buffer
ArrayList al_prealloc(size_t item_size, void *buffer, size_t capacity);
//Get an item from the array list at the index
void *al_get(ArrayList list, size_t index);
//Add an item at the end of the array list
void al_add(ArrayList *list, void *item);
//Remove the item at the index from the list by shifting elements
void al_remove_index(ArrayList *list, size_t index);
//Remove an item that matches the parameter in memory
void al_remove_item(ArrayList *list, void *item);
//Remove an item by swapping out the last one
void al_remove_unorder(ArrayList *list, size_t index);
//Set the index to the given data
void al_set(ArrayList *list, size_t index, void *data);
//Find an item from the array
size_t al_find(ArrayList *list, void *item);
//Remove all items from the array
void al_clear(ArrayList *list);
//Free the memory from an array list
void al_destroy(ArrayList list);
