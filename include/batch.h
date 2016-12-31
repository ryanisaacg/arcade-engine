#pragma once

#include "draw-call.h"

typedef void (*BatchFunction)(ArrayList, ArrayList);

typedef struct {
	ArrayList textures, instances;
	BatchFunction function;
} Batch;


Batch batch_new();
Batch batch_new_custom(BatchFunction func);
void batch_begin(Batch *batch);
void batch_add(Batch *batch, TextureRegion texture, Rect target, Transform transform);
void batch_end(Batch *batch);
