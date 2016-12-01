#include "arcobj.h"
#include <stddef.h>

ArcadeObject arcobj_new(Shape bounds, bool solid, void *data) {
	return (ArcadeObject) { bounds, vec2_new(0, 0), vec2_new(0, 0), vec2_new(0, 0), vec2_new(0, 0), solid, true, NULL, data };
}

bool arcobj_interacts(ArcadeObject *a, ArcadeObject *b) {
	return group_interacts(a->group, b->group);
}
