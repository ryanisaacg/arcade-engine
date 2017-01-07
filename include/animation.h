#pragma once

#include "array_list.h"
#include "texture-region.h"

typedef struct {
	ArrayList frames;
	size_t current_steps, steps_per_frame, current_frame;
} Animation;

Animation anim_new(TextureRegion *frames, size_t num_frames, size_t steps_per_frame);
TextureRegion anim_step(Animation *anim);
