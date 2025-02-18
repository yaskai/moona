#pragma once

#include <cstdint>
#include "animation.hpp"
#include "raylib.h"
#include "spritesheet.hpp"

typedef struct {
	uint8_t type;
	bool active;
	Vector2 position;
	Vector2 start_position;
	Rectangle bounds;
	Animation anim;
	Spritesheet *ss;
} Enemy;

Enemy MakeEnemy(Vector2 position, uint8_t type);

void EnemyUpdate(Enemy *enemy);
void EnemyDraw(Enemy *enemy);

void AlienUpdate(Enemy *enemy);
void AlienDraw(Enemy *enemy);

void UfoUpdate(Enemy *enemy);
void UfoDraw(Enemy *enemy);

