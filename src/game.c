#include "simulation.h"

bool keep_going;
Game game;

Game game_new(WindowConfig config, char **level_names, size_t *indices, size_t num_levels) {
	Game g = {
		.window = malloc(sizeof(Window)),

	};
	ArrayList levels = al_new_sized(sizeof(Level), num_levels);
}

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
