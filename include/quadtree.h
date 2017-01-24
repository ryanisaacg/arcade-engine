#pragma once

#include <stdbool.h>

#include "array_list.h"

#include "arcobj.h"
#include "lines.h"
#include "rect.h"
#include "shape.h"

struct World;

struct QuadNode;
typedef struct QuadNode QuadNode;

struct QuadTree {
	QuadNode *root;
	ArrayList entities, groups;
};

struct QuadNode {
	QuadNode *children[4];
	Rect region;
	ArrayList contains;
};

typedef void (*WorldCollide)(struct World, ArcadeObject*, void*, ArcadeObject*, void*);

QuadTree qt_new(float width, float height, float min_width, float min_height);
size_t qt_add(QuadTree *tree, ArcadeObject obj);
Group *qt_add_group(QuadTree *tree, Group group);
ArcadeObject *qt_get(QuadTree tree, size_t index);
ArcadeObject *qt_point_query(QuadTree tree, Vector2 point, Group *query_as);
ArcadeObject *qt_region_query(QuadTree tree, Shape region, Group *query_as);
ArcadeObject qt_remove(QuadTree *tree, size_t index);
void qt_clear(QuadTree *tree);
size_t qt_len(QuadTree tree);
ArcadeObject *qt_get(QuadTree tree, size_t index);
void qt_collisions(QuadTree tree, struct World world, WorldCollide collide);
void qt_destroy(QuadTree tree);
