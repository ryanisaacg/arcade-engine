#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define SDL_NUM_KEYS 284

KeyboardState ks_new() {
	return (KeyboardState) {
		.keys = calloc(SDL_NUM_KEYS, sizeof(bool))
	};
}

bool ks_is_pressed(const KeyboardState state, const int constant) {
	return state.keys[constant];
}

void ks_set_state(KeyboardState state, const int constant, const bool value) {
	state.keys[constant] = value;
}

void ks_cpy(KeyboardState dest, const KeyboardState src) {
	memcpy(dest.keys, src.keys, sizeof(bool) * SDL_NUM_KEYS);
}

MouseState ms_new() {
	return (MouseState) { 0 };
}

Vector2 ms_get_position(MouseState state) {
	return vec2_new(state.x, state.y);
}

ArrayList gs_load_controllers() {
	int joystick_num = SDL_NumJoysticks();
	if(joystick_num < 0) {
		fprintf(stderr, "Failed to get the number of joysticks.\n\tSDL Error: %s\n", SDL_GetError());
	}
	ArrayList controllers = al_new(sizeof(GamepadState));
	for(int i = 0; i < joystick_num; i++) {
		if(SDL_IsGameController(i)) {
			SDL_GameController *controller = SDL_GameControllerOpen(i);
			if(controller == NULL) {
				fprintf(stderr, "Failed to load a controller.\n\tSDL Error: %s\n", SDL_GetError());
			}
			GamepadState state = (GamepadState) {
				.controller = controller,
				{ 0 }
			};
			al_add(&controllers, &state);
		}
	}
	return controllers;
}

void gs_update(GamepadState *pad) {
	pad->left.x = SDL_GameControllerGetAxis(pad->controller, SDL_CONTROLLER_AXIS_LEFTX);
	pad->left.y = SDL_GameControllerGetAxis(pad->controller, SDL_CONTROLLER_AXIS_LEFTX);
	pad->right.x = SDL_GameControllerGetAxis(pad->controller, SDL_CONTROLLER_AXIS_RIGHTX);
	pad->right.y = SDL_GameControllerGetAxis(pad->controller, SDL_CONTROLLER_AXIS_RIGHTX);
	pad->trigger_left = SDL_GameControllerGetAxis(pad->controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	pad->trigger_right = SDL_GameControllerGetAxis(pad->controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	pad->dpad_up = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_DPAD_UP);
	pad->dpad_right = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	pad->dpad_down = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	pad->dpad_left = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	pad->face_a = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_A);
	pad->face_b = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_B);
	pad->face_x = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_X);
	pad->face_y = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_Y);
	pad->bumper_left = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	pad->bumper_right = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	pad->stick_left = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_LEFTSTICK);
	pad->stick_right = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
	pad->start = SDL_GameControllerGetButton(pad->controller, SDL_CONTROLLER_BUTTON_START);
}

void gs_destroy(GamepadState *state) {
	SDL_GameControllerClose(state->controller);
}

InputAction ia_new(ArrayList keys, MouseState mask) {
	return (InputAction) {
		.keys = keys,
		.buttons = mask
	};
}

bool ia_active(const InputAction action, KeyboardState keyboard, MouseState mouse) {
	for(size_t i = 0; i < action.keys.length; i++) {
		int *index = al_get(action.keys, i);
		if(!ks_is_pressed(keyboard, *index))
			return false;
	}
	return	(mouse.left && action.buttons.left) || 
			(mouse.right && action.buttons.right) ||
			(mouse.middle && action.buttons.middle) ||
			(mouse.x1 && action.buttons.x1) ||
			(mouse.x2 && action.buttons.x2) ||
			(mouse.wheel_up && action.buttons.wheel_up) ||
			(mouse.wheel_down && action.buttons.wheel_down);
}
