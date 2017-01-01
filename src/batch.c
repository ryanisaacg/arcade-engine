#include "batch.h"

#include <stdlib.h>

typedef struct {
	Rect area;
	Transform transform;
} BatchEntry;

Batch batch_new() {
	return batch_new_custom(NULL);
}

Batch batch_new_custom(BatchFunction func) {
	Batch batch;
	batch.textures = al_new(sizeof(TextureRegion));
	batch.call_lists = hm_new();
	batch.function = func;
	batch.call = dc_new();
	return batch;
}

TextureRegion *batch_register(Batch *batch, TextureRegion region) {
	al_add(&batch->textures, &region);
	TextureRegion *texture = al_get(batch->textures, batch->textures.length - 1);
	ArrayList *list = malloc(sizeof(ArrayList));
	*list = al_new(sizeof(BatchEntry));
	hm_put(batch->call_lists, region.source.id, texture, list);
}

void batch_begin(Batch *batch) {
	for(size_t i = 0; i < batch->textures.length; i++) {
		TextureRegion *region = al_get(batch->textures, i);
		ArrayList *list = hm_get(batch->call_lists, region->source.id, region);
		al_clear(list);
	}
}

void batch_add(Batch *batch, TextureRegion *region, Rect target, Transform transform) {
	ArrayList *list = hm_get(batch->call_lists, region->source.id, region);
	BatchEntry entry = { target, transform };
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

