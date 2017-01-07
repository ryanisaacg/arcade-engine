#pragma once

#include "sprite.h"
#include "draw-call.h"
#include "hashmap.h"
#include "texture-region.h"

struct Batch;
typedef struct Batch Batch;

typedef void (*BatchFunction)(Batch);

struct Batch {
	ArrayList textures;
	BatchFunction function;
	HashMap *call_lists;
	DrawCall call;
	Program program;
};

Batch batch_new_default();
Batch batch_new(Program program);
Batch batch_new_custom(Program program, BatchFunction func);
Texture *batch_register(Batch *batch, Texture region);
void batch_begin(Batch *batch);
void batch_add(Batch *batch, TextureRegion texture, Rect target, Transform transform);
void batch_add_sprite(Batch *batch, Sprite spr, Rect target);
void batch_end(Batch *batch);
void batch_destroy(Batch batch);
