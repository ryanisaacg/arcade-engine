#pragma once

#include "batch.h"
#include "groups.h"
#include "lines.h"
#include "shape.h"
#include "sprite.h"

typedef struct {
	Sprite sprite;
	Shape bounds;
	Vector2 velocity, acceleration, max_velocity, drag;
	bool solid, alive, bounce;
	Group *group;
	size_t index, sprite_index;
} ArcadeObject;

ArcadeObject arcobj_new(Shape bounds, bool solid, Sprite spr, Batch *batch);
bool arcobj_interacts(ArcadeObject *a, ArcadeObject *b);
