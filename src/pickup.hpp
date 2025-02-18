#pragma once 

#include <cstdint>
#include "raylib.h"
#include "spritesheet.hpp"
#include "animation.hpp"

typedef struct {
	bool active;
	Vector2 position;
	Rectangle bounds;
	Spritesheet *ss;
	Animation *anim;
} Pickup;

Pickup MakePickup(Vector2 position, Spritesheet *ss, Animation *anim); 

