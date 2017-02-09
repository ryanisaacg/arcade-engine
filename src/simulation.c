#include "geom.h"
#include "multimedia.h"
#include "simulation.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <tmx.h>

//The side of a quad tree bucket square at the smallest level
#define QT_BUCKETS_SIZE 64

ArcadeObject arcobj_new(Shape bounds, bool solid, Sprite spr) {
	return (ArcadeObject) { 
		.sprite = spr,
		.bounds = bounds, 
		.velocity = vec2_new(0, 0), 
		.acceleration = vec2_new(0, 0), 
		.max_velocity = vec2_new(-1, -1), 
		.drag = vec2_new(0, 0), 
		.solid = solid, 
		.alive = true, 
		.bounce = false, 
		.group = NULL,
	};
}

bool arcobj_interacts(ArcadeObject *a, ArcadeObject *b) {
	return group_interacts(a->group, b->group);
}

bool keep_going;
Game game;

Game game_new(WindowConfig config, char **level_names, size_t *indices, size_t num_levels, size_t data_size) {
	ArrayList levels = al_new_sized(sizeof(Level), num_levels);
	Window window = window_new(config);
	AssetManager assets = asset_new(window);
	for(size_t i = 0; i < num_levels; i++) {
		Level lvl = level_load(level_names[i], assets, data_size);
		al_add(&levels, &lvl);
	}
	Window *win_alloced = malloc(sizeof(Window));
	*win_alloced = window;
	Level *first = al_get(levels, 0);
	World current = level_get_world(*first);
	current.window = win_alloced;
	return (Game) {
		.assets = assets,
		.current = current,
		.window = win_alloced,
		.levels = levels,
		.current_level_index = 0
	};
}

void game_start(Game g, WorldUpdate update, WorldCollide collide) {
	game = g;
	keep_going = true;
	while(window_should_contine(*(g.window)) && keep_going) {
		window_events(g.window);
		world_update(g.current, update, collide);
		window_start_draw(g.window, 0, 0, 0);
		world_draw(g.current);
		window_end_draw(*(g.window));
	}
}
void game_stop() {
	keep_going = false;
}
void game_set_level(size_t index) {
	game.current_level_index = index;
	Level *level = al_get(game.levels, index);
	game.current = level_get_world(*level);
}
void game_next_level() {
	game_set_level(game.current_level_index + 1);
}
void game_prev_level() {
	game_set_level(game.current_level_index - 1);
}

Group group_new() {
	static uint64_t current = 0;
	if(current == 0) {
		current = 1;
	} else {
		current *= 2;
	}
	return (Group) { current, 0 };
}

void group_blacklist(Group *group, Group *other) {
	group->blacklist |= other->id;
}

void group_blacklist_self(Group *group) {
	group_blacklist(group, group);
}

void group_whitelist(Group *group, Group *other) {
	group->blacklist &= !other->id;
}

void group_whitelist_self(Group *group) {
	group_whitelist(group, group);
}

bool group_interacts(Group *a, Group *b) {
	return !(a->blacklist & b->id);
}

static void load_layer(AssetManager assets, World *world, tmx_map *map, tmx_layer *current);
static void load_object_layer(AssetManager assets, World *world, tmx_map *map, tmx_layer *current);
static void load_image_layer(AssetManager assets, World *world, tmx_map *map, tmx_layer *current);

Level level_load(char *filename, AssetManager assets, size_t data_size) {
	tmx_map *map = tmx_load(filename);
	if(map == NULL) {
		tmx_perror("Loading the TileMap");
		exit(1);
	}
	World world = world_new(NULL, map->width, map->height, 64, data_size);
	//Retrieve the colors packed into the map->backgroundcolor
	world.r = map->backgroundcolor & 0xff;
	world.g = (map->backgroundcolor >> 8) & 0xff;
	world.b = (map->backgroundcolor >> 16) & 0xff;
	//Iterate over the linked list of map layers
	tmx_layer *current_layer = map->ly_head;
	while(current_layer != NULL) {
		switch(current_layer->type) {
		case L_NONE:
			break;
		case L_LAYER:
			load_layer(assets, &world, map, current_layer);
			break;
		case L_OBJGR:
			load_object_layer(assets, &world, map, current_layer);	
			break;
		case L_IMAGE:
			load_image_layer(assets, &world, map, current_layer);
			break;
		}
	}
}

static void load_layer(AssetManager assets, World *world, tmx_map *map, tmx_layer *current) {

}

