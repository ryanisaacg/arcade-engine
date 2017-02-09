#pragma once

#include "config.h"
#include "geom.h"
#include "multimedia.h"

/**
 * \file simulation.h
 *
 * \brief Defines the structures for the game state
 *
 * QuadTree stores movable or variable-sized objects, represented by ArcadeObject s. 
 * SpatialMap s store data in a tiled format. 
 * World s combine quadtrees and maps into a single unified convenience.
 */

/**
 * \brief A data structure to store ArcadeObject s
 *
 * QuadTree s recursively subdivide a region of space into quarters until the leaves are a certain size. This means that objects can greatly limit what other objects they must check collision against. To manipulate or query, use the qt_xxx functions.
 */
typedef struct QuadTree {
	///\private
	struct QuadTree *children[4]; // The four children of the node that are the four quarters, NULL if this is a leaf
	///\private
	Rect region; // The region that this node contains
	///\private
	ArrayList contains; // The indices that this node has
	///\private
	ArrayList *entities; //All of the entities stored in the quadtree (list of ArcadeObject)
	///\private
	ArrayList *groups; //The different groups stored in the quadtree (list of Group)
	///\private
	bool root; //if the quadtree is root
} QuadTree;
///A structure that divides game space into tiles to allow static objects
typedef struct SpatialMap {
	float width; ///The width of the space the map contains
	float height; ///The height of the space the map contains
	float tile_width; ///The width of each tile
	float tile_height; ///The height of each tile
	///\private
	ArrayList items; //the buffer for the items in the map (list of user-defined items)
	///\private
	ArrayList has; //buffer of flags that determine if an item is present at that index (bool)
	///\private
	bool drawable; //if the map is intended to be drawn (if so, the list of items should be TextureRegion s)
} SpatialMap;
///A struct that unifies various game properties and objects for easy manipulation
typedef struct World {
	///A pointer to the window to draw to
	Window *window;
	///The camera that defines the game viewport
	Camera camera;
	///\private
	QuadTree *entities; //the game entities
	///\private
	ArrayList items; //a list of user-defined data
	///\private
	ArrayList layers; //a list of the maps (list of SpatialMap)
	///\private
	ArrayList sprites; //a list of static sprites
	int r; ///the red component of the background color
	int g; ///the green component of the background color
	int b; ///the blue component of the background color
} World;
/**
 * \brief A group of entities that defines whether they interact
 *
 * Groups may blacklist other groups. 
 * If Group A blacklists Group B, ArcadeObject s of Group A will not interact with (e.g. register collisions with) entities of Group B. 
 * However, ArcadeObject s of Group B may still interact with Group A, unless they also blacklist.
 */
typedef struct Group {
	///\private
	uint64_t id;
	///\private
	uint64_t blacklist;
} Group;
///A game object
typedef struct ArcadeObject {
	Sprite sprite; ///The current game sprite
	Shape bounds; ///The physical, in-world hitbox
	Vector2 velocity; ///The pixels the object should move per frame
	Vector2 acceleration; ///The change in velocity per frame
	Vector2 max_velocity; ///The absolute value the velocity should not exceed
	Vector2 drag; ///The amount to oppose motion each frame (always tends towards 0)
	bool solid; ///If other objects are stopped on collision 
	bool alive; ///If the object should be considered in updates
	bool bounce; ///Should the object bounce off obstacles elastically
	Group *group; ///The group that determines interactions between objects
	size_t index; ///The index of the object in the world (set automatically)
} ArcadeObject;
///A structure that holds data for a level
typedef struct Level {
	///\private
	World data; //the interpreted world
	///If the level should maintain its state each time it is started
	bool persistent;
} Level;
///A structure that unifies all game items
typedef struct Game {
	AssetManager assets; ///The game assets
	World current; ///the current world
	Window *window; ///the window (heap-allocated)
	ArrayList levels; ///the levels in the game (list of Game)
	size_t current_level_index; ///The index of the current level
} Game;

typedef void *(*SpawnerFunction)(HashMap*);
///A struct to load properties of objects that allow them to spawn types of ArcadeObject s 
typedef struct Spawner {
	///\private
	Document document;
	///\private
	SpawnerFunction func;
} Spawner;

typedef void (*WorldUpdate)(World, ArcadeObject*, void*);
typedef void (*WorldCollide)(World, ArcadeObject*, void*, ArcadeObject*, void*);


// *** QUADTREES ***
/// Create a new quadtree where the leaf nodes will have at least min_width and min_height
QuadTree *qt_new(QuadTree *parent, Rect region, float min_width, float min_height);
///Find the smallest node that contains the given rectangle
QuadTree *qt_get_child(QuadTree *tree, Rect region);
/// Add an arcade object to the quadtree and return its index
size_t qt_add(QuadTree *tree, ArcadeObject obj);
/// Register a group to the quadtree and return a pointer to its heap-allocated location
Group *qt_add_group(QuadTree *tree, Group group);
/// Get a pointer to the given arcade object
ArcadeObject *qt_get(QuadTree *tree, size_t index);
/**
 * \brief Check for an arcade object at the given location. 
 *
 * No particular order in returning
 * 
 * NULL indicates no object was found
 */
