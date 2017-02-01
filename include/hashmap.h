#pragma once

#include "array_list.h"
#include <stdbool.h>

#define HASHMAP_ENTRY_LENGTH 1024

typedef struct HashEntry {
	int hash;
	void *key, *value;
} HashEntry;
typedef struct HashMap {
	ArrayList entries[HASHMAP_ENTRY_LENGTH];
	ArrayList keys;
	bool (*eq)(void *a, void *b);
} HashMap;

HashMap *hm_new();
HashMap *hm_new_eqfunc(bool (*eq)(void*, void*));
void hm_put(HashMap *map, int hash, void *key, void *value);
void *hm_get(HashMap *map, int hash, void *key);
ArrayList hm_get_keys(HashMap *map);
bool hm_has(HashMap *map, int hash, void *key);
void hm_destroy(HashMap *map);
