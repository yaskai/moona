#include <cstdint>
#include "raylib.h"
#include "enemy.hpp"
#include "animation.hpp"
#include "spritesheet.hpp"

Enemy MakeEnemy(Vector2 position, uint8_t type) {
	return (Enemy) {
		.type = type,
		.active = true,
		.position = position,
		.start_position = position,
	};
}

	void EnemyUpdate(Enemy *enemy) {
		switch(enemy->type) {
			case 0: AlienUpdate(enemy);	 break;
			case 1: UfoUpdate(enemy);  	 break;
		}
	}

void EnemyDraw(Enemy *enemy) {
	switch(enemy->type) {
		case 0: AlienDraw(enemy); break;
		case 1: UfoDraw(enemy);   break;
	}
}

void EnemyCollision(Enemy *enemy, Player *player) {
	if(CheckCollisionRecs(enemy->killbox, player->bounds) && player->velocity.y > 0) {
		enemy->active = false;
		player->velocity.y = -10;
	}
}

void AlienUpdate(Enemy *enemy) {
	PlayAnimation(&enemy->anim);
}

void AlienDraw(Enemy *enemy) {
	DrawAnimation(&enemy->anim, enemy->position, 0);
}

void UfoUpdate(Enemy *enemy) {
	enemy->bounds = (Rectangle) {
		enemy->position.x,
		enemy->position.y,
		(float)enemy->ss->frame_w, 
		(float)enemy->ss->frame_h
	};

	enemy->killbox = (Rectangle) {
		enemy->position.x,
		enemy->position.y - 10,
		(float)enemy->ss->frame_w,
		enemy->ss->frame_h * 0.5f
	};

	PlayAnimation(&enemy->anim);
}

void UfoDraw(Enemy *enemy) {
	DrawAnimation(&enemy->anim, enemy->position, 0);
}

