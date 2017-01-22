#pragma once

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
	size_t index;
} ArcadeObject;

/*
 * Create a new game object with a given world bound and sprite. Solid objects act like walls; other objects may not pass through them
 */
ArcadeObject arcobj_new(Shape bounds, bool solid, Sprite spr);
/*
 * Checks to see if two objects should interact, to allow things like bullets to ignore friendly units
 */
bool arcobj_interacts(ArcadeObject *a, ArcadeObject *b);
