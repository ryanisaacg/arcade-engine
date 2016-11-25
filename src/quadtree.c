#include "collision.h"
#include "quadtree.h"
#include <stdio.h>

static QuadNode *get_node(QuadNode *subtree, Rect bounds);
static QuadNode *node_new(QuadNode *parent, Rect region, float min_width, float min_height);
static void node_clear(QuadNode *subtree);
static void node_all_collisions(QuadNode *subtree, ArrayList objects, void (*collide)(ArcadeObject*, ArcadeObject*));
static void node_collide(QuadNode *subtree, ArrayList objects, size_t current, void (*collide)(ArcadeObject*, ArcadeObject*));

QuadTree qt_new(float width, float height, float min_width, float min_height) {
	QuadTree tree;
	tree.root = node_new(NULL, rect_new(0.f, 0.f, width, height), min_width, min_height);
	tree.entities = al_new(sizeof(ArcadeObject));
	return tree;
}

size_t qt_add(QuadTree *tree, ArcadeObject obj) {
	QuadNode *node = get_node(tree->root, shape_bounding_box(obj.bounds));
	al_add(&tree->entities, &obj);
	size_t index = tree->entities.length - 1;
	al_add(&node->contains, &index);
	return index;
}
ArcadeObject qt_remove(QuadTree *tree, size_t index) {
	ArcadeObject *obj = al_get(tree->entities, index);
	ArcadeObject value = *obj;
	QuadNode *node = get_node(tree->root, shape_bounding_box(obj->bounds));
	al_remove_item(&node->contains, &index);
	al_remove_index(&tree->entities, index);
	return value;
}

void qt_clear(QuadTree *tree) {
	al_clear(&tree->entities);
	node_clear(tree->root);
}

size_t qt_len(QuadTree tree) {
	return tree.entities.length;
}

ArcadeObject *qt_get(QuadTree tree, size_t index) {
	return al_get(tree.entities, index);
}

void qt_collisions(QuadTree tree, void (*collide)(ArcadeObject*, ArcadeObject*)) {
	node_all_collisions(tree.root, tree.entities, collide);
}

static QuadNode *get_node(QuadNode *subtree, Rect bounds) {
	if(subtree == NULL) {
		return NULL;
	}
	if(!engulfs_rect(subtree->region, bounds)) {
		return get_node(subtree->parent, bounds);
	}
	for(int i = 0; i < 4; i++) {
		QuadNode *child = subtree->children[i];
		if(child != NULL && engulfs_rect(child->region, bounds))
			return get_node(child, bounds);
	}
	return subtree;
}
//PARENT MAY BE NULL, MAY RETURN NULL
static QuadNode *node_new(QuadNode *parent, Rect region, float min_width, float min_height) {
	if(region.width < min_width || region.height < min_height)
		return NULL;
	QuadNode *node = malloc(sizeof(*node));
	node->parent = parent;
	node->region = region;
	node->contains = al_new(sizeof(size_t));
	float child_width = region.width / 2.f;
	float child_height = region.height / 2.f;
	Rect smaller = rect_new(region.x, region.y, child_width, child_height);
	int i = 0;
	for(int x = 0; x <= 1; x++) {
		for(int y = 0; y <= 1; y++) {
			Rect child = rect_new(region.x + x * child_width, region.y + y * child_height, child_width, child_height);
			node->children[i] = node_new(node, child, min_width, min_height);
			i++;
		}
	}
	return node;
}

static void node_clear(QuadNode *subtree) {
	al_clear(&subtree->contains);
	for(int i = 0; i < 4; i++)
		if(subtree->children[i] != NULL)
			node_clear(subtree->children[i]);
}

static void node_all_collisions(QuadNode *subtree, ArrayList objects, void (*collide)(ArcadeObject*, ArcadeObject*)) {
	for(size_t i = 0; i < subtree->contains.length; i++) {
		size_t *value = al_get(subtree->contains, i);
		node_collide(subtree, objects, *value, collide);
	}
	for(size_t i = 0; i < 4; i++) {
		if(subtree->children[i] != NULL) {
			node_all_collisions(subtree->children[i], objects, collide);
		}
	}
}

static void node_collide(QuadNode *subtree, ArrayList objects, size_t current, void (*collide)(ArcadeObject*, ArcadeObject*)) {
	for(size_t i = 0; i < subtree->contains.length; i++) {
		size_t *value = al_get(subtree->contains, i);
		size_t other = *value;
		if(current != other) {
			ArcadeObject *a = al_get(objects, current);
			ArcadeObject *b = al_get(objects, other);
			if(overlaps_shape(a->bounds, b->bounds)) {
				collide(a, b);
			}
		}
	}
	for(size_t i = 0; i < 4; i++) {
		if(subtree->children[i] != NULL) {
			node_collide(subtree->children[i], objects, current, collide);
		}
	}
}

