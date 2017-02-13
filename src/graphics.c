#include "graphics.h"

#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Camera cam_new(Window *window, Rect viewport, bool letterbox) {
	return (Camera) {
		.game_area = viewport,
		.window = window,
		.letterbox = letterbox,
		.offset_x = 0,
		.offset_y = 0,
		.scale = vec2_new(1, 1)
	};
}

void cam_update(Camera *cam) {
	cam->scale = vec2_new(cam->window->width / cam->game_area.width, cam->window->height / cam->game_area.height);
	if(cam->letterbox) {
		float aspect_ratio = cam->game_area.width / cam->game_area.height;
		float expected_width = cam->window->width * aspect_ratio; //calculate the correct width for the aspect ratio
		if(expected_width < cam->window->width) {
			//handle vertical letterboxing (bars on the top and bottom)
			float difference = cam->window->width - expected_width;
			cam->offset_x = (int)(difference / 2);
			cam->scale = vec2_new(expected_width, cam->window->height / cam->game_area.height);
		} else if(expected_width > cam->window->width) {
			//handle horizontal letterboxing (bars on the sides)
			float expected_height = cam->window->width / aspect_ratio;
			float difference = cam->window->height - expected_height;
			cam->offset_y = (int)(difference / 2);
			cam->scale = vec2_new(cam->window->width / cam->game_area.width, expected_height);
		}
	}
}

SDL_Point cam_project_point(Camera cam, Vector2 point) {
	return (SDL_Point) { 
		.x = (int)(point.x * cam.scale.x) + cam.offset_x,
		.y = (int)(point.y * cam.scale.y) + cam.offset_y
	};
}

Vector2 cam_unproject_point(Camera cam, SDL_Point screen) {
	return (Vector2) {
		.x = screen.x / cam.scale.x - cam.offset_x,
		.y = screen.y / cam.scale.y - cam.offset_y
	};
}

SDL_Rect cam_project_rect(Camera cam, Rect area) {
	return (SDL_Rect) {
		.x = (int)(area.x * cam.scale.x) + cam.offset_x,
		.y = (int)(area.y * cam.scale.y) + cam.offset_y,
		.w = (int)(area.width * cam.scale.x),
		.h = (int)(area.height * cam.scale.y)
	};
}

Rect cam_unproject_rect(Camera cam, SDL_Rect screen) {
	return (Rect) {
		.x = screen.x / cam.scale.x - cam.offset_x,
		.y = screen.y / cam.scale.y - cam.offset_y,
		.width = screen.w / cam.scale.x,
		.height = screen.h / cam.scale.y
	};
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
}
