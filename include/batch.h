#pragma once

#include "draw-call.h"
#include "hashmap.h"
#include "texture-region.h"

typedef void (*BatchFunction)(ArrayList, HashMap*, DrawCall*);

typedef struct {
	ArrayList textures;
	BatchFunction function;
	HashMap *call_lists;
	DrawCall call;
} Batch;


Batch batch_new();
Batch batch_new_custom(BatchFunction func);
Texture *batch_register(Batch *batch, Texture region);
void batch_begin(Batch *batch);
void batch_add(Batch *batch, TextureRegion texture, Rect target, Transform transform);
void batch_end(Batch *batch);

