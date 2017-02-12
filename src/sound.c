#include "sound.h"

#include <stdio.h>

// SOUND AND MUSIC
Music music_new(const char *path) {
	Mix_Music *msc = Mix_LoadMUS(path);
	if(msc == NULL) {
		fprintf(stderr, "Unable to load music file %s; SDL_Mixer error: %s\n", path, Mix_GetError());
	}
	return (Music) {
		.music = msc
	};
}

void music_play(Music track, int times) {
	Mix_PlayMusic(track.music, times);
}

void music_fade_in(Music track, int times, int milliseconds) {
	Mix_FadeInMusic(track.music, times, milliseconds);
}

void music_loop(Music track) {
	Mix_PlayMusic(track.music, -1);
}

void music_fade_in_loop(Music track, int milliseconds) {
	Mix_FadeInMusic(track.music, -1, milliseconds);
}

void music_resume() {
	Mix_ResumeMusic();
}

void music_pause() {
	Mix_PauseMusic();
}

void music_stop() {
	Mix_HaltMusic();
}

bool music_is_playing() {
	return Mix_PlayingMusic();
}

bool music_is_paused() {
	return Mix_PausedMusic();
}

bool music_is_fading() {
	return Mix_FadingMusic() != MIX_NO_FADING;
}

void music_rewind() {
	Mix_SetMusicPosition(0);
}

void music_fade_out(int milliseconds) {
	Mix_FadeOutMusic(milliseconds);
}

void music_set_position(double seconds) {
	Mix_SetMusicPosition(seconds);
}

void music_finished_callback(void (*callback)()) {
	Mix_HookMusicFinished(callback);
}

void music_set_volume(int volume) {
	Mix_VolumeMusic(volume);
}

void music_destroy(Music track) {
	Mix_FreeMusic(track.music);
}

Sound sound_new(const char *path) {
	Mix_Chunk *chunk = Mix_LoadWAV(path);
	if(chunk == NULL) {
		fprintf(stderr, "Unable to load sound file %s; SDL_mixer error: %s\n", path, Mix_GetError());
	}
	return (Sound) {
		.chunk = chunk,
		.volume = MIX_MAX_VOLUME
	};
}

static void play_channel(Mix_Chunk *chunk, int loops, int volume) {
	int channel = Mix_PlayChannel(-1, chunk, loops);
	if(channel == -1) {
		int num_channels = Mix_AllocateChannels(-1); //-1 gets channels
		Mix_AllocateChannels(num_channels * 2);
		channel = Mix_PlayChannel(-1, chunk, loops);
		if(channel == -1) {
			fprintf(stderr, "Error trying to play sound: %s\n", Mix_GetError());
		}
	}
	Mix_Volume(channel, volume);
}

void sound_play(Sound sound) {
	play_channel(sound.chunk, 0, sound.volume);
}

void sound_play_volume(Sound sound, int volume) {
	play_channel(sound.chunk, 0, volume);
}

void sound_repeat(Sound sound, int loops) {
	play_channel(sound.chunk, loops, sound.volume);
}

void sound_repeat_volume(Sound sound, int loops, int volume) {
	play_channel(sound.chunk, loops, volume);
}

void sound_destroy(Sound sound) {
	Mix_FreeChunk(sound.chunk);
}
