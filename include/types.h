#pragma once

#define DECL(name) struct name; typedef struct name name

DECL(Animation);
DECL(ArcadeObject);
DECL(AssetManager);
DECL(Camera);
DECL(Circ);
DECL(Game);
DECL(Group);
DECL(Level);
DECL(Line);
DECL(MouseState);
DECL(Polygon);
DECL(QuadNode);
DECL(QuadTree);
DECL(Rect);
DECL(Shape);
DECL(SpatialMap);
DECL(Sprite);
DECL(Texture);
DECL(TextureRegion);
DECL(Transform);
DECL(Window);
DECL(WindowConfig);
DECL(World);
DECL(Vector2);

typedef void (*WorldUpdate)(World, ArcadeObject*, void*);
typedef void (*WorldCollide)(World, ArcadeObject*, void*, ArcadeObject*, void*);

#include <SDL.h>
#include <stddef.h>

#include "array_list.h"
#include "hashmap.h"

// *** GEOMETRY / MATH ***
//A circle
struct Circ {
	float x, y, radius;
};
//A 2D Axis-Aligned Bounding Box
struct Rect {
	float x, y, width, height;
};
//A 2D vector
struct Vector2 {
	float x, y;
}; 
//A line segment
struct Line {
	Vector2 start, end;
};
//A 2D n-sided polygon
struct Polygon {
	Vector2 pos; //The displacement of the shape
	Vector2 origin; //The rotational origin of the shape
	Vector2 *points; //The buffer of the points in the polygon
	size_t num_points; //The number of points in the buffer
	float rotation; //The degrees of rotation of the shape
	float scale; //The scale factor of the shape
};
//A generic handle for a 2D shape
struct Shape {
	union {
		Rect r;
		Circ c;
		Polygon p;
	} data; //Store the shape types in a union
	enum { SHAPE_IS_RECT, SHAPE_IS_CIRC, SHAPE_IS_POLY} type; //The type of shape it's storing
	float rot; //Store the rotation
};

// *** GRAPHICS ***
//Represents a texture
struct Texture {
	int width, height; //the width and height of the texture
	SDL_Texture *texture; //the SDL texture backing
};
//A piece of a texture, used for drawing
struct TextureRegion {
	Texture source; //the source of the texture region
	Rect region; //the part to draw
};
//Store an animation that can be drawn to the screen
struct Animation {
	ArrayList frames; //An ArrayList of TextureRegions
	size_t current_steps; //The number of steps that have passed in the current 
	size_t steps_per_frame; //The total number of steps until the frame moves on
	size_t current_frame; //The index of the current frame
};
//A game sprite in the game world
struct Sprite {
	union {
		Animation anim;
		TextureRegion tex;
	} src; //Store the drawable backing as either animated or static
	bool dynamic; //if the image is dynamic (animated) or static
	Vector2 position, origin, scale; //the physical properties
	float angle; //the rotation
	bool flip_x, flip_y; //whether to flip the texture or animation
};
//A set of configuration options for creating a window
struct WindowConfig {
	bool resizable, fullscreen, borderless;
	int width, height;
	const char *title;
};
//The state of the mouse buttons
struct MouseState {
	bool left, right, middle, x1, x2, wheel_up, wheel_down;
};
//Represent an sdl window and attached ancillary items
struct Window {
	SDL_Window *window; //the backing window
	SDL_Renderer *rend; //hardware renderer attached to window
	bool stay_open; //if the window should remain open
	MouseState mouse, prev_mouse; //mouse input state
	bool *keys, *prev_keys; //keyboard input state
	Uint32 frame_start; //the clock time at which the frame started
	int width, height; //the width and height of the window
};
//A struct to store loaded textures to allow simple memory management and disable texture duplication
struct AssetManager {
	Window window; //The window to use to load textures
	HashMap *data; //The hashmap to store the texture data in
};
//Determine what is drawn to the screen
struct Camera {
	Rect game_area; //The area of the world to draw
	Vector2 follow_padding; //The distance the object is kept from the edge of the screen
	Vector2 follow_speed; // the speed at which the camera tracks the follow object
	Window *window; //The Window that is being drawn to
	int follow_index; //The object that the camera should follow, -1 to not follow
};

// *** SIMULATION ***
// A node in the quadtree
struct QuadNode {
	QuadNode *children[4]; // The four children of the node that are the four quarters, NULL if this is a leaf
	Rect region; // The region that this node contains
	ArrayList contains; // The indices that this node has
};
//A structure that recursively divides game space into quarters to make collision efficient
struct QuadTree {
	QuadNode *root; //The root node of the quadtree
	ArrayList entities; //All of the entities stored in the quadtree (list of ArcadeObject)
	ArrayList groups; //The different groups stored in the quadtree (list of Group)
};
//A structure that divides game space into tiles to allow static objects
struct SpatialMap {
	float width, height, tile_width, tile_height; //physical properties
	ArrayList items; //the buffer for the items in the map (list of user-defined items)
	ArrayList has; //buffer of flags that determine if an item is present at that index (bool)
};
//A struct that unifies various game properties and objects for easy manipulation
struct World {
	Window *window; //pointer to the window for drawing
	Camera camera; //the camera instance
	QuadTree entities; //the game entities
	ArrayList items; //a list of user-defined data
	ArrayList layers; //a list of the maps (list of SpatialMap)
	int r, g, b; //background color, black by default
};
//A group of entities
struct Group {
	uint64_t id, blacklist;
};
//A game object
struct ArcadeObject {
	Sprite sprite; //The current game sprite
	Shape bounds; //The physical, in-world hitbox
	Vector2 velocity, acceleration, max_velocity, drag; //Physical properties
	bool solid; //If other objects are stopped on collision (TODO: implement)
	bool alive; //If the object should be considered in updateds
	bool bounce; //Should the object bounce off obstacles elastically
	Group *group; //The group that determines interactions between objects
	size_t index; //The index of the object in the world (set automatically)
};
//A structure that holds data for a level
struct Level {
	World data; //the interpreted world
	bool persistent; //if the level should be the same if it is returned to
};
//A structure that unifies all game items
struct Game {
	AssetManager assets; //The game assets
	World current; //the current world
	Window *window; //the window (heap-allocated)
	ArrayList levels; //the levels in the game (list of Game)
};

