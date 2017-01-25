#pragma once

#include "hashmap.h"

#include "forward-decl.h"

struct AssetManager {
	Window window;
	HashMap *data;
};

#include "texture.h"

AssetManager asset_new(Window window);
Texture asset_load(AssetManager assets, char *path);
void asset_destroy(AssetManager assets);
