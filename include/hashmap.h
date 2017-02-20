#pragma once

#include "array_list.h"
#include <stdbool.h>

#define HASHMAP_ENTRY_LENGTH 1024

typedef bool (*ArbitraryEqualFunc)(void*, void*);

/**
 *
 */
typedef struct HashMap {
	int *hashes;
	char *keys;
	char *values;
	bool *has;
	size_t key_size, value_size, length, items;
	ArrayList keys_list;
	ArbitraryEqualFunc eq_func;
} HashMap;


HashMap hm_new(size_t key_size, size_t value_size);
HashMap hm_new_eqfunc(size_t key_size, size_t value_size, ArbitraryEqualFunc eq_func);
HashMap hm_new_sized(size_t key_size, size_t value_size, ArbitraryEqualFunc eq_func, size_t size);
void hm_put(HashMap *map, int hash, void *key, void *value);
void *hm_get(HashMap map, int hash, void *key);
ArrayList hm_get_keys(HashMap map);
bool hm_has(HashMap map, int hash, void *key);
void hm_destroy(HashMap map);
