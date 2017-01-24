#pragma once

#include "arcobj.h"
#include "lines.h"
#include "rect.h"
struct Window;

struct Camera {
	Rect game_area;
	struct Window *window;
	int follow_index;
};

Camera cam_new(struct Window *window, Rect viewport);
SDL_Point cam_project_point(Camera cam, Vector2 point);
Vector2 cam_unproject_point(Camera cam, SDL_Point screen);
SDL_Rect cam_project_rect(Camera cam, Rect game_area);
Rect cam_unproject_rect(Camera cam, SDL_Rect screen);
void cam_set_follow(Camera *cam, ArcadeObject *obj);
void cam_clear_follow(Camera *cam);
