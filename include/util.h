#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "geom.h"

bool equal_float(float a, float b);
char *read_file(char *filename);
Rect sdl_rect_conv(SDL_Rect rect);
SDL_Rect rect_conv(Rect rect);
float random_float(float min, float max);
bool str_equal(void *a, void *b);
inline bool predicate(bool p, bool q) {
	return !p || q;
}
