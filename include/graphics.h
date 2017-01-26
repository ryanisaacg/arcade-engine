#pragma once

#include "geom.h"
//Represents a texture
typedef struct Texture {
	int width, height; //the width and height of the texture
	SDL_Texture *texture; //the SDL texture backing
} Texture;
//A piece of a texture, used for drawing
typedef struct TextureRegion {
	Texture source; //the source of the texture region
	Rect region; //the part to draw
} TextureRegion;
//Store an animation that can be drawn to the screen
typedef struct Animation {
	ArrayList frames; //An ArrayList of TextureRegions
	size_t current_steps; //The number of steps that have passed in the current 
	size_t steps_per_frame; //The total number of steps until the frame moves on
	size_t current_frame; //The index of the current frame
} Animation;
//A game sprite in the game world
typedef struct Sprite {
	union {
		Animation anim;
		TextureRegion tex;
	} src; //Store the drawable backing as either animated or static
	bool dynamic; //if the image is dynamic (animated) or static
	Vector2 position, origin, scale; //the physical properties
	float angle; //the rotation
	bool flip_x, flip_y; //whether to flip the texture or animation
} Sprite;
//A set of configuration options for creating a window
typedef struct WindowConfig {
	bool resizable, fullscreen, borderless;
	int width, height;
	const char *title;
} WindowConfig;
//The state of the mouse buttons
typedef struct MouseState {
	bool left, right, middle, x1, x2, wheel_up, wheel_down;
} MouseState;
//Represent an sdl window and attached ancillary items
typedef struct Window {
	SDL_Window *window; //the backing window
	SDL_Renderer *rend; //hardware renderer attached to window
	bool stay_open; //if the window should remain open
	MouseState mouse, prev_mouse; //mouse input state
	bool *keys, *prev_keys; //keyboard input state
	Uint32 frame_start; //the clock time at which the frame started
	int width, height; //the width and height of the window
} Window;
//A struct to store loaded textures to allow simple memory management and disable texture duplication
typedef struct AssetManager {
	Window window; //The window to use to load textures
	HashMap *data; //The hashmap to store the texture data in
} AssetManager;
//Determine what is drawn to the screen
typedef struct Camera {
	Rect game_area; //The area of the world to draw
	Vector2 follow_padding; //The distance the object is kept from the edge of the screen
	Vector2 follow_speed; // the speed at which the camera tracks the follow object
	Window *window; //The Window that is being drawn to
	int follow_index; //The object that the camera should follow, -1 to not follow
} Camera;

// *** TEXTURES ***
Texture texture_new(Window window, char *texture_path);
void texture_destroy(Texture tex);
TextureRegion texregion_new(Texture source);
TextureRegion texregion_new_sized(Texture source, Rect region);
TextureRegion texregion_slice(TextureRegion region, Rect subregion);

// *** ANIMATIONS ***
// Creates a new Animation from a buffer
Animation anim_new(TextureRegion *frames, size_t num_frames, size_t steps_per_frame);
// Creates an animation from a spritesheet
//Frame Order is an array of points where the point is on the grid of sides tile_width, tile_height
Animation anim_new_spritesheet(TextureRegion source, int tile_width, int tile_height, int frame_order[][2], size_t frames, size_t steps_per_frame);
// Creates an animation from a spritesheet laid out left to right sequentially
Animation anim_new_linear_sheet(TextureRegion source, int tile_width, size_t steps_per_frame);
// Get the current frame of the animation
TextureRegion anim_get_current(Animation anim);
// Step the animation forward and return the current frame
TextureRegion anim_step(Animation *anim);

// *** SPRITES *** 
//Create a sprite that only uses an image
Sprite spr_new_static(TextureRegion region, Vector2 pos);
//Create a sprite that uses an animation
Sprite spr_new_animated(Animation anim, Vector2 pos);
//Get the current image of the sprite
TextureRegion spr_image(Sprite spr);
//Steps the sprite forward a frame
void spr_step(Sprite *spr);
//Center the rotational origin
void spr_center_origin(Sprite *spr);

// *** WINDOWING ***
// Pack attributes into a configuration struct
WindowConfig window_config_new(int width, int height, const char *title);
// Spawn a window with the given configuration
// Initializes multimedia libraries and spawns an SDL window
Window window_new(WindowConfig config);
// Runs the event loop and check for keyboard and mouse events
// Will update the current and previous input state
// Will also check to see if the Window should close
void window_events(Window *window);
// Start drawing with a clear color given by r, g, b (bound from 0 to 255)
void window_start_draw(Window *window, int r, int g, int b);
// End the draw call (present the draw to the screen)
// Will sleep spare time to finish the frame to maintain 60 FPS
void window_end_draw(Window window);
// If the Window has received a close event
bool window_should_contine(Window window);
// Checks to see if a given key is currently held down
// Key constants given by SDL key constants
bool window_key_pressed(Window window, int key_code);
// Checks to see if a given mouse button is currently held down
// Button constants given by SDL constants
bool window_mouse_pressed(Window window, int button);
// Checks to see if a key was held last frame
bool window_key_was_pressed(Window window, int key_code);
// Checks to see if a button was held last frame
bool window_mouse_was_pressed(Window window, int button);
// Gets the mouse position in screen coordinates
Vector2 window_get_mouse_pos(Window window);
// Draw a Sprite to the Window
// Camera is an optional camera argument (NULL = no camera)
void window_draw(Window window, Camera *cam, struct Sprite sprite);
// Cleans up the Window and all multimedia libraries
void window_destroy(Window window);


// *** ASSETS ***
//Create a new asset manager using that window
AssetManager asset_new(Window window);
//Load or retrieve a raw texture from a file
Texture asset_load(AssetManager assets, char *path);
//Destroy the manager and the loaded textures
void asset_destroy(AssetManager assets);


// *** CAMERA ***
//Create a new camera
Camera cam_new(Window *window, Rect viewport);
//Project a point from game coordinates to the screen
SDL_Point cam_project_point(Camera cam, Vector2 point);
//Unproject a point from the screen to the game
Vector2 cam_unproject_point(Camera cam, SDL_Point screen);
//Project a rectangle from the game to the screen
SDL_Rect cam_project_rect(Camera cam, Rect game_area);
//Unproject a rectangle from the screen to the game
Rect cam_unproject_rect(Camera cam, SDL_Rect screen);
//Follow a specific object
void cam_set_follow(Camera *cam, ArcadeObject *obj);
//Stop following the object
void cam_clear_follow(Camera *cam);
