/*
 * Pack textures together into a single atlas
 * Uses a JSON format that maps texture names to x, y, width, and height on the image
 * Uses a simple algorithm to pack the textures:
 * 	- Take the largest texture from the list
 * 	- Try to place it in the bottom-left
 * 	- Move to the spot closest to the bottom-left, expanding the total area if necessary
 * 	- Repeat until the list of textures is empty
 */

#include "arcade.h"
