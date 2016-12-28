#pragma once

#include <GL/glew.h>

/*
 * Shader programs should accept two 2d vectors: position and texture position, and a texture
 * They should be called 'position', 'tex_position', and 'texture'
 */

typedef struct {
	GLuint id;
} Shader;

Shader shader_vertex_new(const GLchar const *data);
Shader shader_fragment_new(const GLchar const *data);
Shader shader_program_new(Shader vertex, Shader fragment);
void shader_add_attribute(const GLchar const *name, int members, int distance_between, int start_offset);
