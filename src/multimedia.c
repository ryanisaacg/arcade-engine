#include "multimedia.h"
#include "geom.h"
#include "simulation.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


void multimedia_init() {
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		exit(-1);
	}
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if(IMG_Init(flags) != flags) {
		fprintf(stderr, "SDL_Image could not initialize. IMG Error: %s\n", IMG_GetError());
		exit(-1);
	}
	flags = MIX_INIT_MP3 | MIX_INIT_OGG;
	if(Mix_Init(flags) != flags) {
		fprintf(stderr, "SDL_mixer could not initialize. MIX Error: %s\n", Mix_GetError());
		exit(-1);
	}
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		fprintf(stderr, "SDL_mixer could not connect to audio drivers. MIX Error: %s\n", Mix_GetError());
		exit(-1);
	}
	if(SDL_JoystickEventState(SDL_ENABLE) <= 0) {
		fprintf(stderr, "SDL could not initialize game controllers. SDL Error: %s\n", SDL_GetError());
	}
}

void multimedia_quit() {
	SDL_Quit();
	IMG_Quit();
	Mix_CloseAudio();
	Mix_Quit();
}


AssetManager asset_new(Window window) {
	return (AssetManager) {
		.window = window,
		.data = hm_new()
	};
}

Texture asset_load_texture(AssetManager assets, char *path) {
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

Music asset_load_music(AssetManager assets, char *path) {
	int hash = path[0] + path[1] + path[2] + path[3];
	Music *music;
	if(hm_has(assets.data, hash, path)) {
		music = hm_get(assets.data, hash, path);
	} else {
		music = malloc(sizeof(Music));
		*music = music_new(path);
		hm_put(assets.data, hash, path, music);
	}
	return *music;
}

Sound asset_load_sound(AssetManager assets, char *path) {
	int hash = path[0] + path[1] + path[2] + path[3];
	Sound *sound;
	if(hm_has(assets.data, hash, path)) {
		sound = hm_get(assets.data, hash, path);
	} else {
		sound = malloc(sizeof(Sound));
		*sound = sound_new(path);
		hm_put(assets.data, hash, path, sound);
	}
	return *sound;
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
