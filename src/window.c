#include "window.h"

#include "util.h"

WindowConfig window_config_new(int width, int height, const char *title) {
	return (WindowConfig) { 
		.resizable = false, 
		.fullscreen = false,
		.borderless = false,
		.width = width,
		.height = height,
		.title = title 
	};
}

Window window_new(WindowConfig config) {
	SDL_Window *window = SDL_CreateWindow(config.title, 
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.width, config.height, 
			(SDL_WINDOW_FULLSCREEN_DESKTOP & config.fullscreen) |
			(SDL_WINDOW_BORDERLESS & config.borderless));
	SDL_Renderer *rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	return (Window) {
		.window = window,
		.rend = rend,
		.stay_open = true
	};
}

void window_update(Window window);
void window_start_draw(Window window, int r, int g, int a);
void window_end_draw(Window window);
bool window_should_contine(Window window);
void window_close(Window window);
bool window_key_pressed(Window window, int key_code);
bool window_mouse_pressed(Window window, int button);
Vector2 window_get_mouse_pos(Window window);
void window_set_mouse_pos(Window window, Vector2 pos);

void window_draw(Window window, Sprite sprite) {
	TextureRegion region = spr_image(sprite);
	SDL_RendererFlip flip = SDL_FLIP_NONE | (SDL_FLIP_HORIZONTAL & sprite.flip_x) | (SDL_FLIP_VERTICAL & sprite.flip_y);
	SDL_Point point = { (int) sprite.origin.x, (int) sprite.origin.y };
	SDL_Rect src = rect_conv(region.region);
	SDL_Rect dest = rect_conv(sprite.bounds);
	SDL_RenderCopyEx(window.rend, region.source.texture, &src, &dest, sprite.angle, &point, flip);
}

void window_destroy(Window window) {
	SDL_DestroyRenderer(window.rend);
	SDL_DestroyWindow(window.window);
}
