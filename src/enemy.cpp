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

void AlienUpdate(Enemy *enemy) {
	PlayAnimation(&enemy->anim);
}

void AlienDraw(Enemy *enemy) {
	DrawAnimation(&enemy->anim, enemy->position, 0);
}

void UfoUpdate(Enemy *enemy) {

}

void UfoDraw(Enemy *enemy) {

}

