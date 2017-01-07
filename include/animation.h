#pragma once

#include <stddef.h>

#include "array_list.h"
#include "texture-region.h"

typedef struct {
	ArrayList frames;
	size_t current_steps, steps_per_frame, current_frame;
	Transform transform;
} Sprite;

Sprite anim_new(size_t steps_per_frame);
Sprite anim_new_al(ArrayList frames, size_t steps_per_frame);
Sprite anim_new_buffer(TextureRegion *frames, size_t num_frames, size_t steps_per_frame);
void anim_set_transform(Sprite *anim, Transform trans);
void anim_add_frame(Sprite *anim, TextureRegion texture);
TextureRegion anim_current_frame(Sprite anim);
void anim_step(Sprite anim);
