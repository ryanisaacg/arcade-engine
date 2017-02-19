#include "hashmap.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

static bool void_ptr_equals(void *a, void *b);
static int get_hash_value(char *buffer, size_t index, size_t length);
static void *get_key_ptr(char *buffer, size_t index, size_t length);
static void *get_value_ptr(char *buffer, size_t index, size_t length, size_t key);

static bool void_ptr_equals(void *a, void *b) {
	return a == b;
}

HashMap hm_new(size_t key_size, size_t value_size) {
	return hm_new_eqfunc(key_size, value_size, void_ptr_equals);
}

HashMap hm_new_eqfunc(size_t key_size, size_t value_size, ArbitraryEqualFunc eq_func) {
	return (HashMap) {
		.buffer = malloc((sizeof(int) + key_size + value_size) * 32),
		.has_buffer = calloc(32, sizeof(bool)),
		.key_size = key_size,
		.value_size = value_size,
		.length = 32,
		.items = 0,
		.keys = al_new(key_size),
		.eq_func = eq_func
	};
}

void hm_put(HashMap *map, int hash, void *key, void *value) {
	size_t chunk_size = sizeof(int) + map->key_size + map->value_size; //the size of each hash entry in memory
	//if we should resize the array
	if(map->items * 2 > map->length) {
		map->length *= 2; //double the map capacity
		char *old = map->buffer; //store the old buffer temorarily
		map->buffer = malloc(map->length * chunk_size); 
		map->has_buffer = calloc(map->length, sizeof(bool));
		size_t length = map->length / 2;
		for(size_t i = 0; i < length; i++) {
			//put the old data into the new map
			hm_put(map, get_hash_value(old, i, chunk_size), get_key_ptr(old, i, chunk_size), get_value_ptr(old, i, chunk_size, map->key_size));
		}
		free(old);
	}
	int index = hash % map->length; //find the index to start at
	while(map->has_buffer[index]) {//move forward until a free spot is found
		index = (index + 1) % map->length; //wrap if necesary
	} 
	char *location = map->buffer + chunk_size * index; //find the start of the chunk
	//find and set the chunk data
	int *hash_location = (int*)location;
	void *key_location = location + sizeof(int);
	void *value_location = location + sizeof(int) + map->key_size;
	*hash_location = hash;
	memcpy(key_location, key, map->key_size);
	memcpy(value_location, value, map->key_size);
	map->has_buffer[index] = true; //mark the position as taken
	al_add(&(map->keys), key); //add the key to the key list
	map->items++; //increase the number of items
}

void *hm_get(HashMap map, int hash, void *key) {
	int index = hash % map.length;
	int start = index;
	size_t chunk_size = sizeof(int) + map.key_size + map.value_size;
	while(!map.eq_func(key, get_key_ptr(map.buffer, index, chunk_size))) {
		index = (index + 1) % map.length; 
		if(index == start) {
			return NULL;
		}
	}
	return get_value_ptr(map.buffer, index, chunk_size, map.key_size);
}

ArrayList hm_get_keys(HashMap map) {
	return map.keys;
}
bool hm_has(HashMap map, int hash, void *key) {
	return hm_get(map, hash, key) != NULL;
}

void hm_destroy(HashMap map) {
	free(map.buffer);
	free(map.has_buffer);
	al_destroy(map.keys);
}

static int get_hash_value(char *buffer, size_t index, size_t length) {
	int *hash = (int*)(buffer + index * length);
	return *hash;
}

static void *get_key_ptr(char *buffer, size_t index, size_t length) {
	return buffer + (index * length) + sizeof(int);
}

static void *get_value_ptr(char *buffer, size_t index, size_t length, size_t key) {
	return buffer + (index * length) + sizeof(int) + key;
}
