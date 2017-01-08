#pragma once

#include <GL/glew.h>

#include "shaders.h"
#include "sprite.h"


typedef struct {
	GLfloat *vbo; //vertex buffer object, a way to draw the vertices with OpenGL
	size_t vbo_size;
	GLuint *ebo; //element buffer object, a way to re-use vertices with OpenGL
	size_t ebo_size;
	GLuint num_sprites, vbo_id, ebo_id;
	Program program;
} Batch;

Batch batch_new();
void batch_bind(Texture atlas);
size_t batch_add(Batch *batch, Sprite sprite);
void batch_update(Batch *batch, Sprite sprite, size_t index);
void batch_draw(Batch batch);
void batch_clear(Batch *batch);
void batch_destroy(Batch batch);
