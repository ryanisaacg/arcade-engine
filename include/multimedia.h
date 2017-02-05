#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "array_list.h"
#include "hashmap.h"

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
	const char *title;
} WindowConfig;
///The state of the mouse buttons
typedef struct MouseState {
	bool left, right, middle, x1, x2, wheel_up, wheel_down;
} MouseState;
///Represent an sdl window and attached ancillary items
typedef struct Window {
	SDL_Window *window; ///the backing window
	SDL_Renderer *rend; ///hardware renderer attached to window
	bool stay_open; ///if the window should remain open
	MouseState mouse, prev_mouse; ///mouse input state
	bool *keys, *prev_keys; ///keyboard input state
	Uint32 frame_start; ///the clock time at which the frame started
	int width, height; ///the width and height of the window
} Window;
///A struct to store loaded textures to allow simple memory management and disable texture duplication
typedef struct AssetManager {
	Window window; ///The window to use to load textures
	HashMap *data; ///The hashmap to store the multimedia data in (Texture, Sound, and Music)
} AssetManager;
///Determine what is drawn to the screen
typedef struct Camera {
	Rect game_area; ///The area of the world to draw
	Window *window; ///The Window that is being drawn to
	bool letterbox; ///If the camera should respond to mismatched aspect ratios by using bars
	int offset_x; ///The amount all projections should be offset on the x axis
	int offset_y; ///The amount all projections should be offset on the y axis
	Vector2 scale; ///The scale that will be applied to all images
} Camera;
///A music track; only one track can be played at once
typedef struct Music {
	///\private
	Mix_Music *music;
} Music;
///A sound sample
typedef struct Sound {
	///\private
	Mix_Chunk *chunk;
	/**
	 * \brief The default volume for playing the sound sample from 0 to 128
	 *
	 * Can be overriden by playing the sound with the sound_play_volume function
	 */
	int volume;
} Sound;

// *** INITIALIZATION
/// Initialize all dependency multimedia libraries
void multimedia_init();
/// Deinitialize all dependency multimedia libraries
void multimedia_quit();


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
 * \brief Checks to see if a given key is currently held down 
 *
 * Key constants are given by SDL key constants. Specifically, the SDL_SCANCODE_XXX constant should be used.
 */
bool window_key_pressed(Window window, int key_code);
/** 
 * \brief Checks to see if a given mouse button is currently held down 
 *
 * Button constants are given by SDL button constants
 */
bool window_mouse_pressed(Window window, int button);
/// Checks to see if a key was held last frame
bool window_key_was_pressed(Window window, int key_code);
/// Checks to see if a button was held last frame
bool window_mouse_was_pressed(Window window, int button);
/// Gets the mouse position in screen coordinates
Vector2 window_get_mouse_pos(Window window);
/**
 * \brief Draw a Sprite to the Window
 *
 * camera is an optional Camera argument (NULL = no camera)
 */
void window_draw(Window window, Camera *cam, struct Sprite sprite);
/// Cleans up the Window
void window_destroy(Window window);


// *** ASSETS ***
///Create a new asset manager using that window
AssetManager asset_new(Window window);
///Load or retrieve a raw Texture from a file
Texture asset_load_texture(AssetManager assets, char *path);
///Load or retrieve a Music track from a file
Music asset_load_music(AssetManager assets, char *path);
///Load or retrieve a Sound sample from a file
Sound asset_load_sound(AssetManager assets, char *path);
///Destroy the manager and the loaded textures
void asset_destroy(AssetManager assets);


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


// *** MUSIC ***
/// Load music from a file
Music music_new(char *path);
/// Play a music track a certain number of times through
void music_play(Music track, int times);
/// Fade in a music track over a certain number of milliseconds a certain number of times through
void music_fade_in(Music track, int times, int milliseconds);
/// Loop a music track indefinitely
void music_loop(Music track);
/// Fade in a music track over a certain number of milliseconds then loop indefinitely
void music_fade_in_loop(Music track, int milliseconds);
/// Checks if a music track is currently playing
bool music_is_playing();
/// Check if a music track is paused
bool music_is_paused();
/// Check if a music track is fading in or out
bool music_is_fading();
/// Resume the music track if it is paused
void music_resume();
/// Pause the music track; only use when music is playing
void music_pause();
/// Stop the music track
void music_stop();
/// Rewind the music to the beginning
void music_rewind();
/// Fade out the current track and then stop over a given number of milliseconds
void music_fade_out(int milliseconds);
/// Set a position in seconds for a point in the music track
void music_set_position(double seconds);
/**
 * \brief Call the given function when the music finishes
 *
 * DO NOT call music or sound functions in the callback
 */
void music_finished_callback(void (*callback)());
/// Sets the volume of the music (from 0 to 128)
void music_set_volume(int volume);
/// Deallocate a music track
void music_destroy(Music track);

// *** SOUND ***
/// Load a sound from a file
Sound sound_new(char *path);
/// Play a sound at its stored volume
void sound_play(Sound sound);
/// Play a sound at a specific volume from 0 to 128
void sound_play_volume(Sound sound, int volume);
/// Play a sound a given number of times at its stored volume
void sound_repeat(Sound sound, int loops);
/// Play a sound a given number of times at a specific volume from 0 to 128
void sound_repeat_volume(Sound sound, int loops, int volume);
/// Deallocate a sound sample
void sound_destroy(Sound sound);
