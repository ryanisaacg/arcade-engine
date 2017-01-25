#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "array_list.h"

#include "forward-decl.h"

struct Sprite {
	union {
		Animation anim;
		TextureRegion tex;
	} src;
	bool dynamic;
	Vector2 position, origin, scale;
	float angle;
	bool flip_x, flip_y;
};

#include "animation.h"
#include "rect.h"
#include "texture-region.h"
#include "transform.h"

Sprite spr_new_static(TextureRegion region, Vector2 pos);
Sprite spr_new_animated(Animation anim, Vector2 pos);
TextureRegion spr_image(Sprite spr);
void spr_step(Sprite *spr);
void spr_center_origin(Sprite *spr);
