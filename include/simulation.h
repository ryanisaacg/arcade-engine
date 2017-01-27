#pragma once

#include "geom.h"
#include "graphics.h"

// *** SIMULATION ***
// A node in the quadtree
typedef struct QuadNode {
	struct QuadNode *children[4]; // The four children of the node that are the four quarters, NULL if this is a leaf
	Rect region; // The region that this node contains
	ArrayList contains; // The indices that this node has
} QuadNode;
//A structure that recursively divides game space into quarters to make collision efficient
typedef struct QuadTree {
	QuadNode *root; //The root node of the quadtree
	ArrayList entities; //All of the entities stored in the quadtree (list of ArcadeObject)
	ArrayList groups; //The different groups stored in the quadtree (list of Group)
} QuadTree;
//A structure that divides game space into tiles to allow static objects
typedef struct SpatialMap {
	float width, height, tile_width, tile_height; //physical properties
	ArrayList items; //the buffer for the items in the map (list of user-defined items)
	ArrayList has; //buffer of flags that determine if an item is present at that index (bool)
} SpatialMap;
//A struct that unifies various game properties and objects for easy manipulation
typedef struct World {
	Window *window; //pointer to the window for drawing
	Camera camera; //the camera instance
	QuadTree entities; //the game entities
	ArrayList items; //a list of user-defined data
	ArrayList layers; //a list of the maps (list of SpatialMap)
	int r, g, b; //background color, black by default
} World;
//A group of entities
typedef struct Group {
	uint64_t id, blacklist;
} Group;
//A game object
typedef struct ArcadeObject {
	Sprite sprite; //The current game sprite
	Shape bounds; //The physical, in-world hitbox
	Vector2 velocity, acceleration, max_velocity, drag; //Physical properties
	bool solid; //If other objects are stopped on collision (TODO: implement)
	bool alive; //If the object should be considered in updateds
	bool bounce; //Should the object bounce off obstacles elastically
	Group *group; //The group that determines interactions between objects
	size_t index; //The index of the object in the world (set automatically)
} ArcadeObject;
//A structure that holds data for a level
typedef struct Level {
	World data; //the interpreted world
	bool persistent; //if the level should be the same if it is returned to
} Level;
//A structure that unifies all game items
typedef struct Game {
	AssetManager assets; //The game assets
	World current; //the current world
	Window *window; //the window (heap-allocated)
	ArrayList levels; //the levels in the game (list of Game)
} Game;

typedef void (*WorldUpdate)(World, ArcadeObject*, void*);
typedef void (*WorldCollide)(World, ArcadeObject*, void*, ArcadeObject*, void*);

// *** QUADTREES ***
// Create a new quadtree where the leaf nodes will have at least min_width and min_height
QuadTree qt_new(float width, float height, float min_width, float min_height);
// Add an arcade object to the quadtree and return its index
size_t qt_add(QuadTree *tree, ArcadeObject obj);
// Register a group to the quadtree and return a pointer to its heap-allocated location
Group *qt_add_group(QuadTree *tree, Group group);
// Get a pointer to the given arcade object
ArcadeObject *qt_get(QuadTree tree, size_t index);
// Check for an arcade object at the given location. No particular order in returning
// NULL indicates no object was found
ArcadeObject *qt_point_query(QuadTree tree, Vector2 point, Group *query_as);
// Check for an arcade object at the given region. No particular order in returning
// NULL indicates no object was found
ArcadeObject *qt_region_query(QuadTree tree, Shape region, Group *query_as);
// Remove an object from the quadtree and return it
ArcadeObject qt_remove(QuadTree *tree, size_t index);
// Clear all data from the quadtree but retain allocated memory
void qt_clear(QuadTree *tree);
// Return the number of objects inserted into the quadtree
size_t qt_len(QuadTree tree);
// Run the collision checks and do whatever action collide performs
// Objects will not interact with objects that have blacklisted groups
void qt_collisions(QuadTree tree, World world, WorldCollide collide);
// Destroy the quadtree and deallocate its memory
void qt_destroy(QuadTree tree);

