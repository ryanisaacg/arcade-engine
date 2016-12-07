#pragma once

#include "lines.h"
#include "quadtree.h"
#include "tilemap.h"
#include <stdbool.h>

typedef struct {
	QuadTree entities;
	ArrayList items;
	ArrayList layers;
} World;

World world_new(float width, float height, float qt_buckets_size, size_t data_size);
size_t world_add(World *world, ArcadeObject object, void *objects);
size_t world_add_tilemap(World *world, TileMap map);
Group *world_add_group(World *world, Group group);
bool world_point_free(World world, Vector2 point, ArcadeObject *query_as);
bool world_region_free(World world, Shape region, ArcadeObject *query_as);
ArcadeObject *world_get(World world, size_t index);
void *world_get_data(World world, size_t index);
TileMap *world_get_tilemap(World world, size_t index);
ArcadeObject world_remove(World *world, size_t index);
void world_update(World world, float milliseconds, void (*update)(World,ArcadeObject*), void (*collision_func)(ArcadeObject*, ArcadeObject*));
void world_destroy(World world);
