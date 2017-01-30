#include "graphics.h"
#include "geom.h"
#include "simulation.h"

#include <SDL.h>
#include <stdlib.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

#define SDL_NUM_KEYS 284

Animation anim_new(TextureRegion *frames, size_t num_frames, size_t steps_per_frame) {
	ArrayList list = al_prealloc(sizeof(TextureRegion), frames, num_frames);
	list.length = num_frames;
	return (Animation) {
		.frames = list,
		.current_steps = 0,
		.steps_per_frame = steps_per_frame,
		.current_frame = 0 
	};
}

Animation anim_new_spritesheet(TextureRegion source, int tile_width, int tile_height, int frame_order[][2], size_t frames, size_t steps) {
	TextureRegion *frame_buffer = malloc(sizeof(TextureRegion) * frames);
	for(size_t i = 0; i < frames; i++) {
		int x = frame_order[i][0] * tile_width;
		int y = frame_order[i][1] * tile_height;
		TextureRegion frame = texregion_slice(source, rect_new(x, y, tile_width, tile_height));
		frame_buffer[i] = frame;
	}
	return anim_new(frame_buffer, frames, steps);
}

Animation anim_new_linear_sheet(TextureRegion source, int tile_width, size_t steps_per_frame) {
	size_t frames = (int)source.region.width / tile_width;
	int frame_order[frames][2];
	for(size_t i = 0; i < frames; i++) {
		frame_order[i][0] = i;
		frame_order[i][1] = 0;
	}
	return anim_new_spritesheet(source, tile_width, (int)source.region.height, frame_order, frames, steps_per_frame);
}

TextureRegion anim_get_current(Animation anim) {
	TextureRegion *current = al_get(anim.frames, anim.current_frame);
	return *current;
}

TextureRegion anim_step(Animation *anim) {
	//Advance a step (and a frame if necessary)
	anim->current_steps++;
	if(anim->current_steps >= anim->steps_per_frame) {
		anim->current_steps = 0;
		anim->current_frame = (anim->current_frame + 1) % anim->frames.length;
	}
	return anim_get_current(*anim);
}
Camera cam_new(Window *window, Rect viewport) {
	return (Camera) {
		.game_area = viewport,
		.window = window,
		.follow_index = -1
	};
}

static int xproject(Camera cam, float x) {
	return (int)(x * cam.window->width / cam.game_area.width);
}

static int yproject(Camera cam, float y) {
	return (int)(y * cam.window->height / cam.game_area.height);
}

static float xunproject(Camera cam, float x) {
	return x * cam.game_area.width / cam.window->width;
}

static float yunproject(Camera cam, float y) {
	return y * cam.game_area.height / cam.window->height;
}

SDL_Point cam_project_point(Camera cam, Vector2 point) {
	return (SDL_Point) { 
		.x = xproject(cam, cam.game_area.x + point.x),
		.y = yproject(cam, cam.game_area.y + point.y) 
	};
}

Vector2 cam_unproject_point(Camera cam, SDL_Point screen) {
	return (Vector2) {
		.x = xunproject(cam, screen.x) + cam.game_area.x, 
		.y = yunproject(cam, screen.y) + cam.game_area.y 
	};
}

SDL_Rect cam_project_rect(Camera cam, Rect game_area) {
	return (SDL_Rect) {
		.x = xproject(cam, cam.game_area.x + game_area.x),
		.y = yproject(cam, cam.game_area.y + game_area.y),
		.w = xproject(cam, game_area.width),
		.h = yproject(cam, game_area.height)
	};
}

Rect cam_unproject_rect(Camera cam, SDL_Rect screen) {
	return (Rect) {
		.x = xunproject(cam, screen.x) + cam.game_area.x,
		.y = yunproject(cam, screen.y) + cam.game_area.y,
		.width = xunproject(cam, screen.w),
		.height = yunproject(cam, screen.h)
	};
}

void cam_set_follow(Camera *cam, ArcadeObject *obj) {
	cam->follow_index = obj->index;
}

void cam_clear_follow(Camera *cam) {
	cam->follow_index = -1;
}
AssetManager asset_new(Window window) {
	return (AssetManager) {
		.window = window,
		.data = hm_new()
	};
}

