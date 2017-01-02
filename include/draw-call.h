#pragma once

#include <GL/glew.h>

#include "array_list.h"
#include "rect.h"
#include "transform.h"

typedef struct {
	ArrayList vertex_buffer, element_buffer;
	GLuint elements, vbo_id, ebo_id;
} DrawCall;

DrawCall dc_new();
void dc_clear(DrawCall *call);
void dc_add_manual(DrawCall *call, GLfloat *vertices, size_t vertex_size, 
									GLuint *elements, size_t num_elements);
void dc_add(DrawCall *call, Rect texture_source, Rect area, Transform transform);
void dc_draw(DrawCall call);
void dc_destroy(DrawCall call);
