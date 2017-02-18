#pragma once

#include "hashmap.h"
#include "graphics.h"
#include "sound.h"

/**
 * \file multimedia.h
 *
 * \brief General multimedia utilities
 */

///A struct to store loaded textures to allow simple memory management and disable texture duplication
typedef struct AssetManager {
	Window window; ///The window to use to load textures
	HashMap data; ///The hashmap to store the multimedia data in (Texture, Sound, and Music)
} AssetManager;

// *** INITIALIZATION
/// Initialize all dependency multimedia libraries
void multimedia_init();
/// Deinitialize all dependency multimedia libraries
void multimedia_quit();



// *** ASSETS ***
///Create a new asset manager using that window
AssetManager asset_new(Window window);
///Load or retrieve a raw Texture from a file
Texture asset_load_texture(AssetManager assets, char *path);
///Load or retrieve a Music track from a file
Music asset_load_music(AssetManager assets, char *path);
///Load or retrieve a Sound sample from a file
Sound asset_load_sound(AssetManager assets, char *path);
///Destroy the manager and the loaded textures
void asset_destroy(AssetManager assets);
