#pragma once

#include <GL/glew.h>

typedef struct {
	int width, height;
	GLuint id;
} Texture;

Texture texture_new(char *texture_path, char *bind_name);
void texture_bind(Texture tex, Shader shader);
