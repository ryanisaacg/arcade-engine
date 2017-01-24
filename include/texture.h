#pragma once

#include <SDL.h>

struct Texture {
	int width, height;
	SDL_Texture *texture;
};

#include "window.h"

Texture texture_new(Window window, char *texture_path);
void texture_destroy(Texture tex);
