#pragma once

#include "arcobj.h"
#include "array_list.h"
#include "lines.h"
#include "rect.h"
#include "shape.h"
#include <stdbool.h>

struct QuadNode;
typedef struct QuadNode QuadNode;

typedef struct {
	QuadNode *root;
	ArrayList entities, groups;
} QuadTree;

struct QuadNode {
	QuadNode *children[4];
	Rect region;
	ArrayList contains;
};

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
void qt_collisions(QuadTree tree, ArrayList items, void (*collide)(ArcadeObject*, void*, ArcadeObject*, void*));
void qt_destroy(QuadTree tree);
