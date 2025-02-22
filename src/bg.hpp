#pragma once

#include <cstdint>
#include "raylib.h"

typedef struct {
	uint8_t flags;
	float scale;
	float scroll_mod;
	Vector2 position;
	Texture2D texture;	
} BgLayer;

typedef struct {
				
} Background;
