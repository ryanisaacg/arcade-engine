#pragma once

#include "manager.h"
#include "world.h"

typedef struct {
	World data;
	bool persistent;
} Level;

Level level_load(char *filename, AssetManager assets, size_t data_size);
World level_get_world(Level level);
void level_destroy(Level level);
