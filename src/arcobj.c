#include "arcobj.h"
#include <stddef.h>

ArcadeObject arcobj_new(Shape bounds, bool solid, Sprite spr) {
	return (ArcadeObject) { 
		.sprite = spr,
		.bounds = bounds, 
		.velocity = vec2_new(0, 0), 
		.acceleration = vec2_new(0, 0), 
		.max_velocity = vec2_new(-1, -1), 
		.drag = vec2_new(0, 0), 
		.solid = solid, 
		.alive = true, 
		.bounce = false, 
		.group = NULL,
	};
}

bool arcobj_interacts(ArcadeObject *a, ArcadeObject *b) {
	return group_interacts(a->group, b->group);
}
