/* Deccan Game Engine - C11 2D SDL Game Engine.
 * Copyright 2020 Ayush Bardhan Tripathy
 *
 * This software is licensed under MIT License.
 * See LICENSE.md included with this package for more info.
 */

#include "String.h"
#include "Utils.h"

char* DE_StringNew(const char* source) {
	size_t len = strlen(source);
	char* string = DE_Alloc(sizeof(char), len + 1);
	
	strncpy(string, source, len);
	string[len] = '\0';
	return string;
}