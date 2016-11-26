#include "world.h"

World world_new(float width, float height, float qt_buckets_size) {

}

size_t world_add(World *world, ArcadeObject object) {
	return qt_add(&world->entities, object);
}

ArcadeObject *world_get(World world, size_t index) {
	return qt_get(world.entities, index);
}

size_t world_add_tilemap(World *world, TileMap map) {
	size_t index = world->layers.length;
	al_add(&world->layers, &map);
	return index;
}

TileMap *world_get_tilemap(World world, size_t index) {
	return al_get(world.layers, index);
}

ArcadeObject world_remove(World *world, size_t index) {
	return qt_remove(&world->entities, index);
}

void world_update(World world, float milliseconds, void (*update)(ArcadeObject*), void (*collision_func)(ArcadeObject*, ArcadeObject*)) {
	size_t length = qt_len(world.entities);
	if(update != NULL) {
		for(size_t i = 0; i < length; i++) {
			ArcadeObject *obj = qt_get(world.entities, i);
			//Accelerate the object
			Vector2 acceleration = vec2_scl(obj->acceleration, milliseconds);
			obj->velocity = vec2_add(acceleration, obj->velocity);
			//Move the object
			Vector2 velocity = vec2_scl(obj->velocity, milliseconds);
			Vector2 pos = shape_get_position(obj->bounds);
			shape_set_position(&obj->bounds, vec2_add(pos, velocity)); //TODO: Check against the tilemap
			update(qt_get(world.entities, i));
		}
	}
	if(collision_func != NULL) {
		qt_collisions(world.entities, collision_func);
	}
}
