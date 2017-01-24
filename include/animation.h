#pragma once

#include "array_list.h"
#include "texture-region.h"

struct Animation {
	ArrayList frames;
	size_t current_steps, steps_per_frame, current_frame;
};

/*
 * Creates a new Animation from a buffer
 */
Animation anim_new(TextureRegion *frames, size_t num_frames, size_t steps_per_frame);
/*
 * Creates an animation from a spritesheet
 * Frame Order is an array of points where the point is on the grid of sides tile_width, tile_height
 */
Animation anim_new_spritesheet(TextureRegion source, int tile_width, int tile_height, int frame_order[][2], size_t frames, size_t steps_per_frame);
/*
 * Creates an animation from a spritesheet laid out left to right sequentially
 */
Animation anim_new_linear_sheet(TextureRegion source, int tile_width, size_t steps_per_frame);
/*
 * Get the current frame of the animation
 */
TextureRegion anim_get_current(Animation anim);
/*
 * Step the animation forward and return the current frame
 */
TextureRegion anim_step(Animation *anim);
