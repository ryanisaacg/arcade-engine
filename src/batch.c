#include "batch.h"

#include <stdlib.h>

const size_t sprite_vbo_size = 16; //2 floats per vertex for position, 2 for texture position (2 * 2 * 4)
const size_t sprite_ebo_size = 6; //2 triangles with 3 vertices each

const GLchar const *default_vertex = "#version 150 core\n"
	"in vec2 position;"
	"in vec2 tex_position;"
	"out vec2 Tex_position;"
	"void main() {"
	"	Tex_position = tex_position;"
	"	gl_Position = vec4(position, 0.0, 1.0);"
	"}";

const GLchar const *default_fragment = "#version 150 core\n"
	"in vec2 Tex_position;"
	"out vec4 out_color;"
	"uniform sampler2D image;"
	"void main() {"
	"	out_color = texture(image, Tex_position);"
	"}";

Batch batch_new() {
	Batch batch;
	//Generate and bind buffers
	glGenBuffers(1, &batch.vbo_id);
	glGenBuffers(1, &batch.ebo_id);
	glBindBuffer(GL_ARRAY_BUFFER, batch.vbo_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.ebo_id);
	//Load and compile shaders
	Shader vertex = shader_vertex_new(default_vertex);
	Shader fragment = shader_fragment_new(default_fragment);
	Program default_program = program_new(vertex, fragment, "out_color");
	program_add_attribute(default_program, "position", 2, 4, 0);
	program_add_attribute(default_program, "tex_position", 2, 4, 2);
	//Allocate buffers to an initial size
	batch.vbo_size = 64;
	batch.vbo = malloc(sizeof(GLfloat) * batch.vbo_size);
	batch.ebo_size = 24;
	batch.ebo = malloc(sizeof(GLuint) * batch.ebo_size);
	batch.num_sprites = 0;
	return batch;
}

size_t batch_add(Batch *batch, Sprite sprite) {

}
void batch_update(Batch *batch, Sprite sprite, size_t index) {

}

void batch_clear(Batch *batch) {

}

void batch_destroy(Batch batch) {
	free(batch.vbo);
	free(batch.ebo);
	glDeleteBuffers(1, &batch.vbo_id);
	glDeleteBuffers(1, &batch.ebo_id);
	program_destroy(batch.program);
}
