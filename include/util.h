#pragma once

#include <SDL.h>
#include <stdbool.h>

#include "types.h"

bool equal_float(float a, float b);
char *read_file(char *filename);
Rect sdl_rect_conv(SDL_Rect rect);
SDL_Rect rect_conv(Rect rect);
