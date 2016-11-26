#pragma once

#include "quadtree.h"
#include "tilemap.h"

typedef struct {
	QuadTree entities;
	ArrayList layers;
} World;

World world_new(float width, float height, float qt_buckets_size);
size_t world_add(World *world, ArcadeObject object);
size_t world_add_tilemap(World *world, TileMap map);
ArcadeObject *world_get(World world, size_t index);
TileMap *world_get_tilemap(World world, size_t index);
ArcadeObject world_remove(World *world, size_t index);
void world_update(World world, float milliseconds, void (*update)(ArcadeObject*), void (*collision_func)(ArcadeObject*, ArcadeObject*));
