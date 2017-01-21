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

WindowConfig window_config_new(int width, int height, const char *title);
Window window_new(WindowConfig config);
void window_events(Window *window);
void window_start_draw(Window *window, int r, int g, int b);
void window_end_draw(Window window);
bool window_should_contine(Window window);
bool window_key_pressed(Window window, int key_code);
bool window_mouse_pressed(Window window, int button);
Vector2 window_get_mouse_pos(Window window);
void window_draw(Window window, struct Sprite sprite);
void window_destroy(Window window);