static void load_object_layer(AssetManager assets, World *world, tmx_map *map, tmx_layer *current) {
	tmx_object *obj = current->content.objgr->head;
	while(obj != NULL) {
		switch(obj->shape) {
		case S_TILE:
			break;
		case S_NONE:
		case S_SQUARE:
		case S_POLYLINE:
		case S_POLYGON:
		case S_ELLIPSE:
			fprintf(stderr, "Attempted to load a geometric object from a TMX file. These are not supported.");
			break;
		}
		Sprite sprite;
		Shape shape = shape_rect(rect_new(obj->x + current->offsetx, obj->y + current->offsety, obj->width, obj->height));
		tmx_tile *tile = tmx_get_tile(map, obj->gid);
		if(tile->animation) {
			TextureRegion *frames = malloc(sizeof(TextureRegion) * tile->animation_len);
			for(size_t i = 0; i < tile->animation_len; i++) {
				Texture tex = asset_load_texture(assets, tmx_get_tile(map, tile->animation[i].tile_id)->image->source);
				frames[i] = texregion_new_sized(tex, rect_new(tile->ul_x, tile->ul_y, tile->image->width, tile->image->height));
			}
			Animation anim = anim_new(frames, tile->animation_len, tile->animation[0].duration);
			sprite = spr_new_animated(anim, shape_get_position(shape));
		} else {
			Texture tex = asset_load_texture(assets, tile->image->source);
			TextureRegion region = texregion_new_sized(tex, rect_new(tile->ul_x, tile->ul_y, tile->image->width, tile->image->height));
			sprite = spr_new_static(region, shape_get_position(shape));
		}
		if(obj->rotation != 0) {
			shape_set_rotation(&shape, obj->rotation);
			sprite.angle = obj->rotation;
		}
		//TODO: Loading of custom properties with function pointer
		world_add(world, arcobj_new(shape, false, sprite), NULL); //TODO: load if the object shoud be solid from the TMX file 
		obj = obj->next;
	}
}

static void load_image_layer(AssetManager assets, World *world, tmx_map *map, tmx_layer *current) {
	tmx_image *image = current->content.image;
	Texture tex = asset_load_texture(assets, image->source);
	TextureRegion region = texregion_new(tex);
	Sprite sprite = spr_new_static(region, vec2_new(0, 0));
	sprite.scale.x = image->width / tex.width;
	sprite.scale.y = image->height / tex.height;
	world_add_sprite(world, sprite);
}

World level_get_world(Level level) {
	//TODO: Deep copy if the level is not persistent
	return level.data;
}

void level_destroy(Level level) {
	world_destroy(level.data);
}

static void qt_collide(QuadTree *subtree, World world, size_t current, WorldCollide collide);

QuadTree *qt_new(QuadTree *parent, Rect region, float min_width, float min_height) {
	if(region.width < min_width || region.height < min_height)
		return NULL;
	ArrayList *entities, *groups;
	if(parent == NULL) {
		entities = malloc(sizeof(ArrayList));
		groups = malloc(sizeof(ArrayList));
		*entities = al_new(sizeof(ArcadeObject));
		*groups = al_new(sizeof(Group));
	} else {
		entities = parent->entities;
		groups = parent->groups;
	}
	QuadTree *node = malloc(sizeof(QuadTree));
	*node = (QuadTree) {
		.region = region,
		.contains = al_new(sizeof(size_t)),
		.entities = entities,
		.groups = groups,
		.root = parent == NULL
	};
	float child_width = region.width / 2;
	float child_height = region.height / 2;
	int i = 0;
	for(int x = 0; x <= 1; x++) {
		for(int y = 0; y <= 1; y++) {
			Rect child = rect_new(region.x + x * child_width, region.y + y * child_height, child_width, child_height);
			node->children[i++] = qt_new(node, child, min_width, min_height);
		}
	}
	return node;
}

QuadTree *qt_get_child(QuadTree *subtree, Rect bounds) {
	if(subtree == NULL) {
		return NULL;
	}
	for(int i = 0; i < 4; i++) {
		QuadTree *child = subtree->children[i];
		if(child != NULL && engulfs_rect(child->region, bounds))
			return qt_get_child(child, bounds);
	}
	return subtree;
}

size_t qt_add(QuadTree *tree, ArcadeObject obj) {
	QuadTree *node = qt_get_child(tree, shape_bounding_box(obj.bounds));
	al_add(tree->entities, &obj);
	size_t index = tree->entities->length - 1;
	al_add(&node->contains, &index);
	return index;
}

