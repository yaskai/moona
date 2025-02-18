#include "raylib.h"
#include "pickup.hpp"

Pickup MakePickup(Vector2 position, Spritesheet *ss, Animation *anim) {
	return (Pickup) {
		.active = true,
		.position = position,
		.bounds = (Rectangle){position.x + 12, position.y + 12, 80, 80},
		.ss = ss,
		.anim = anim
	};
}

