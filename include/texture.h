#pragma once

#include <SDL.h>

typedef struct {
	int width, height;
	SDL_Texture *texture;
} Texture;

#include "window.h"

Texture texture_new(Window window, char *texture_path);
void texture_destroy(Texture tex);
