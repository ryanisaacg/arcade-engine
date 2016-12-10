#pragma once

#include "array_list.h"
#include "shape.h"
#include <stddef.h>

typedef struct {
	float width, height, tile_width, tile_height;
	ArrayList items;
	ArrayList has;
} SpatialMap;

SpatialMap sm_new(size_t item_size, float width, float height, float tile_width, float tile_height);
void sm_set(SpatialMap *map, void *item, float x, float y);
void sm_remove(SpatialMap *map, float x, float y);
void *sm_get(SpatialMap map, float x, float y);
bool sm_has(SpatialMap map, float x, float y);
bool sm_free(SpatialMap map, Shape region);
void sm_destroy(SpatialMap map);
