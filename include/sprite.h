#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "animation.h"
#include "array_list.h"
#include "rect.h"
#include "texture-region.h"
#include "transform.h"

typedef struct Sprite {
	union {
		Animation anim;
		TextureRegion tex;
	} src;
	bool dynamic;
	Rect bounds;
	float angle;
	bool flip_x, flip_y;
	Vector2 origin;
} Sprite;

Sprite spr_new_static(TextureRegion region, Rect bounds);
Sprite spr_new_animated(Animation anim, Rect bounds);
TextureRegion spr_image(Sprite spr);
void spr_step(Sprite *spr);
