#pragma once

#include <SDL2/SDL.h>

#include "array_list.h"
#include "input.h"
#include "geom.h"

/**
 * \file graphics.h
 * Defines an interface to 2D hardware-accelerated graphics
 */

///Represents a loaded image file
typedef struct Texture {
	int width; ///The width of the texture
	int height; ///The height of the texture
	SDL_Texture *texture; //the SDL texture backing
} Texture;
///A rectangular piece of a Texture that can be drawn
typedef struct TextureRegion {
	Texture source; ///the Texture that the region is a slice of
	Rect region; ///the area of the texture to draw
} TextureRegion;
///Store an animation that can be drawn to the screen
typedef struct Animation {
	ArrayList frames; ///An ArrayList of TextureRegion s
	size_t current_steps; ///The number of steps that have passed in the current frame
	size_t steps_per_frame; ///The total number of steps until the frame moves on
	size_t current_frame; ///The index of the current frame
} Animation;
///A game sprite in the game world
typedef struct Sprite {
	union {
		Animation anim;
		TextureRegion tex;
	} src; ///Store the drawable backing as either animated or static
	bool dynamic; ///if the image is dynamic (animated) or static
	Vector2 position, origin, scale; ///the physical properties
	float angle; ///the rotation
	bool flip_x, flip_y; ///whether to flip the texture or animation
} Sprite;
///A set of configuration options for creating a window
typedef struct WindowConfig {
	bool resizable, fullscreen, borderless;
	int width, height;
	unsigned int framerate;
	const char *title;
} WindowConfig;
///Represent an sdl window and attached ancillary items
typedef struct Window {
	///\private
	SDL_Window *window; //the backing window
	///\private
	SDL_Renderer *rend; //hardware renderer attached to window
	///private
	bool stay_open;
	MouseState mouse, prev_mouse; ///mouse input state
	KeyboardState keys, prev_keys; ///keyboard input state
	ArrayList controllers; /// a list of GamepadState s
	///\private
	Uint32 frame_start; //the clock time at which the frame started
	int width, height; ///the width and height of the window
	unsigned int framerate; ///the number of game ticks and draw calls per second
} Window;
///Determine what is drawn to the screen
typedef struct Camera {
	Rect game_area; ///The area of the world to draw
	Window *window; ///The Window that is being drawn to
	bool letterbox; ///If the camera should respond to mismatched aspect ratios by using bars
	int offset_x; ///The amount all projections should be offset on the x axis
	int offset_y; ///The amount all projections should be offset on the y axis
	Vector2 scale; ///The scale that will be applied to all images
} Camera;
// *** TEXTURES ***
Texture texture_new(Window window, char *texture_path);
void texture_destroy(Texture tex);
TextureRegion texregion_new(Texture source);
TextureRegion texregion_new_sized(Texture source, Rect region);
TextureRegion texregion_slice(TextureRegion region, Rect subregion);

// *** ANIMATIONS ***
/// Creates a new Animation from a buffer
Animation anim_new(TextureRegion *frames, size_t num_frames, size_t steps_per_frame);
/// \brief Creates an animation from a spritesheet
///
///Frame Order is an array of points where the point is on the grid of sides tile_width, tile_height
Animation anim_new_spritesheet(TextureRegion source, int tile_width, int tile_height, int frame_order[][2], size_t frames, size_t steps_per_frame);
/// Creates an animation from a spritesheet laid out left to right sequentially
Animation anim_new_linear_sheet(TextureRegion source, int tile_width, size_t steps_per_frame);
/// Get the current frame of the animation
TextureRegion anim_get_current(Animation anim);
/// Step the animation forward and return the current frame
TextureRegion anim_step(Animation *anim);

// *** SPRITES *** 
///Create a sprite that only uses an image
Sprite spr_new_static(TextureRegion region, Vector2 pos);
///Create a sprite that uses an animation
Sprite spr_new_animated(Animation anim, Vector2 pos);
///Get the current image of the sprite
TextureRegion spr_image(Sprite spr);
///Steps the sprite forward a frame
void spr_step(Sprite *spr);
///Center the rotational origin
void spr_center_origin(Sprite *spr);

// *** WINDOWING ***
/// Pack attributes into a configuration struct
WindowConfig window_config_new(int width, int height, const char *title);
/// Spawn a window with the given configuration
Window window_new(WindowConfig config);
/**
 * \brief Runs the event loop and check for keyboard and mouse events
 *
 * Updates the current and previous input state
 *
 * Will also check to see if the Window should close
 */
void window_events(Window *window);
/// Start drawing with a clear color given by r, g, b (bound from 0 to 255)
void window_start_draw(Window *window, int r, int g, int b);
/**
 * \brief Draw the sprites to the screen 
 *
 * To maintain 60 FPS, this function sleep until 1/60 seconds have passed since window_start_draw
 */
void window_end_draw(Window window);
/// If the Window has received a close event
bool window_should_contine(Window window);
/**
 * \brief Draw a Sprite to the Window
 *
 * camera is an optional Camera argument (NULL = no camera)
 */
void window_draw(Window window, Camera *cam, Sprite sprite);
/// Cleans up the Window
void window_destroy(Window window);
// *** CAMERA ***
///Create a new camera
Camera cam_new(Window *window, Rect viewport, bool letterbox);
///Project a point from game coordinates to the screen
SDL_Point cam_project_point(Camera cam, Vector2 point);
///Unproject a point from the screen to the game
Vector2 cam_unproject_point(Camera cam, SDL_Point screen);
///Project a rectangle from the game to the screen
SDL_Rect cam_project_rect(Camera cam, Rect game_area);
///Unproject a rectangle from the screen to the game
Rect cam_unproject_rect(Camera cam, SDL_Rect screen);
/**\brief Update a camera's offset and scale
 *
 * Must be called after Window is resized or Camera is changed
 */
void cam_update(Camera *cam);

