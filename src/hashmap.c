#include "hashmap.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

static bool void_ptr_equals(void *a, void *b);

static bool void_ptr_equals(void *a, void *b) {
	return a == b;
}

HashMap hm_new(size_t key_size, size_t value_size) {
	return hm_new_eqfunc(key_size, value_size, void_ptr_equals);
}

HashMap hm_new_eqfunc(size_t key_size, size_t value_size, ArbitraryEqualFunc eq_func) {
	return hm_new_sized(key_size, value_size, eq_func, 32);
}

HashMap hm_new_sized(size_t key_size, size_t value_size, ArbitraryEqualFunc eq_func, size_t size) {
	return (HashMap) {
		.has = calloc(sizeof(int), size),
		.hashes = calloc(sizeof(int), size),
		.keys = malloc(key_size * size),
		.values = malloc(value_size * size),
		.key_size = key_size,
		.value_size = value_size,
		.length = size,
		.items = 0,
		.keys_list = al_new(key_size),
		.eq_func = eq_func
	};
}

void hm_put(HashMap *map, int hash, void *key, void *value) {
	//if we should resize the array
	if(map->items * 2 > map->length) {
		size_t length = map->length * 2;
		HashMap new = hm_new_sized(map->key_size, map->value_size, map->eq_func, length);
		hm_destroy(*map);
		*map = new;
	}
	int index = hash % map->length; //find the index to start at
	while(map->has[index]) {//move forward until a free spot is found
		index = (index + 1) % map->length; //wrap if necesary
	}
	map->hashes[index] = hash;
	map->has[index] = true;
	memcpy(map->keys + index * map->key_size, key, map->key_size);
	memcpy(map->values + index * map->value_size, value, map->value_size);
	al_add(&(map->keys_list), key); //add the key to the key list
	map->items++; //increase the number of items
}

void *hm_get(HashMap map, int hash, void *key) {
	int index = hash % map.length;
	int start = index;
	while(!map.eq_func(key, map.keys + index * map.key_size)) {
		index = (index + 1) % map.length; 
		if(index == start) {
			return NULL;
		}
	}
	return map.values + index * map.value_size;
}

ArrayList hm_get_keys(HashMap map) {
	return map.keys_list;
}
bool hm_has(HashMap map, int hash, void *key) {
	return hm_get(map, hash, key) != NULL;
}

void hm_destroy(HashMap map) {
	free(map.hashes);
	free(map.keys);
	free(map.values);
	free(map.has);
	al_destroy(map.keys_list);
}

