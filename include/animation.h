#pragma once

#include "array_list.h"
#include "texture-region.h"

typedef struct {
	ArrayList frames;
	size_t current_steps, steps_per_frame, current_frame;
} Animation;

Animation anim_new(TextureRegion *frames, size_t num_frames, size_t steps_per_frame);
Animation anim_new_spritesheet(TextureRegion source, int tile_width, int tile_height, int frame_order[][2], size_t frames, size_t steps_per_frame);
TextureRegion anim_get_current(Animation anim);
TextureRegion anim_step(Animation *anim);
