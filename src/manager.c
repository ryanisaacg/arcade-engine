#include "manager.h"

#include <stdlib.h>

AssetManager asset_new(Window window) {
	return (AssetManager) {
		.window = window,
		.data = hm_new()
	};
}

Texture asset_load(AssetManager assets, char *path) {
	int hash = path[0] + path[1] + path[2] + path[3];
	Texture *texture;
	if(hm_has(assets.data, hash, path)) {
		texture = hm_get(assets.data, hash, path);
	} else {
		texture = malloc(sizeof(Texture));
		*texture = texture_new(assets.window, path);
		hm_put(assets.data, hash, path, texture);
	}
	return *texture;
}

void asset_destroy(AssetManager assets) {
	ArrayList keys = hm_get_keys(assets.data);
	for(size_t i = 0; i < keys.length; i++) {
		char *path = al_get(keys, i);
		int hash = path[0] + path[1] + path[2] + path[3];
		Texture *texture = hm_get(assets.data, hash, path);
		texture_destroy(*texture);
	}
	hm_destroy(assets.data);
}
