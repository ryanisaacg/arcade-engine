#pragma once

#include "types.h"

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
