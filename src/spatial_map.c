#include "spatial_map.h"

#include "collision.h"

static size_t get_index(SpatialMap *map, float x, float y) {
	int gridHeight = (int)(map->height / map->tile_height);
	int posX = (int)(x / map->tile_width);
	int posY = (int)(y / map->tile_height);
	return posX * gridHeight + posY;
}

static Rect get_region(SpatialMap map, float x, float y) {
	int gridX = (int)(x * map.tile_width) / (int)map.tile_width;
	int gridY = (int)(y * map.tile_height) / (int)map.tile_height;
	return rect_new(gridX, gridY, map.tile_width, map.tile_height);
}

SpatialMap sm_new(size_t item_size, float width, float height, float tile_width, float tile_height) {
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
void *sm_get(SpatialMap map, float x, float y) {
	size_t index = get_index(&map, x, y);
	bool *has = al_get(map.has, index);
	return (*has) ? al_get(map.items, index) : NULL;
}
bool sm_has(SpatialMap map, float x, float y) {
	size_t index = get_index(&map, x, y);
	bool *has = al_get(map.has, index);
	return *has;
}
bool sm_free(SpatialMap map, Shape shape) {
	Rect region = shape_bounding_box(shape);
	for(float x = region.x; x < region.x + region.width; x++) {
		for(float y = region.y; y < region.y + region.height; y++) {
			if(sm_has(map, x, y) && overlaps_shape(shape, shape_rect(get_region(map, x, y)))) {
				return false;
			}
		}
	}
	return true;
}
