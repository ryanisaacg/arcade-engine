#include "animation.h"

Sprite anim_new(size_t steps) {
	return anim_new_al(al_new(sizeof(TextureRegion)), steps);
}

Sprite anim_new_al(ArrayList frames, size_t steps) {
	return (Sprite) { frames, 0, steps, 0, trans_new_identity() };
}

Sprite anim_new_buffer(TextureRegion *frames, size_t num_frames, size_t steps) {
	return anim_new_al(al_prealloc(sizeof(TextureRegion), frames, num_frames), steps);
}

void anim_add_frame(Sprite *anim, TextureRegion texture) {
	al_add(&anim->frames, &texture);
}

TextureRegion anim_current_frame(Sprite anim) {
	TextureRegion *current = al_get(anim.frames, anim.current_frame);
	return *current;
}

void anim_step(Sprite anim) {
	anim.current_steps++;
	if(anim.current_steps >= anim.steps_per_frame) {
		anim.current_steps = 0; 
		anim.current_frame++;
		if(anim.current_frame >= anim.frames.length) {
			anim.current_frame = 0;
		}
	}
}
