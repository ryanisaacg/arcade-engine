#pragma once

#include <SDL.h>
#include <stdbool.h>

typedef struct {
	bool resizable, fullscreen, borderless;
	int width, height;
	const char *title;
} WindowConfig;

typedef struct {
	SDL_Window *window;
	SDL_Renderer *rend;
	bool stay_open;
} Window;

#include "lines.h"
#include "sprite.h"

WindowConfig window_config_new(int width, int height, const char *title);
Window window_new(WindowConfig config);
void window_update(Window window);
void window_start_draw(Window window, int r, int g, int a);
void window_end_draw(Window window);
bool window_should_contine(Window window);
void window_close(Window window);
bool window_key_pressed(Window window, int key_code);
bool window_mouse_pressed(Window window, int button);
Vector2 window_get_mouse_pos(Window window);
void window_set_mouse_pos(Window window, Vector2 pos);
void window_draw(Window window, Sprite sprite);
void window_destroy(Window window);
