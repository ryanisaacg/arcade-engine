#include "batch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

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
	//Allocate buffers to an initial size
	batch.vbo_size = 64;
	batch.vbo = malloc(sizeof(GLfloat) * batch.vbo_size);
	batch.ebo_size = 24;
	batch.ebo = malloc(sizeof(GLuint) * batch.ebo_size);
	batch.num_sprites = 0;
	//Generate and bind buffers
	GLuint vao;
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &batch.vbo_id);
	print_gl_error("Batch VBO generation");
	glGenBuffers(1, &batch.ebo_id);
	//Load and compile shaders
	Shader vertex = shader_vertex_new(default_vertex);
	Shader fragment = shader_fragment_new(default_fragment);
	Program default_program = program_new(vertex, fragment, "out_color");
	batch.program = default_program;
	return batch;
}

size_t batch_add(Batch *batch, Sprite sprite) {
	//Expand buffers if necessary
	if((batch->num_sprites + 1) * sprite_vbo_size < batch->vbo_size) {
		batch->vbo_size *= 2;
		batch->ebo_size *= 2;
		batch->vbo = realloc(batch->vbo, batch->vbo_size);
		batch->ebo = realloc(batch->ebo, batch->ebo_size);
	}
	batch_update(batch, sprite, batch->num_sprites);
	batch->num_sprites += 1;
	return batch->num_sprites;
}

void batch_update(Batch *batch, Sprite sprite, size_t index) {
	//Get the insertion points of the data
	GLfloat *current_vbo = batch->vbo + index * sprite_vbo_size;
	GLuint *current_ebo = batch->ebo + index * sprite_ebo_size;
	//Add the data to the vbo
	for(int i = 0; i <= 1; i++) {
		for(int j = 0; j <= 1; j++) {
			*(current_vbo++) = sprite.bounds.x + sprite.bounds.width * i;
			*(current_vbo++) = sprite.bounds.y + sprite.bounds.height * j;
			TextureRegion tex = spr_image(sprite);
			*(current_vbo++) = tex.region.x + tex.region.width * i;
			*(current_vbo++) = tex.region.y + tex.region.height * j;
		}
	}
	GLuint num = index * 4;
	GLuint ebo_data[] = { num, num + 1, num + 2, num + 2, num + 3, num };
	memcpy(current_ebo, ebo_data, sprite_ebo_size);
}

void batch_draw(Batch batch) {
	glBindBuffer(GL_ARRAY_BUFFER, batch.vbo_id);
	glBufferData(GL_ARRAY_BUFFER, batch.vbo_size, batch.vbo, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, batch.ebo_size, batch.ebo, GL_STATIC_DRAW);
	program_add_attribute(batch.program, "position", 2, 4, 0);
	program_add_attribute(batch.program, "tex_position", 2, 4, 2);
	glDrawElements(GL_TRIANGLES, batch.num_sprites, GL_UNSIGNED_INT, 0);
}


void batch_clear(Batch *batch) {
	batch->num_sprites = 0;
}

void batch_destroy(Batch batch) {
	free(batch.vbo);
	free(batch.ebo);
	glDeleteBuffers(1, &batch.vbo_id);
	glDeleteBuffers(1, &batch.ebo_id);
	program_destroy(batch.program);
}
