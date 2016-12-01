#pragma once

#include "groups.h"
#include "lines.h"
#include "shape.h"

typedef struct {
	Shape bounds;
	Vector2 velocity, acceleration, max_velocity, drag;
	bool solid, alive;
	Group *group;
	void *data;
} ArcadeObject;

ArcadeObject arcobj_new(Shape bounds, bool solid, void *data);
bool arcobj_interacts(ArcadeObject *a, ArcadeObject *b);