ArcadeObject *qt_point_query(QuadTree *tree, Vector2 point, Group *query_as);
/// Check for an arcade object at the given region. No particular order in returning
/// NULL indicates no object was found
ArcadeObject *qt_region_query(QuadTree *tree, Shape region, Group *query_as);
/// Checks if a point of the quadtree is empty of solid objects
bool qt_point_free(QuadTree *tree, Vector2 point, ArcadeObject *ignore);
/// Checks if a region of the quadtree is empty of solid objects
bool qt_region_free(QuadTree *tree, Shape region, ArcadeObject *ignore);
/// Remove an object from the quadtree and return it
ArcadeObject qt_remove(QuadTree *tree, size_t index);
/// Clear all data from the quadtree but retain allocated memory
void qt_clear(QuadTree *tree);
/// Return the number of objects inserted into the quadtree
size_t qt_len(QuadTree *tree);
/// Run the collision checks and do whatever action collide performs
/// Objects will not interact with objects that have blacklisted groups
void qt_collisions(QuadTree *tree, World world, WorldCollide collide);
/// Destroy the quadtree and deallocate its memory
void qt_destroy(QuadTree *tree);

// *** SPATIAL MAPS ***
/// Create a new spatial map of a user-specified type
SpatialMap sm_new(size_t item_size, float width, float height, float tile_width, float tile_height, bool drawable);
/// Set the data at a given position
void sm_set(SpatialMap *map, void *item, float x, float y);
/// Remove the data at a given position
void sm_remove(SpatialMap *map, float x, float y);
/// Check to see if the position is within the bounds of the map
bool sm_contains(SpatialMap map, float x, float y);
/// Get the data at a given position (NULL indicates no data)
void *sm_get(SpatialMap map, float x, float y);
/// Check if the spatial map has data a given position
bool sm_has(SpatialMap map, float x, float y);
/// Check to see if the space bounded by the shape is entirely empty
bool sm_free(SpatialMap map, Shape region);
/// Deallocates the spatial map
void sm_destroy(SpatialMap map);

// *** SIMULATION WORLDS ***
/// Create a new world with the given properties. qt_bucket_size is the size of the quadtree leaves. data_size is the sizeof the user-defined data struct
World world_new(Window *window, float width, float height, float qt_buckets_size, size_t data_size);
/// Add a game object and associated user-defined data and get its index
size_t world_add(World *world, ArcadeObject object, void *data);
/// Add a spatial map to the world and get its index
size_t world_add_map(World *world, SpatialMap map);
/// Add a group to the world and get its heap allocated location
Group *world_add_group(World *world, Group group);
// Get the object of the specified index
/// Add a static sprite to be drawn each frame
void world_add_sprite(World *world, Sprite sprite);
/// Check if a point is empty within the world
bool world_point_free(World world, Vector2 point, ArcadeObject *ignore);
/// Check if a region is free within the world
bool world_region_free(World world, Shape region, ArcadeObject *ignore);
/// Get the object of the specified index
ArcadeObject *world_get(World world, size_t index);
/// Get the user-defined data of the specified index
void *world_get_data(World world, size_t index);
/// Get the SpatialMap at the given index
SpatialMap *world_get_map(World world, size_t index);
/// Remove the arcade object at an index
ArcadeObject world_remove(World *world, size_t index);
/// Run an update step, where update is the function to apply to the game objects and collide is the function to handle collisions
void world_update(World world, WorldUpdate update, WorldCollide collide);
/// Apply a function to all arcade objects in the world
void world_foreach(World world, WorldUpdate update);
/// Draw the world to the screeen
void world_draw(World world);
/// Deallocate the world's memory
void world_destroy(World world);

// *** GROUPS ***
/// Create a new group
Group group_new();
/// group will now not interact with other
void group_blacklist(Group *group, Group *other);
/// group will not interact with itself
void group_blacklist_self(Group *group);
/// group will now interact with other
void group_whitelist(Group *group, Group *other);
/// group will now interact with itself
void group_whitelist_self(Group *group);
/// Checks to see if a will interact with b
bool group_interacts(Group *a, Group *b);

// *** ARCADE OBJECTS ***
/// Create a new game object with a given world bound and sprite. Solid objects act like walls; other objects may not pass through them
ArcadeObject arcobj_new(Shape bounds, bool solid, Sprite spr);
/// Checks to see if two objects should interact, to allow things like bullets to ignore friendly units
bool arcobj_interacts(ArcadeObject *a, ArcadeObject *b);

// *** LEVELS ***
/// Load a level from a filename, loading the assets using the asset manager provided
///
/// spawner is an optional parameter, NULL for no spawner
Level level_load(char *filename, Spawner *spawner, AssetManager assets, size_t data_size);
/// Get a copy of the levels world or a reference, depending on if it is persistent
World level_get_world(Level level);
/// Deallocate the memory for the level and its backing world
void level_destroy(Level level);

// *** GAME ***
/// Creates a new game instance, loading the levels defined in level_names in the order defined in indices
/// 
/// spawner is an optional Spawner that will be applied to all loaded objects
Game game_new(WindowConfig config, char **level_names, size_t *indices, size_t num_levels, size_t data_size, Spawner *spawner);
/// Takes control of execution and sets global data
void game_start(Game game, WorldUpdate update, WorldCollide collide);
/// Uses global state
void game_stop();
/// Move the game to the level at the given index (global state)
void game_set_level(size_t index);
/// Advance the game to the next level (global state)
void game_next_level();
/// Move the game back a level (global state)
void game_prev_level();

/// Create a new Spawner using a document
Spawner spawn_new(Document doc, SpawnerFunction func);
/// Sets up an ArcadeObject
void spawn_get_obj(Spawner spawn, ArcadeObject *obj, char *name);
/// Get the default data from the Spawner
void *spawn_get_data(Spawner spawn, char *name);
