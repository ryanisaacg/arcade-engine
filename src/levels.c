#include "levels.h"

#include <tmx.h>

//The side of a quad tree bucket square at the smallest level
#define QT_BUCKETS_SIZE 64

Level level_load(char *filename, AssetManager assets, size_t data_size) {
	tmx_map *map = tmx_load(filename);
	if(map == NULL) {
		tmx_perror("Loading the TileMap");
		exit(1);
	}
	World world = world_new(NULL, map->width, map->height, 64, data_size);
	world.r = map->backgroundcolor & 0xff;
	world.g = (map->backgroundcolor >> 8) & 0xff;
	world.b = (map->backgroundcolor >> 16) & 0xff;

}

World level_get_world(Level level) {

}

void level_destroy(Level level) {
	world_destroy(level.data);
}