Group *qt_add_group(QuadTree *tree, Group group) {
	al_add(tree->groups, &group);
	return al_get(*tree->groups, tree->groups->length - 1);
}

ArcadeObject qt_remove(QuadTree *tree, size_t index) {
	ArcadeObject *obj = al_get(*tree->entities, index);
	ArcadeObject value = *obj;
	QuadTree *node = qt_get_child(tree, shape_bounding_box(obj->bounds));
	al_remove_item(&node->contains, &index);
	al_remove_index(tree->entities, index);
	return value;
}

void qt_clear(QuadTree *tree) {
	al_clear(tree->entities);
	al_clear(&tree->contains);
	for(int i = 0; i < 4; i++)
		if(tree->children[i] != NULL)
			qt_clear(tree->children[i]);
}

size_t qt_len(QuadTree *tree) {
	return tree->entities->length;
}

ArcadeObject *qt_get(QuadTree *tree, size_t index) {
	return al_get(*tree->entities, index);
}

ArcadeObject *qt_point_query(QuadTree *tree, Vector2 point, Group *query_as) {
	for(size_t i = 0; i < tree->contains.length; i++) {
		size_t *index = al_get(tree->contains, i);
		ArcadeObject *obj = al_get(*tree->entities, *index);
		if(obj->alive && (obj->group == NULL || group_interacts(obj->group, query_as)) && shape_contains(obj->bounds, point)) {
			return obj;
		}
	}
	for(size_t i = 0; i < 4; i++) {
		QuadTree *child = tree->children[i];
		if(child != NULL && rect_contains(child->region, point)) {
			ArcadeObject *obj = qt_point_query(child, point, query_as);
			if(obj != NULL) {
				return obj;
			}
		}
	}
}

ArcadeObject *qt_region_query(QuadTree *tree, Shape region, Group *query_as) {
	for(size_t i = 0; i < tree->contains.length; i++) {
		size_t *index = al_get(tree->contains, i);
		ArcadeObject *obj = al_get(*tree->entities, *index);
		if(obj->alive && (obj->group == NULL || group_interacts(obj->group, query_as)) && overlaps_shape(obj->bounds, region)) {
			return obj;
		}
	}
	for(size_t i = 0; i < 4; i++) {
		QuadTree *child = tree->children[i];
		if(child != NULL && overlaps_shape(shape_rect(child->region), region)) {
			ArcadeObject *obj = qt_region_query(child, region, query_as);
			if(obj != NULL) {
				return obj;
			}
		}
	}
	return NULL;
}

bool qt_point_free(QuadTree *tree, Vector2 point, ArcadeObject *ignore) {
	for(size_t i = 0; i < tree->contains.length; i++) {
		size_t *index = al_get(tree->contains, i);
		ArcadeObject *obj = al_get(*tree->entities, *index);
		if(obj != ignore && obj->alive && obj->solid && shape_contains(obj->bounds, point)) {
			return false;
		}
	}
	for(size_t i = 0; i < 4; i++) {
		QuadTree *child = tree->children[i];
		if(child != NULL && rect_contains(child->region, point)) {
			bool free = qt_point_free(child, point, ignore);
			if(!free) {
				return false;
			}
		}
	}
	return true;
}

bool qt_region_free(QuadTree *tree, Shape region, ArcadeObject *ignore) {
	for(size_t i = 0; i < tree->contains.length; i++) {
		size_t *index = al_get(tree->contains, i);
		ArcadeObject *obj = al_get(*tree->entities, *index);
		if(obj != ignore && obj->alive && obj->solid && overlaps_shape(obj->bounds, region)) {
			return false;
		}
	}
	for(size_t i = 0; i < 4; i++) {
		QuadTree *child = tree->children[i];
		if(child != NULL && overlaps_shape(shape_rect(child->region), region)) {
			bool free = qt_region_free(child, region, ignore);
			if(!free) {
				return false;
			}
		}
	}
	return true;
}

void qt_collisions(QuadTree *tree, World world, WorldCollide collide) {
	for(size_t i = 0; i < tree->contains.length; i++) {
		size_t *value = al_get(tree->contains, i);
		ArcadeObject *obj = qt_get(tree, *value);
		if(obj->alive) {
			qt_collide(tree, world, *value, collide);
		}
	}
	for(size_t i = 0; i < 4; i++) {
		if(tree->children[i] != NULL) {
			qt_collisions(tree->children[i], world, collide);
		}
	}
}

