#include "animation.h"

Animation anim_new(TextureRegion *frames, size_t num_frames, size_t steps_per_frame) {
	return (Animation) {
		.frames = al_prealloc(sizeof(TextureRegion), frames, num_frames),
		.current_steps = 0,
		.steps_per_frame = steps_per_frame,
		.current_frame = 0 
	};
}

Animation anim_new_spritesheet(TextureRegion source, int tile_width, int tile_height, int frame_order[][2], size_t frames, size_t steps) {
	TextureRegion *frame_buffer = malloc(sizeof(TextureRegion) * frames);
	for(size_t i = 0; i < frames; i++) {
		int x = frame_order[i][0] * tile_width;
		int y = frame_order[i][1] * tile_height;
		TextureRegion frame = texregion_slice(source, rect_new(x, y, tile_width, tile_height));
		frame_buffer[i] = frame;
	}
	return anim_new(frame_buffer, frames, steps);
}

TextureRegion anim_get_current(Animation anim) {
	TextureRegion *current = al_get(anim.frames, anim.current_frame);
	return *current;
}

TextureRegion anim_step(Animation *anim) {
	//Advance a step (and a frame if necessary)
	anim->current_steps++;
	if(anim->current_steps >= anim->steps_per_frame) {
		anim->current_steps = 0;
		anim->current_frame = (anim->current_frame + 1) % anim->frames.length;
	}
	return anim_get_current(*anim);
}
