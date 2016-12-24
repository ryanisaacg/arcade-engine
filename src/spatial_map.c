#include "spatial_map.h"

#include "collision.h"

static size_t get_index(SpatialMap *map, float x, float y) {
	int gridHeight = (int)(map->height / map->tile_height);
	int posX = (int)(x / map->tile_width);
	int posY = (int)(y / map->tile_height);
	return posX * gridHeight + posY;
}

SpatialMap sm_new(size_t item_size, float width, float height, 
									float tile_width, float tile_height) {
	SpatialMap map;
	map.width 		= width;
	map.height		= height;
	map.tile_width 	= tile_width;
	map.tile_height = tile_height;
	size_t last 	= get_index(&map, width, height);
	map.items 		= al_new_sized(item_size, last);
	map.has 		= al_new_sized(item_size, last);
	bool tmp = false;
	for(size_t i = 0; i < last; i++) {
		al_set(&map.has, i, &tmp);
	}
	return map;
}
void sm_set(SpatialMap *map, void *item, float x, float y) {
	size_t index = get_index(map, x, y);
	al_set(&map->items, index, item);
	bool tmp = true;
	al_set(&map->has, index, &tmp);
}
void sm_remove(SpatialMap *map, float x, float y) {
	size_t index = get_index(map, x, y);
	bool tmp = false;
	al_set(&map->has, index, &tmp);
}
bool sm_contains(SpatialMap map, float x, float y) {
	return x >= 0 && y >= 0 && x < map.width && y < map.height; 
}
void *sm_get(SpatialMap map, float x, float y) {
	if(sm_contains(map, x, y)) {
		size_t index = get_index(&map, x, y);
		bool *has = al_get(map.has, index);
		return (*has) ? al_get(map.items, index) : NULL;
	} else {
		return NULL;
	}
}
bool sm_has(SpatialMap map, float x, float y) {
	if(sm_contains(map, x, y)) {
		size_t index = get_index(&map, x, y);
		bool *has = al_get(map.has, index);
		return *has;
	} else {
		return true;
	}
}
bool sm_free(SpatialMap map, Shape shape) {
	Rect region = shape_bounding_box(shape);
	if(!sm_contains(map, region.x, region.y) || !sm_contains(map, region.x + region.width, region.y + region.height))
		return false;
	for(float x = region.x; x < region.x + region.width; x++) {
		for(float y = region.y; y < region.y + region.height; y++) {
			if(sm_has(map, x, y)) {
				return false;
			}
		}
	}
	return true;
}

void sm_destroy(SpatialMap map) {
	al_destroy(map.items);
	al_destroy(map.has);
}
