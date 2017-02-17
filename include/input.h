#pragma once

#include <stdbool.h>

#include "array_list.h"
#include "geom.h"

/**
 * \file input.h
 *
 * \brief Contains input states and structures for querying inputs
 */

///The state of the keyboard
typedef struct KeyboardState {
	///\private
	bool *keys;
} KeyboardState;

///The state of the mouse buttons
typedef struct MouseState {
	int x, y;
	bool left, right, middle, x1, x2, wheel_up, wheel_down;
} MouseState;

///The state of a traditional gamepad
/// 
///The API makes the (relatively sane) assumption that the controller has 2 sticks, a dpad, 2 bumper buttons, 2 triggers, and 4 face buttons. 
///In the event this is a bad assumption, all non-relevant fields will be set to 0 or false.
typedef struct GamepadState {
	SDL_GameController *controller;
	Vector2 left, right;
	int trigger_left, trigger_right;
	bool dpad_up, dpad_down, dpad_left, dpad_right, bumper_left, bumper_right, face_a, face_b, face_x, face_y, stick_left, stick_right, start;
} GamepadState;

/// An abstract input action that allows checking keyboard masks as one call
typedef struct InputAction {
	ArrayList keys; ///An ArrayList of ints that are the indices of the keys to check
	MouseState buttons; ///A MouseState where the buttons that are marked true are checked
} InputAction;

// *** KEYS ***
/// Instantiate a blank keyboard state
KeyboardState ks_new(); 
/// Checks if a key is pressed (use the SDL_SCANCODE constants)
bool ks_is_pressed(const KeyboardState state, const int constant);
/// Manually sets a key state
void ks_set_state(KeyboardState state, const int constant, const bool value);
/// Sets a keyboard state from another state
void ks_cpy(KeyboardState dest, const KeyboardState src);

// *** MOUSE STATE ***
/// Instantiate a blank mouse state
MouseState ms_new();
/// Get the mouse position as a vector
Vector2 ms_get_position(MouseState state);

// *** GAMEPAD STATE ***
/// Load and instantiate plugged in gamepads (returns an ArrayList of GamepadState s)
ArrayList gs_load_controllers();
/// Update the state of the game controller
void gs_update(GamepadState *pad);
/// Unbind the controller from the program
void gs_destroy(GamepadState *state);

// *** INPUT ACTIONS ***
/// Create an input action from a list of keys and a representative mouse state
InputAction ia_new(const ArrayList keys, const MouseState mask);
/// Checks an input action against given states
bool ia_active(const InputAction action, const KeyboardState keyboard, const MouseState mouse);
