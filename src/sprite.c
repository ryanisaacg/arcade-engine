#include "geom.h"
#include "graphics.h"

Sprite spr_new_static(TextureRegion region, Vector2 pos) {
	return (Sprite) {
		.src = { .tex = region},
		.dynamic = false,
		.position = pos,
		.origin = vec2_new(0, 0),
		.scale = vec2_new(1, 1)
	};
}

Sprite spr_new_animated(Animation anim, Vector2 pos) {
	return (Sprite) {
		.src = {.anim = anim},
		.dynamic = true,
		.position = pos,
		.origin = vec2_new(0, 0),
		.scale = vec2_new(1, 1)
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

void spr_center_origin(Sprite *spr) {
	TextureRegion region = spr_image(*spr);
	spr->origin = vec2_new(region.region.width / 2, region.region.height / 2);
}
