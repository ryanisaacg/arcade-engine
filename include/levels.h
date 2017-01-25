#pragma once

#include "forward-decl.h"

struct Level {
	World data;
	bool persistent;
};

#include "manager.h"
#include "world.h"

Level level_load(char *filename, AssetManager assets, size_t data_size);
World level_get_world(Level level);
void level_destroy(Level level);
