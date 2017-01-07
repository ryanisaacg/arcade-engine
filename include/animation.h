#pragma once

#include <stddef.h>

#include "array_list.h"
#include "texture-region.h"

typedef struct {
	ArrayList frames;
	size_t current_steps, steps_per_frame, current_frame;
	Transform transform;
} Sprite;

Sprite spr_new(size_t steps_per_frame);
Sprite spr_new_al(ArrayList frames, size_t steps_per_frame);
Sprite spr_new_buffer(TextureRegion *frames, size_t num_frames, size_t steps_per_frame);
void spr_set_transform(Sprite *spr, Transform trans);
void spr_add_frame(Sprite *spr, TextureRegion texture);
TextureRegion spr_current_frame(Sprite spr);
void spr_step(Sprite spr);
