#pragma once

#include "array_list.h"
#include <stdbool.h>

#define HASHMAP_ENTRY_LENGTH 1024

typedef bool (*ArbitraryEqualFunc)(void*, void*);

typedef struct HashMap {
	char *buffer;
	bool *has_buffer;
	size_t key_size, value_size, length, items;
	ArrayList keys;
	ArbitraryEqualFunc eq_func;
} HashMap;


HashMap hm_new(size_t key_size, size_t value_size);
HashMap hm_new_eqfunc(size_t key_size, size_t value_size, ArbitraryEqualFunc eq_func);
void hm_put(HashMap *map, int hash, void *key, void *value);
void *hm_get(HashMap map, int hash, void *key);
ArrayList hm_get_keys(HashMap map);
bool hm_has(HashMap map, int hash, void *key);
void hm_destroy(HashMap map);
