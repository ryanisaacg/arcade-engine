#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "batch.h"
#include "lines.h"

typedef struct {
	bool resizable;
	int fullscreen_monitor, width, height;
	const char *title;
} WindowConfig;

typedef struct {
	GLFWwindow *window;
	Batch batch;
} Window;

WindowConfig window_config_new(int width, int height, const char *title);
Window window_new(WindowConfig config);
Window window_new_batch(WindowConfig config, Batch batch);
void window_begin(Window *window);
void window_end(Window *window);
bool window_should_contine(Window window);
bool window_key_pressed(Window window, int key_code);
bool window_mouse_pressed(Window window, int button);
Vector2 window_get_mouse_pos(Window window);
void window_set_mouse_pos(Window window, Vector2 pos);
void window_destroy(Window window);
