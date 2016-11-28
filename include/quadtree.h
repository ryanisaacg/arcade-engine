#pragma once

#include "array_list.h"
#include "rect.h"
#include "shape.h"

struct QuadNode;
typedef struct QuadNode QuadNode;

typedef struct {
	QuadNode *root;
	ArrayList entities;
} QuadTree;

struct QuadNode {
	QuadNode *children[4];
	Rect region;
	ArrayList contains;
};

typedef struct {
	Shape bounds;
	Vector2 velocity, acceleration;
	void *data;
} ArcadeObject;

QuadTree qt_new(float width, float height, float min_width, float min_height);
size_t qt_add(QuadTree *tree, ArcadeObject obj);
ArcadeObject *qt_get(QuadTree tree, size_t index);
ArcadeObject qt_remove(QuadTree *tree, size_t index);
void qt_clear(QuadTree *tree);
size_t qt_len(QuadTree tree);
ArcadeObject *qt_get(QuadTree tree, size_t index);
void qt_collisions(QuadTree tree, void (*collide)(ArcadeObject*, ArcadeObject*));
