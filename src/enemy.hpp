#pragma once

#include <cstdint>
#include "animation.hpp"
#include "raylib.h"
#include "spritesheet.hpp"
#include "player.hpp"
#include "tilemap.hpp"

#define ENEMY_STATIC 	0x01
#define ENEMY_WALK   	0x02
#define ENEMY_ORBIT  	0x04

typedef struct {
	uint8_t flags;
	uint8_t type;
	uint8_t animFPS;
	uint8_t HP;
	char dir;
	bool active;
	bool dead;
	bool DAMAGE;
	float damage_timer;
	Vector2 position;
	Vector2 start_position;
	Vector2 velocity;
	Rectangle bounds;
	Rectangle killbox;
	Animation anim;
	Animation death_anim;
	Spritesheet *ss;
	Spritesheet *damage_ss;
	Spritesheet *ssPtr;
} Enemy;

Enemy MakeEnemy(Vector2 position, uint8_t type);
void EnemyUpdate(Enemy *enemy, float dt);
void EnemyDraw(Enemy *enemy);
void EnemyCollision(Enemy *enemy, Player *player);
void EnemyDamage(Enemy *enemy);
void EnemyUpdateSpritesheet(Enemy *enemy);

void AlienUpdate(Enemy *enemy);
void AlienDraw(Enemy *enemy);

void UfoUpdate(Enemy *enemy);
void UfoDraw(Enemy *enemy);

void EnemyWalk(Enemy *enemy, Tilemap *tilemap);
void EnemyOrbit(Enemy *enemy);