static void qt_collide(QuadTree *subtree, World world, size_t current, WorldCollide collide) {
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
			qt_collide(subtree->children[i], world, current, collide);
		}
	}
}

void qt_destroy(QuadTree *tree) {
	if(tree->root) {
		al_destroy(*tree->entities);
		al_destroy(*tree->groups);
		free(tree->entities);
		free(tree->groups);
	}
	for(size_t i = 0; i < 4; i++) {
		if(tree->children[i] != NULL) 
			qt_destroy(tree->children[i]);
	}
	free(tree);
}

static size_t get_index(SpatialMap *map, float x, float y) {
	int gridHeight = (int)(map->height / map->tile_height);
	int posX = (int)(x / map->tile_width);
	int posY = (int)(y / map->tile_height);
	return posX * gridHeight + posY;
}

SpatialMap sm_new(size_t item_size, float width, float height, 
									float tile_width, float tile_height, bool drawable) {
	SpatialMap map;
	map.width 		= width;
	map.height		= height;
	map.tile_width 	= tile_width;
	map.tile_height = tile_height;
	size_t last 	= get_index(&map, width, height);
	map.items 		= al_new_sized(item_size, last);
	map.has 		= al_new_sized(item_size, last);
	map.drawable	= drawable;
	bool tmp = false;
	for(size_t i = 0; i < last; i++) {
		al_set(&map.has, i, &tmp);
	}
	return map;
}
void sm_set(SpatialMap *map, void *item, float x, float y) {
	size_t index = get_index(map, x, y);
	al_set(&map->items, index, item);
	bool tmp = true;
	al_set(&map->has, index, &tmp);
}
void sm_remove(SpatialMap *map, float x, float y) {
	size_t index = get_index(map, x, y);
	bool tmp = false;
	al_set(&map->has, index, &tmp);
}
bool sm_contains(SpatialMap map, float x, float y) {
	return x >= 0 && y >= 0 && x < map.width && y < map.height; 
}
void *sm_get(SpatialMap map, float x, float y) {
	if(sm_contains(map, x, y)) {
		size_t index = get_index(&map, x, y);
		bool *has = al_get(map.has, index);
		return (*has) ? al_get(map.items, index) : NULL;
	} else {
		return NULL;
	}
}
bool sm_has(SpatialMap map, float x, float y) {
	if(sm_contains(map, x, y)) {
		size_t index = get_index(&map, x, y);
		bool *has = al_get(map.has, index);
		return *has;
	} else {
		return true;
	}
}
bool sm_free(SpatialMap map, Shape shape) {
	Rect region = shape_bounding_box(shape);
	if(!sm_contains(map, region.x, region.y) || !sm_contains(map, region.x + region.width, region.y + region.height))
		return false;
	for(float x = region.x; x < region.x + region.width; x += map.tile_width) {
		for(float y = region.y; y < region.y + region.height; y += map.tile_height) {
			if(sm_has(map, x, y)) {
				return false;
			}
		}
	}
	return true;
}

void sm_destroy(SpatialMap map) {
	al_destroy(map.items);
	al_destroy(map.has);
}

World world_new(Window *window, float width, float height, float qt_buckets_size, size_t data_size) {
	return (World) {
		.window = window,
		.entities = qt_new(NULL, rect_new(0, 0, width, height), qt_buckets_size, qt_buckets_size),
		.items = al_new(data_size),
		.layers = al_new(sizeof(SpatialMap)),
		.camera = cam_new(window, rect_new(0, 0, width, height), true),
		.r = 0,
		.g = 0,
		.b = 0
	};
}

size_t world_add(World *world, ArcadeObject object, void *data_object) {
	al_add(&world->items, data_object);
	size_t index = qt_add(world->entities, object);
	world_get(*world, index)->index = index;
	return index;
}

ArcadeObject *world_get(World world, size_t index) {
	return qt_get(world.entities, index);
}

void *world_get_data(World world, size_t index) {
	return al_get(world.items, index);
}

size_t world_add_map(World *world, SpatialMap map) {
	size_t index = world->layers.length;
	al_add(&world->layers, &map);
	return index;
}

Group *world_add_group(World *world, Group group) {
	return qt_add_group(world->entities, group);
}

void world_add_sprite(World *world, Sprite sprite) {
	al_add(&world->sprites, &sprite);
}

SpatialMap *world_get_map(World world, size_t index) {
	return al_get(world.layers, index);
}

ArcadeObject world_remove(World *world, size_t index) {
	return qt_remove(world->entities, index);
}