Texture asset_load(AssetManager assets, char *path) {
	int hash = path[0] + path[1] + path[2] + path[3];
	Texture *texture;
	if(hm_has(assets.data, hash, path)) {
		texture = hm_get(assets.data, hash, path);
	} else {
		texture = malloc(sizeof(Texture));
		*texture = texture_new(assets.window, path);
		hm_put(assets.data, hash, path, texture);
	}
	return *texture;
}

void asset_destroy(AssetManager assets) {
	ArrayList keys = hm_get_keys(assets.data);
	for(size_t i = 0; i < keys.length; i++) {
		char *path = al_get(keys, i);
		int hash = path[0] + path[1] + path[2] + path[3];
		Texture *texture = hm_get(assets.data, hash, path);
		texture_destroy(*texture);
	}
	hm_destroy(assets.data);
}
Sprite spr_new_static(TextureRegion region, Vector2 pos) {
	return (Sprite) {
		.src = { .tex = region},
		.dynamic = false,
		.position = pos,
		.origin = vec2_new(0, 0),
		.scale = vec2_new(1, 1)
	};
}

Sprite spr_new_animated(Animation anim, Vector2 pos) {
	return (Sprite) {
		.src = {.anim = anim},
		.dynamic = true,
		.position = pos,
		.origin = vec2_new(0, 0),
		.scale = vec2_new(1, 1)
	};
}

TextureRegion spr_image(Sprite spr) {
	if(spr.dynamic) {
		return anim_get_current(spr.src.anim);
	} else {
		return spr.src.tex;
	}
}

void spr_step(Sprite *spr) {
	if(spr->dynamic) {
		anim_step(&(spr->src.anim));
	}
}

void spr_center_origin(Sprite *spr) {
	TextureRegion region = spr_image(*spr);
	spr->origin = vec2_new(region.region.width / 2, region.region.height / 2);
}
Texture texture_new(Window window, char *texture_path) {
	SDL_Texture *tex = NULL;
	SDL_Surface *loaded = IMG_Load(texture_path);
	if(loaded == NULL) {
		goto err;
	} else {
		tex = SDL_CreateTextureFromSurface(window.rend, loaded);
		if(tex == NULL) {
			goto err;
		}
		SDL_FreeSurface(loaded);
	}
	Texture texture;
	texture.texture = tex;
	SDL_QueryTexture(tex, NULL, NULL, &(texture.width), &(texture.height));
	return texture;
	err:
	fprintf(stderr, "Unable to load image %s. \n\tSDL Error: %s\n\tSDL_Image Error: %s", texture_path, SDL_GetError(), IMG_GetError());
	exit(-1);
}

void texture_destroy(Texture tex) {
	SDL_DestroyTexture(tex.texture);
}

TextureRegion texregion_new(Texture source) {
	return (TextureRegion) { source, rect_new(0, 0, source.width, source.height) };
}

TextureRegion texregion_new_sized(Texture source, Rect region) {
	return (TextureRegion) { source, region };
}

TextureRegion texregion_slice(TextureRegion region, Rect subregion) {
	Rect sub = rect_new(region.region.x + subregion.x, region.region.y + subregion.y, 
			subregion.width, subregion.height);
	return (TextureRegion) { region.source, sub };
}

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

void window_draw(Window window, Camera *cam, Sprite sprite) {
	TextureRegion region = spr_image(sprite);
	SDL_RendererFlip flip = SDL_FLIP_NONE | (SDL_FLIP_HORIZONTAL & sprite.flip_x) | (SDL_FLIP_VERTICAL & sprite.flip_y);
	SDL_Point point = { (int) sprite.origin.x, (int) sprite.origin.y };
	SDL_Rect src = rect_conv(region.region);
	Rect game_dest = rect_new(sprite.position.x, sprite.position.y, src.w * sprite.scale.x, src.h * sprite.scale.y);
	SDL_Rect dest;
	if(cam) {
		dest = cam_project_rect(*cam, game_dest);
	} else {
		dest = (SDL_Rect) { (int) sprite.position.x, (int) sprite.position.y, src.w * sprite.scale.x, src.h * sprite.scale.y };
	}
	SDL_RenderCopyEx(window.rend, region.source.texture, &src, &dest, sprite.angle, &point, flip);
}

void window_destroy(Window window) {
	SDL_DestroyRenderer(window.rend);
	SDL_DestroyWindow(window.window);
	SDL_Quit();
	IMG_Quit();
}
