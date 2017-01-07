#include "animation.h"

Sprite spr_new(size_t steps) {
	return spr_new_al(al_new(sizeof(TextureRegion)), steps);
}

Sprite spr_new_al(ArrayList frames, size_t steps) {
	return (Sprite) { frames, 0, steps, 0, trans_new_identity() };
}

Sprite spr_new_buffer(TextureRegion *frames, size_t num_frames, size_t steps) {
	return spr_new_al(al_prealloc(sizeof(TextureRegion), frames, num_frames), steps);
}

void spr_add_frame(Sprite *spr, TextureRegion texture) {
	al_add(&spr->frames, &texture);
}

TextureRegion spr_current_frame(Sprite spr) {
	TextureRegion *current = al_get(spr.frames, spr.current_frame);
	return *current;
}

void spr_step(Sprite spr) {
	spr.current_steps++;
	if(spr.current_steps >= spr.steps_per_frame) {
		spr.current_steps = 0; 
		spr.current_frame++;
		if(spr.current_frame >= spr.frames.length) {
			spr.current_frame = 0;
		}
	}
}
