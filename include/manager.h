#pragma once

#include "hashmap.h"
#include "texture.h"

struct AssetManager {
	Window window;
	HashMap *data;
};

AssetManager asset_new(Window window);
Texture asset_load(AssetManager assets, char *path);
void asset_destroy(AssetManager assets);
