#pragma once

#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

/**
 * \file sound.h
 *
 * \brief Contains definitions for Music and Sound structs and their associated functions
 */

///A music track; only one track can be played at once
typedef struct Music {
	///\private
	Mix_Music *music;
} Music;
///A sound sample
typedef struct Sound {
	///\private
	Mix_Chunk *chunk;
	/**
	 * \brief The default volume for playing the sound sample from 0 to 128
	 *
	 * Can be overriden by playing the sound with the sound_play_volume function
	 */
	int volume;
} Sound;

// *** MUSIC ***
/// Load music from a file
Music music_new(const char *path);
/// Play a music track a certain number of times through
void music_play(const Music track, const int times);
/// Fade in a music track over a certain number of milliseconds a certain number of times through
void music_fade_in(const Music track, const int times, const int milliseconds);
/// Loop a music track indefinitely
void music_loop(const Music track);
/// Fade in a music track over a certain number of milliseconds then loop indefinitely
void music_fade_in_loop(const Music track, const int milliseconds);
/// Checks if a music track is currently playing
bool music_is_playing();
/// Check if a music track is paused
bool music_is_paused();
/// Check if a music track is fading in or out
bool music_is_fading();
/// Resume the music track if it is paused
void music_resume();
/// Pause the music track; only use when music is playing
void music_pause();
/// Stop the music track
void music_stop();
/// Rewind the music to the beginning
void music_rewind();
/// Fade out the current track and then stop over a given number of milliseconds
void music_fade_out(const int milliseconds);
/// Set a position in seconds for a poconst int in the music track
void music_set_position(const double seconds);
/**
 * \brief Call the given function when the music finishes
 *
 * DO NOT call music or sound functions in the callback
 */
void music_finished_callback(void (*const callback)());
/// Sets the volume of the music (from 0 to 128)
void music_set_volume(const int volume);
/// Deallocate a music track
void music_destroy(Music track);

// *** SOUND ***
/// Load a sound from a file
Sound sound_new(const char *path);
/// Play a sound at its stored volume
void sound_play(const Sound sound);
/// Play a sound at a specific volume from 0 to 128
void sound_play_volume(const Sound sound, const int volume);
/// Play a sound a given number of times at its stored volume
void sound_repeat(const Sound sound, const int loops);
/// Play a sound a given number of times at a specific volume from 0 to 128
void sound_repeat_volume(const Sound sound, const int loops, const int volume);
/// Deallocate a sound sample
void sound_destroy(Sound sound);
