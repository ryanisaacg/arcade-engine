#include "window.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>

#include "sprite.h"
#include "util.h"

#define SDL_NUM_KEYS 284

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
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		exit(-1);
	}
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if(IMG_Init(flags) != flags) {
		fprintf(stderr, "SDL_Image could not initialize. IMG Error: %s\n", IMG_GetError());
		exit(-1);
	}
	SDL_Window *window = SDL_CreateWindow(config.title, 
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.width, config.height, 
			(SDL_WINDOW_FULLSCREEN_DESKTOP & config.fullscreen) |
			(SDL_WINDOW_BORDERLESS & config.borderless));
	SDL_Renderer *rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	return (Window) {
		.window = window,
		.rend = rend,
		.stay_open = true,
		.keys = malloc(sizeof(bool) * SDL_NUM_KEYS),
		.prev_keys = malloc(sizeof(bool) * SDL_NUM_KEYS),
		.mouse = { 0 },
		.prev_mouse = { 0 },
	};
}

void window_events(Window *window) {
	window->prev_mouse = window->mouse;
	window->mouse.wheel_up = window->mouse.wheel_down = false;
	memcpy(window->prev_keys, window->keys, sizeof(bool) * SDL_NUM_KEYS);
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			window->stay_open = false;
			break;
		case SDL_KEYDOWN:
			window->keys[e.key.keysym.scancode] = true;
			break;
		case SDL_KEYUP:
			window->keys[e.key.keysym.scancode] = false;
			break;
		case SDL_MOUSEWHEEL:
			if(e.wheel.y > 0) {
				window->mouse.wheel_up = true;
			} else if(e.wheel.y < 0) {
				window->mouse.wheel_down = true;
			}
			break;
		}
	}
	Uint32 state = SDL_GetMouseState(NULL, NULL);
	window->mouse.left = state & SDL_BUTTON_LEFT;
	window->mouse.right = state & SDL_BUTTON_RIGHT;
	window->mouse.middle = state & SDL_BUTTON_MIDDLE;
	window->mouse.x1 = state & SDL_BUTTON_X1;
	window->mouse.x2 = state & SDL_BUTTON_X2;
}

void window_start_draw(Window *window, int r, int g, int b) {
	SDL_SetRenderDrawColor(window->rend, r, g, b, 255);
	SDL_RenderClear(window->rend);
	window->frame_start = SDL_GetTicks();

}

void window_end_draw(Window window) {
	SDL_RenderPresent(window.rend);
	Uint32 current = SDL_GetTicks();
	Uint32 delay = 16 - (current - window.frame_start);
	if(16 > current - window.frame_start) {
		SDL_Delay(delay);
	}
}

bool window_should_contine(Window window) {
	return window.stay_open;
}

bool window_key_pressed(Window window, int key_code) {
	return window.keys[key_code];
}

static bool mouse_state_pressed(MouseState mouse, int button) {
	switch(button) {
	case SDL_BUTTON_LEFT:
		return mouse.left;
	case SDL_BUTTON_RIGHT:
		return mouse.right;
	case SDL_BUTTON_MIDDLE:
		return mouse.middle;
	case SDL_BUTTON_X1:
		return mouse.x1;
	case SDL_BUTTON_X2:
		return mouse.x2;
	}
	return false;
}

bool window_mouse_pressed(Window window, int button) {
	return mouse_state_pressed(window.mouse, button);
}

bool window_key_was_pressed(Window window, int key_code) {
	return window.prev_keys[key_code];
}

bool window_mouse_was_pressed(Window window, int button) {
	return mouse_state_pressed(window.prev_mouse, button);
}

Vector2 window_get_mouse_pos(Window window) {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return vec2_new(x, y);
}

void window_draw(Window window, Sprite sprite) {
	TextureRegion region = spr_image(sprite);
	SDL_RendererFlip flip = SDL_FLIP_NONE | (SDL_FLIP_HORIZONTAL & sprite.flip_x) | (SDL_FLIP_VERTICAL & sprite.flip_y);
	SDL_Point point = { (int) sprite.origin.x, (int) sprite.origin.y };
	SDL_Rect src = rect_conv(region.region);
	SDL_Rect dest = { (int) sprite.position.x, (int) sprite.position.y, src.w * sprite.scale.x, src.h * sprite.scale.y };
	SDL_RenderCopyEx(window.rend, region.source.texture, &src, &dest, sprite.angle, &point, flip);
}

void window_destroy(Window window) {
	SDL_DestroyRenderer(window.rend);
	SDL_DestroyWindow(window.window);
	SDL_Quit();
	IMG_Quit();
}
