#include "batch.h"

#include <stdlib.h>

typedef struct {
	Rect area, texture_area;
	Transform transform;
} BatchEntry;

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
		(batch->function)(batch->textures, batch->call_lists, &batch->call);
		return;
	}
	for(size_t i = 0; i < batch->textures.length; i++) {
		dc_clear(&batch->call);

	}
}

