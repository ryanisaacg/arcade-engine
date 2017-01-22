#pragma once

#include "manager.h"
#include "window.h"
#include "world.h"

#include "array_list.h"

typedef struct {
	AssetManager assets;
	World current;
	Window *window;
	ArrayList levels;
} Game;

Game game_new(WindowConfig config, char **level_names, size_t *indices, size_t num_levels);
/*
 * Takes control of execution and sets global data
 */
void game_run(Game game);
