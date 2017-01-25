#pragma once

#include "array_list.h"

#include "manager.h"
#include "level.h"
#include "window.h"
#include "world.h"


typedef struct {
	AssetManager assets;
	World current;
	Window *window;
	Level *levels;
} Game;

Game game_new(WindowConfig config, char **level_names, size_t *indices, size_t num_levels);
/*
 * Takes control of execution and sets global data
 */
void game_start(Game game);
/*
 * Uses global state
 */
void game_stop();
/*
 * Sets global state
 */
void game_restart();