#include "sprite.h"

Sprite spr_new_static(TextureRegion region, Rect bounds) {
	return (Sprite) {
		.src = { .tex = region},
		.dynamic = false,
		.bounds = bounds,
		.transform = trans_new_identity()
	};
}

Sprite spr_new_animated(Animation anim, Rect bounds) {
	return (Sprite) {
		.src = {.anim = anim},
		.dynamic = true,
		.bounds = bounds,
		.transform = trans_new_identity()
	};
}

TextureRegion spr_image(Sprite spr) {
	if(spr.dynamic) {
		return anim_get_current(spr.src.anim);
	} else {
		return spr.src.tex;
	}
}

void spr_step(Sprite *spr) {
	if(spr->dynamic) {
		anim_step(&(spr->src.anim));
	}
}

