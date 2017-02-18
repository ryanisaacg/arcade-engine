#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "geom.h"

bool equal_float(float a, float b);
char *read_file(char *filename);
Rect sdl_rect_conv(SDL_Rect rect);
SDL_Rect rect_conv(Rect rect);
float random_float(float min, float max);
inline bool str_equal(void *a, void *b) {
	char **a_ = a;
	char **b_ = b;
	return strcmp(*a_, *b_) == 0;
}
inline bool predicate(bool p, bool q) {
	return !p || q;
}
