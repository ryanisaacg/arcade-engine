#pragma once
/**
 * \file config.h
 * 
 * \brief Allows loading a custom INI-like configuration format designed for defining game entity types
 * 
 * A document contained a list of sectioned key-value pairs, where keys are strings and values may be scalars, 2d vectors, or 3d vectors.
 * 
 * A key may contain any character except the '=' symbol and a newline. A value is one, two, or three numeric values separated by commas. 'true' is 1, 'false' is 0. Trailing commas are acceptable.
 * 
 * The document takes the format:
 * 
 * [Section]
 * key1 = 1
 * key 2 = 2, 3.5
 * key three = true, false, -1
 */

#include "hashmap.h"
 
/// Represents a configuration document
typedef struct Document {
	///\private
	HashMap *items; //a map of char* to (a map of char* to (float* of length 3))
} Document;

/// Load a document from a file into memory
Document config_new(char *filepath);
/// Get all the sections of a file
ArrayList config_get_sections(Document doc);
/// Get all the keys of a section
ArrayList config_get_keys(Document doc, char *section);
/// Get the values of a key
float *config_get_value(Document doc, char *section, char *key);