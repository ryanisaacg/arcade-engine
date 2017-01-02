#pragma once

#include <GL/glew.h>

#include "shaders.h"

typedef struct {
	int width, height;
	GLuint id;
} Texture;

Texture texture_new(char *texture_path);
void texture_bind(Texture tex, Program prog, char *name);
void texture_destroy(Texture tex);
