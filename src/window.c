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
	
}

Window window_new_batch(WindowConfig config, Batch batch) {
	if(window_refcount == 0) { //initialize GLEW and GLFW
		glfwInit();	
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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

void window_begin(Window *window);
void window_end(Window *window);
bool window_should_contine(Window window);
bool window_key_pressed(Window window, int key_code);
bool window_mouse_pressed(Window window, int button);
Vector2 window_get_mouse_pos(Window window);
void window_set_mouse_pos(Window window, Vector2 pos);
void window_destroy(Window window);
