#pragma once

#include <SDL.h>
#include <stdbool.h>

typedef struct {
	bool resizable, fullscreen, borderless;
	int width, height;
	const char *title;
} WindowConfig;

typedef struct {
	bool left, right, middle, x1, x2, wheel_up, wheel_down;
} MouseState;

typedef struct {
	SDL_Window *window;
	SDL_Renderer *rend;
	bool stay_open; 
	MouseState mouse, prev_mouse;
	bool *keys, *prev_keys;
	Uint32 frame_start;
} Window;

#include "lines.h"

struct Sprite;
/*
 * Pack attributes into a configuration struct
 */
WindowConfig window_config_new(int width, int height, const char *title);
/*
 * Spawn a window with the given configuration
 * Initializes multimedia libraries and spawns an SDL window
 */
Window window_new(WindowConfig config);
/*
 * Runs the event loop and check for keyboard and mouse events
 * Will update the current and previous input state
 * Will also check to see if the Window should close
 */
void window_events(Window *window);
/*
 * Start drawing with a clear color given by r, g, b (bound from 0 to 255)
 */
void window_start_draw(Window *window, int r, int g, int b);
/*
 * End the draw call (present the draw to the screen)
 * Will sleep spare time to finish the frame to maintain 60 FPS
 */
void window_end_draw(Window window);
/*
 * If the Window has received a close event
 */
bool window_should_contine(Window window);
/*
 * Checks to see if a given key is currently held down
 * Key constants given by SDL key constants
 */
bool window_key_pressed(Window window, int key_code);
/*
 * Checks to see if a given mouse button is currently held down
 * BUtton constants given by SDL constants
 */
bool window_mouse_pressed(Window window, int button);
/*
 * Checks to see if a key was held last frame
 */
bool window_key_was_pressed(Window window, int key_code);
/*
 * Checks to see if a button was held last frame
 */
bool window_mouse_was_pressed(Window window, int button);
/*
 * Gets the mouse position in screen coordinates
 */
Vector2 window_get_mouse_pos(Window window);
/*
 * Draw a Sprite to the Window
 */
void window_draw(Window window, struct Sprite sprite);
/*
 * Cleans up the Window and all multimedia libraries
 */
void window_destroy(Window window);
