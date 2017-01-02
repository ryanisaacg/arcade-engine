#include "batch.h"

#include <stdlib.h>

typedef struct {
	Rect area, texture_area;
	Transform transform;
} BatchEntry;

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

Batch batch_new_default() {
	Shader vertex = shader_vertex_new(default_vertex);
	Shader fragment = shader_fragment_new(default_fragment);
	Program default_program = program_new(vertex, fragment, "out_color");
	program_add_attribute(default_program, "position", 2, 4, 0);
	program_add_attribute(default_program, "tex_position", 2, 4, 2);
	return batch_new(default_program);
}

/*
 * Shader programs should accept two 2d vectors: position and texture position; and a texture
 * They should be called 'position', 'tex_position', and 'image'
 */
Batch batch_new(Program program) {
	return batch_new_custom(program, NULL);
}

Batch batch_new_custom(Program program, BatchFunction func) {
	Batch batch;
	batch.textures = al_new(sizeof(Texture));
	batch.call_lists = hm_new();
	batch.function = func;
	batch.call = dc_new();
	batch.program = program;
	return batch;
}

Texture *batch_register(Batch *batch, Texture region) {
	al_add(&batch->textures, &region);
	Texture *texture = al_get(batch->textures, batch->textures.length - 1);
	ArrayList *list = malloc(sizeof(ArrayList));
	*list = al_new(sizeof(BatchEntry));
	hm_put(batch->call_lists, region.id, texture, list);
	return texture;
}

void batch_begin(Batch *batch) {
	for(size_t i = 0; i < batch->textures.length; i++) {
		Texture *region = al_get(batch->textures, i);
		ArrayList *list = hm_get(batch->call_lists, region->id, region);
		al_clear(list);
	}
}

void batch_add(Batch *batch, TextureRegion region, Rect target, Transform transform) {
	Texture tex = region.source;
	ArrayList *list = hm_get(batch->call_lists, region.source.id, &tex);
	BatchEntry entry = { target, region.region, transform };
	al_add(list, &entry);
}

void batch_end(Batch *batch) {
	if(batch->function != NULL) {
		batch->function(*batch);
		return;
	}
	for(size_t i = 0; i < batch->textures.length; i++) {
		dc_clear(&batch->call);
		Texture *tex = al_get(batch->textures, i);
		ArrayList *draws_ptr = hm_get(batch->call_lists, tex->id, tex);
		ArrayList draws = *draws_ptr;
		for(size_t j = 0; j < draws.length; j++) {
			BatchEntry *entry = al_get(draws, j);
			dc_add(&batch->call, entry->texture_area, entry->area, entry->transform);
		}
		texture_bind(*tex, batch->program, "image");
		dc_draw(batch->call);
	}
}

void batch_destroy(Batch batch) {
	al_destroy(batch.textures);
	hm_destroy(batch.call_lists);
	dc_destroy(batch.call);
	program_destroy(batch.program);
}
