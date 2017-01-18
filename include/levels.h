#pragma once

typedef struct {
	ArrayList objects, layers; //ArcadeObject, SpatialMap
} Level;

Level level_load(char *filename);
void level_destroy(Level level);
