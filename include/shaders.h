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
void shader_destroy(Shader shader);

typedef struct {
	GLuint id;
	Shader vertex, fragment;
} Program;

Program program_new(Shader vertex, Shader fragment, char *out_color_name);
void program_add_attribute(	Program shader, const GLchar const *name, int members, 
							int distance_between, int start_offset);
void program_destroy(Program program);
