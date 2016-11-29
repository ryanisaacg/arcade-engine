#include "world.h"

World world_new(float width, float height, float qt_buckets_size) {
	World world;
	world.entities = qt_new(width, height, qt_buckets_size, qt_buckets_size);
	world.layers = al_new(sizeof(TileMap));
	return world;
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

bool world_point_free(World world, Vector2 point, ArcadeObject *ignore) {
	for(size_t i = 0; i < world.layers.length; i++) {
		TileMap *map = al_get(world.layers, i);
		if(!tl_free(*map, (int)point.x, (int)point.y))
			return false;
	}
	ArcadeObject *query = qt_point_query(world.entities, point);
	return query == NULL || query == ignore;
}

bool world_region_free(World world, Shape region, ArcadeObject *ignore) {
	Rect bounds = shape_bounding_box(region);
	for(size_t i = 0; i < world.layers.length; i++) {
		TileMap *map = al_get(world.layers, i);
		if(!tl_empty(*map, (int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height))
			return false;
	}
	ArcadeObject *query = qt_region_query(world.entities, region);
	return query == NULL || query == ignore;
}

void world_update(World world, float milliseconds, void (*update)(ArcadeObject*), void (*collision_func)(ArcadeObject*, ArcadeObject*)) {
	size_t length = qt_len(world.entities);
	if(update != NULL) {
		for(size_t i = 0; i < length; i++) {
			ArcadeObject *obj = qt_get(world.entities, i);
			//Apply the custom update
			update(qt_get(world.entities, i));
			//If this object is already entangled with another, skip the physics
			if(!world_region_free(world, obj->bounds, obj)) continue;
			//Accelerate the object
			Vector2 acceleration = vec2_scl(obj->acceleration, milliseconds);
			obj->velocity = vec2_add(acceleration, obj->velocity);
			//Determine the velocity of the object
			Vector2 velocity = vec2_scl(obj->velocity, milliseconds);
			Vector2 pos = shape_get_position(obj->bounds);
			//Move the object to a free space
			Vector2 attempt = vec2_add(pos, velocity);
			Vector2 unit = vec2_nor(attempt);
			shape_set_position(&obj->bounds, attempt);
			while(!world_region_free(world, obj->bounds, obj) && vec2_len2(attempt) > 1) {
				attempt = vec2_sub(attempt, unit);
				shape_set_position(&obj->bounds, attempt);
			}
		}
	}
	if(collision_func != NULL) {
		qt_collisions(world.entities, collision_func);
	}
}
