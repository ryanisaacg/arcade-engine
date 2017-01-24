#include "camera.h"

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
