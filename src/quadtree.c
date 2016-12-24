#include "quadtree.h"

#include <stdio.h>

#include "collision.h"
#include "world.h"

static QuadNode *get_node(QuadNode *subtree, Rect bounds);
static QuadNode *node_new(Rect region, float min_width, float min_height);
static ArcadeObject *node_point_query(QuadNode *subtree, ArrayList objects, Vector2 point, Group *query_as);
static ArcadeObject *node_region_query(QuadNode *subtree, ArrayList objects, Shape region, Group *query_as);
static void node_clear(QuadNode *subtree);
static void node_all_collisions(QuadNode *subtree, World world, WorldCollide collide);
static void node_collide(QuadNode *subtree, World world, size_t current, WorldCollide collide);
static void node_destroy(QuadNode *subtree);

QuadTree qt_new(float width, float height, float min_width, float min_height) {
	QuadTree tree;
	tree.root = node_new(rect_new(0.f, 0.f, width, height), min_width, min_height);
	tree.entities = al_new(sizeof(ArcadeObject));
	tree.groups = al_new(sizeof(Group));
	return tree;
}

size_t qt_add(QuadTree *tree, ArcadeObject obj) {
	QuadNode *node = get_node(tree->root, shape_bounding_box(obj.bounds));
	al_add(&tree->entities, &obj);
	size_t index = tree->entities.length - 1;
	al_add(&node->contains, &index);
	return index;
}

Group *qt_add_group(QuadTree *tree, Group group) {
	al_add(&tree->groups, &group);
	return al_get(tree->groups, tree->groups.length - 1);
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

ArcadeObject *qt_point_query(QuadTree tree, Vector2 point, Group *query_as) {
	return node_point_query(tree.root, tree.entities, point, query_as);
}

ArcadeObject *qt_region_query(QuadTree tree, Shape region, Group *query_as) {
	return node_region_query(tree.root, tree.entities, region, query_as);
}

void qt_collisions(QuadTree tree, World world, WorldCollide collide) {
	node_all_collisions(tree.root, world, collide);
}

void qt_destroy(QuadTree tree) {
	al_destroy(tree.entities);
	al_destroy(tree.groups);
	node_destroy(tree.root);
}

static QuadNode *get_node(QuadNode *subtree, Rect bounds) {
	if(subtree == NULL) {
		return NULL;
	}
	for(int i = 0; i < 4; i++) {
		QuadNode *child = subtree->children[i];
		if(child != NULL && engulfs_rect(child->region, bounds))
			return get_node(child, bounds);
	}
	return subtree;
}
//PARENT MAY BE NULL, MAY RETURN NULL
static QuadNode *node_new(Rect region, float min_width, float min_height) {
	if(region.width < min_width || region.height < min_height)
		return NULL;
	QuadNode *node = malloc(sizeof(*node));
	node->region = region;
	node->contains = al_new(sizeof(size_t));
	float child_width = region.width / 2.f;
	float child_height = region.height / 2.f;
	int i = 0;
	for(int x = 0; x <= 1; x++) {
		for(int y = 0; y <= 1; y++) {
			Rect child = rect_new(region.x + x * child_width, region.y + y * child_height, child_width, child_height);
			node->children[i] = node_new(child, min_width, min_height);
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

static void node_all_collisions(QuadNode *subtree, World world, WorldCollide collide) {
	for(size_t i = 0; i < subtree->contains.length; i++) {
		size_t *value = al_get(subtree->contains, i);
		ArcadeObject *obj = world_get(world, *value);
		if(obj->alive) {
			node_collide(subtree, world, *value, collide);
		}
	}
	for(size_t i = 0; i < 4; i++) {
		if(subtree->children[i] != NULL) {
			node_all_collisions(subtree->children[i], world, collide);
		}
	}
}

static void node_collide(QuadNode *subtree, World world, size_t current, WorldCollide collide) {
	for(size_t i = 0; i < subtree->contains.length; i++) {
		size_t *value = al_get(subtree->contains, i);
		size_t other = *value;
		if(current != other) {
			ArcadeObject *a = world_get(world, current);
			ArcadeObject *b = world_get(world, other);
			if(a->alive && b->alive && overlaps_shape(a->bounds, b->bounds) 
					&& (a->group == NULL || b->group == NULL || group_interacts(a->group, b->group))) {
				void *a_data = world_get_data(world, current);
				void *b_data = world_get_data(world, other);
				collide(world, a, a_data, b, b_data);
			}
		}
	}
	for(size_t i = 0; i < 4; i++) {
		if(subtree->children[i] != NULL) {
			node_collide(subtree->children[i], world, current, collide);
		}
	}
}

static ArcadeObject *node_point_query(QuadNode *subtree, ArrayList objects, Vector2 point, Group *query_as) {
	for(size_t i = 0; i < subtree->contains.length; i++) {
		size_t *index = al_get(subtree->contains, i);
		ArcadeObject *obj = al_get(objects, *index);
		if(obj->alive && (obj->group == NULL || group_interacts(obj->group, query_as)) && obj->solid && shape_contains(obj->bounds, point)) {
			return obj;
		}
	}
	for(size_t i = 0; i < 4; i++) {
		QuadNode *child = subtree->children[i];
		if(child != NULL && rect_contains(child->region, point)) {
			ArcadeObject *obj = node_point_query(child, objects, point, query_as);
			if(obj != NULL) {
				return obj;
			}
		}
	}
	return NULL;
}
static ArcadeObject *node_region_query(QuadNode *subtree, ArrayList objects, Shape region, Group *query_as) {
	for(size_t i = 0; i < subtree->contains.length; i++) {
		size_t *index = al_get(subtree->contains, i);
		ArcadeObject *obj = al_get(objects, *index);
		if(obj->alive && (obj->group == NULL || group_interacts(obj->group, query_as)) && obj->solid && overlaps_shape(obj->bounds, region)) {
			return obj;
		}
	}
	for(size_t i = 0; i < 4; i++) {
		QuadNode *child = subtree->children[i];
		if(child != NULL && overlaps_shape(shape_rect(child->region), region)) {
			ArcadeObject *obj = node_region_query(child, objects, region, query_as);
			if(obj != NULL) {
				return obj;
			}
		}
	}
	return NULL;
}

static void node_destroy(QuadNode *subtree) {
	for(size_t i = 0; i < 4; i++) {
		if(subtree->children[i] != NULL) 
			node_destroy(subtree->children[i]);
	}
	if(subtree != NULL) {
		free(subtree);
	}
}
