#pragma once

#include <stddef.h>

#include "array_list.h"
#include "texture-region.h"

typedef struct {
	ArrayList frames;
	size_t current_steps, steps_per_frame, current_frame;
} Animation;

Animation anim_new(size_t steps_per_frame);
Animation anim_new_al(ArrayList frames, size_t steps_per_frame);
Animation anim_new_buffer(TextureRegion *frames, size_t num_frames, size_t steps_per_frame);
void anim_add_frame(Animation *anim, TextureRegion texture);
TextureRegion anim_current_frame(Animation anim);
void anim_step(Animation anim);