bool world_point_free(World world, Vector2 point, ArcadeObject *ignore) {
	for(size_t i = 0; i < world.layers.length; i++) {
		SpatialMap *map = al_get(world.layers, i);
		if(sm_has(*map, point.x, point.y))
			return false;
	}
	return qt_point_free(world.entities, point, ignore);
}

bool world_region_free(World world, Shape region, ArcadeObject *ignore) {
	for(size_t i = 0; i < world.layers.length; i++) {
		SpatialMap *map = al_get(world.layers, i);
		if(!sm_free(*map, region)) {
			return false;
		}
	}
	return qt_region_free(world.entities, region, ignore);
}

static inline Vector2 try_move(World world, ArcadeObject *obj, Vector2 velocity) {
	Vector2 pos = shape_get_position(obj->bounds);
	Vector2 attempt = vec2_add(pos, velocity);
	shape_set_position(&obj->bounds, attempt);
	if(world_region_free(world, obj->bounds, obj)) {
		return velocity;
	}
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

static inline void move_entity(World world, ArcadeObject *obj, void *data) {
	if(!world_region_free(world, obj->bounds, obj)) return; //If this object is already entangled with another, skip the physics
	//Accelerate the object
	obj->velocity = vec2_add(obj->acceleration, obj->velocity);
	//Apply velocity maximums
	if(obj->max_velocity.x >= 0)
		obj->velocity.x = clamp(obj->velocity.x, obj->max_velocity.x);
	if(obj->max_velocity.y >= 0)
		obj->velocity.y = clamp(obj->velocity.y, obj->max_velocity.y);
	//Apply drag to the velocity
	obj->velocity.x = oppose(obj->velocity.x, obj->drag.x);
	obj->velocity.y = oppose(obj->velocity.y, obj->drag.y);
	Vector2 velocity = obj->velocity;
	//Move the object to a free space
	Vector2 x = vec2_new(velocity.x, 0);
	Vector2 y = vec2_new(0, velocity.y);
	if(obj->bounce) {
		Vector2 oldX = x;
		Vector2 oldY = y;
		x = try_move(world, obj, x);
		y = try_move(world, obj, y);
		if(oldX.x != x.x) {
			velocity.x *= -1;
		}
		if(oldY.y != y.y) {
			velocity.y *= -1;
		}
	} else {
		velocity.x = try_move(world, obj, x).x;
		velocity.y = try_move(world, obj, y).y;
	}
	obj->velocity = velocity;
}

void world_update(World world, WorldUpdate update, WorldCollide collide) {
	world_foreach(world, move_entity);
	if(update != NULL) {
		world_foreach(world, update);
	}
	if(collide != NULL) {
		qt_collisions(world.entities, world, collide);
	}
}

void world_foreach(World world, WorldUpdate update) {
	size_t length = qt_len(world.entities);
	for(size_t i = 0; i < length; i++) {
		ArcadeObject *obj = qt_get(world.entities, i);
		if(!obj->alive) continue;
		update(world, obj, al_get(world.items, i));
	}
}

static void entity_draw(World world, ArcadeObject *obj, void *data) {
	window_draw(*(world.window), &(world.camera), obj->sprite);
}

void world_draw(World world) {
	for(size_t i = 0; i < world.sprites.length; i++) {
		Sprite *spr = al_get(world.sprites, i);
		window_draw(*(world.window), &(world.camera), *spr);
	}
	for(size_t i = 0; i < world.layers.length; i++) {
		SpatialMap *map = al_get(world.layers, i);
		if(!map->drawable) continue;
		SpatialMap m = *map;
		for(float x = 0; x < map->width; x += map->tile_width) {
			for(float y = 0; y < map->height; y += map->tile_height) {
				if(sm_has(m, x, y)) {
					TextureRegion *region = sm_get(m, x, y);
					Sprite spr = spr_new_static(*region, vec2_new(x, y));
					window_draw(*(world.window), &(world.camera), spr);
				}
			}
		}
	}
	world_foreach(world, entity_draw);
}

void world_destroy(World world) {
	qt_destroy(world.entities);
	for(size_t i = 0; i < world.layers.length; i++) {
		SpatialMap *map = al_get(world.layers, i);
		sm_destroy(*map);
	}
	al_destroy(world.layers);
}

Spawner spawn_new(Document doc, char *name, SpawnerFunction func) {

}

ArcadeObject spawn_get_obj(Spawner spawn) {
	return spawn.obj;
}

void *spawn_get_data(Spawner spawn) {
	return spawn.data;
}
