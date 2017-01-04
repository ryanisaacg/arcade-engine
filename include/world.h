#pragma once

#include <stdbool.h>

#include "batch.h"
#include "lines.h"
#include "quadtree.h"
#include "spatial_map.h"

typedef struct World {
	QuadTree entities;
	ArrayList items;
	ArrayList layers;
} World;

typedef void (*WorldUpdate)(World, ArcadeObject*, void*);
typedef void (*WorldDraw)(Batch, ArcadeObject*, void*);

World world_new(float width, float height, float qt_buckets_size, size_t data_size);
size_t world_add(World *world, ArcadeObject object, void *objects);
size_t world_add_map(World *world, SpatialMap map);
Group *world_add_group(World *world, Group group);
bool world_point_free(World world, Vector2 point, ArcadeObject *query_as);
bool world_region_free(World world, Shape region, ArcadeObject *query_as);
ArcadeObject *world_get(World world, size_t index);
void *world_get_data(World world, size_t index);
SpatialMap *world_get_map(World world, size_t index);
ArcadeObject world_remove(World *world, size_t index);
void world_update(World world, float milliseconds, WorldUpdate update, WorldCollide collide);
void world_draw(World world, Batch batch);
void world_draw_custom(World world, WorldDraw draw, Batch batch);
void world_destroy(World world);
