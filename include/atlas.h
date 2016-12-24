#pragma once

#include "shaders.h"

typedef struct {
	int width, height;
} TextureAtlas;

TextureAtlas atlas_new(char *filename, Shader shader);
