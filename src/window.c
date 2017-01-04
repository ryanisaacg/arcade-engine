#include "window.h"

WindowConfig window_config_new(int width, int height, const char *title) {
	return (WindowConfig) { .resizable = false, 
							.fullscreen_monitor = -1,
							.width = width,
							.height = height,
							.title = title };
}

int window_refcount = 0;

Window window_new(WindowConfig config) {
	return window_new_batch(config, batch_new_default());
}

Window window_new_batch(WindowConfig config, Batch batch) {
	if(window_refcount == 0) { //initialize GLEW and GLFW
		glfwInit();	
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glewExperimental = GL_TRUE;
		glewInit();
	}
	window_refcount++;
	glfwWindowHint(GLFW_RESIZABLE, config.resizable);
	GLFWmonitor *monitor;
	if(config.fullscreen_monitor != -1) {
		int number_monitors;
		GLFWmonitor **monitors = glfwGetMonitors(&number_monitors);
		monitor = monitors[config.fullscreen_monitor];
	} else {
		monitor = NULL;
	}
	return (Window) { glfwCreateWindow(config.width, config.height, config.title, 
							monitor, NULL), batch };
}

void window_begin(Window *window) {
	glfwPollEvents();
	batch_begin(&window->batch);
}

void window_end(Window *window) {
	batch_end(&window->batch);
	glfwSwapBuffers(window->window);
}

bool window_should_contine(Window window) {
	return glfwWindowShouldClose(window.window);
}

void window_close(Window window) {
	glfwSetWindowShouldClose(window.window, GL_TRUE);
}

bool window_key_pressed(Window window, int key_code) {
	return glfwGetKey(window.window, key_code) == GLFW_PRESS;
}

bool window_mouse_pressed(Window window, int button) {
	return glfwGetMouseButton(window.window, button) == GLFW_PRESS;
}

Vector2 window_get_mouse_pos(Window window) {
	double x, y;
	glfwGetCursorPos(window.window, &x, &y);
	return vec2_new((float)x, (float)y);
}

void window_set_mouse_pos(Window window, Vector2 pos) {
	glfwSetCursorPos(window.window, pos.x, pos.y);
}

void window_destroy(Window window) {
	glfwDestroyWindow(window.window);
	batch_destroy(window.batch);
	window_refcount--;
	if(window_refcount == 0) {
		glfwTerminate();
	}
}
