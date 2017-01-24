#pragma once

#include "manager.h"
#include "world.h"

struct Level {
	World data;
	bool persistent;
};

Level level_load(char *filename, AssetManager assets, size_t data_size);
World level_get_world(Level level);
void level_destroy(Level level);