// *** SPATIAL MAPS ***
// Create a new spatial map of a user-specified type
SpatialMap sm_new(size_t item_size, float width, float height, float tile_width, float tile_height);
// Set the data at a given position
void sm_set(SpatialMap *map, void *item, float x, float y);
// Remove the data at a given position
void sm_remove(SpatialMap *map, float x, float y);
// Check to see if the position is within the bounds of the map
bool sm_contains(SpatialMap map, float x, float y);
// Get the data at a given position (NULL indicates no data)
void *sm_get(SpatialMap map, float x, float y);
// Check if the spatial map has data a given position
bool sm_has(SpatialMap map, float x, float y);
// Check to see if the space bounded by the shape is entirely empty
bool sm_free(SpatialMap map, Shape region);
// Deallocates the spatial map
void sm_destroy(SpatialMap map);

// *** SIMULATION WORLDS ***
// Create a new world with the given properties. qt_bucket_size is the size of the quadtree leaves. data_size is the sizeof the user-defined data struct
World world_new(Window *window, float width, float height, float qt_buckets_size, size_t data_size);
// Add a game object and associated user-defined data and get its index
size_t world_add(World *world, ArcadeObject object, void *data);
// Add a spatial map to the world and get its index
size_t world_add_map(World *world, SpatialMap map);
// Add a group to the world and get its heap allocated location
Group *world_add_group(World *world, Group group);
// Check if a point is empty within the world
bool world_point_free(World world, Vector2 point, ArcadeObject *query_as);
// Check if a region is free within the world
bool world_region_free(World world, Shape region, ArcadeObject *query_as);
// Get the object of the specified index
ArcadeObject *world_get(World world, size_t index);
// Get the user-defined data of the specified index
void *world_get_data(World world, size_t index);
// Get the SpatialMap at the given index
SpatialMap *world_get_map(World world, size_t index);
// Remove the arcade object at an index
ArcadeObject world_remove(World *world, size_t index);
// Run an update step, where milliseconds is the milliseconds that has passed since the last world_update, 
// update is the function to apply to the game objects,
// and collide is the function to handle collisions
void world_update(World world, float milliseconds, WorldUpdate update, WorldCollide collide);
// Apply a function to all arcade objects in the world
void world_foreach(World world, WorldUpdate update);
// Draw the world to the screeen
void world_draw(World world);
// Deallocate the world's memory
void world_destroy(World world);

// *** GROUPS ***
// Create a new group
Group group_new();
// group will now not interact with other
void group_blacklist(Group *group, Group *other);
// group will not interact with itself
void group_blacklist_self(Group *group);
// group will now interact with other
void group_whitelist(Group *group, Group *other);
// group will now interact with itself
void group_whitelist_self(Group *group);
// Checks to see if a will interact with b
bool group_interacts(Group *a, Group *b);

// *** ARCADE OBJECTS ***
// Create a new game object with a given world bound and sprite. Solid objects act like walls; other objects may not pass through them
ArcadeObject arcobj_new(Shape bounds, bool solid, Sprite spr);
// Checks to see if two objects should interact, to allow things like bullets to ignore friendly units
bool arcobj_interacts(ArcadeObject *a, ArcadeObject *b);

// *** LEVELS ***
// Load a level from a filename, loading the assets using the asset manager provided
Level level_load(char *filename, AssetManager assets, size_t data_size);
// Get a copy of the levels world or a reference, depending on if it is persistent
World level_get_world(Level level);
// Deallocate the memory for the level and its backing world
void level_destroy(Level level);

// *** GAME ***
// Creates a new game instance, loading the levels defined in level_names in the order defined in indices
Game game_new(WindowConfig config, char **level_names, size_t *indices, size_t num_levels);
// Takes control of execution and sets global data
void game_start(Game game);
// Uses global state
void game_stop();
// Sets global state
void game_restart();
