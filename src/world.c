#include <math.h>
#include <stdio.h>
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

Group *world_add_group(World *world, Group group) {
	return qt_add_group(&world->entities, group);
}

TileMap *world_get_tilemap(World world, size_t index) {
	return al_get(world.layers, index);
}

ArcadeObject world_remove(World *world, size_t index) {
	return qt_remove(&world->entities, index);
}

bool world_point_free(World world, Vector2 point, ArcadeObject *query_as) {
	for(size_t i = 0; i < world.layers.length; i++) {
		TileMap *map = al_get(world.layers, i);
		if(!tl_free(*map, (int)point.x, (int)point.y))
			return false;
	}
	ArcadeObject *query = qt_point_query(world.entities, point, query_as->group);
	return query == NULL || query == query_as;
}

bool world_region_free(World world, Shape region, ArcadeObject *query_as) {
	Rect bounds = shape_bounding_box(region);
	for(size_t i = 0; i < world.layers.length; i++) {
		TileMap *map = al_get(world.layers, i);
		if(!tl_empty(*map, (int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height))
			return false;
	}
	ArcadeObject *query = qt_region_query(world.entities, region, query_as->group);
	return query == NULL || query == query_as;
}

static inline Vector2 try_move(World world, ArcadeObject *obj, Vector2 velocity) {
	Vector2 pos = shape_get_position(obj->bounds);
	Vector2 attempt = vec2_add(pos, velocity);
	shape_set_position(&obj->bounds, attempt);
	if(world_region_free(world, obj->bounds, obj)) {
		return velocity;
	}
	Vector2 unit = vec2_nor(attempt);
	while(!world_region_free(world, obj->bounds, obj) && vec2_len2(attempt) > 1) {
		velocity = vec2_scl(velocity, 0.5);
		attempt = vec2_add(pos, velocity);
		shape_set_position(&obj->bounds, attempt);
	}
	if(!world_region_free(world, obj->bounds, obj)) {
		shape_set_position(&obj->bounds, pos);
		return vec2_new(0, 0);
	} else {
		return velocity;
	}
}

static inline float clamp(float value, float max) {
	if(value > max) return max;
	else if(value < -max) return -max;
	else return value;
}

static inline float oppose(float value, float factor) {
	if(fabs(value) < factor) 
		return 0;
	if(value > 0)
		return value - factor;
	else
		return value + factor;
}

void world_update(World world, float milliseconds, void (*update)(World,ArcadeObject*), void (*collision_func)(ArcadeObject*, ArcadeObject*)) {
	size_t length = qt_len(world.entities);
	if(update != NULL) {
		for(size_t i = 0; i < length; i++) {
			ArcadeObject *obj = qt_get(world.entities, i);
			if(!obj->alive) continue;
			//Apply the custom update
			update(world, qt_get(world.entities, i));
			//If this object is already entangled with another, skip the physics
			if(!world_region_free(world, obj->bounds, obj)) continue;
			//Accelerate the object
			Vector2 acceleration = vec2_scl(obj->acceleration, milliseconds);
			obj->velocity = vec2_add(acceleration, obj->velocity);
			//Apply velocity maximums
			if(obj->max_velocity.x >= 0)
				obj->velocity.x = clamp(obj->velocity.x, obj->max_velocity.x);
			if(obj->max_velocity.y >= 0)
				obj->velocity.y = clamp(obj->velocity.y, obj->max_velocity.y);
			//Apply drag to the velocity
			obj->velocity.x = oppose(obj->velocity.x, obj->drag.x);
			obj->velocity.y = oppose(obj->velocity.y, obj->drag.y);
			//Determine the velocity of the object
			Vector2 velocity = vec2_scl(obj->velocity, milliseconds);
			//Move the object to a free space
			Vector2 x = vec2_new(velocity.x, 0);
			Vector2 y = vec2_new(0, velocity.y);
			velocity.x = try_move(world, obj, x).x;
			velocity.y = try_move(world, obj, y).y;
			obj->velocity = vec2_scl(velocity, 1 / milliseconds);
		}
	}
	if(collision_func != NULL) {
		qt_collisions(world.entities, collision_func);
	}
}
