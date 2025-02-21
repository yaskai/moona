#include <cstdint>
#include "raylib.h"
#include "enemy.hpp"
#include "animation.hpp"
#include "spritesheet.hpp"

Enemy MakeEnemy(Vector2 position, uint8_t type) {
	return (Enemy) {
		.type = type,
		.HP = 3,
		.active = true,
		.dead = false,
		.position = position,
		.start_position = position,
	};
}

void EnemyUpdate(Enemy *enemy, float dt) {
	if(dt < 1.0f) enemy->anim.fps = enemy->animFPS * 0.1f;
	else enemy->anim.fps = enemy->animFPS;

	switch(enemy->type) {
		case 0: AlienUpdate(enemy);	 break;
		case 1: UfoUpdate(enemy);  	 break;
	}

	enemy->damage_timer -= dt;
}

void EnemyDraw(Enemy *enemy) {
	switch(enemy->type) {
		case 0: AlienDraw(enemy); break;
		case 1: UfoDraw(enemy);   break;
	}
}

void EnemyCollision(Enemy *enemy, Player *player) {
	if(CheckCollisionRecs(enemy->killbox, player->bounds) && player->velocity.y > 0) {
		// ENEMY KILL...
		//enemy->active = false;
		enemy->dead = true;	
		player->velocity.y = -10;
		player->boost_used = false;
		player->last_ground_y = enemy->position.y;
	} 
	
	if(CheckCollisionRecs(enemy->bounds, player->bounds) && player->velocity.y <= 0)
		player->TakeDamage();
}

void EnemyDamage(Enemy *enemy) {
	if(enemy->damage_timer <= 0) {
		enemy->damage_timer = 10;
		enemy->HP--;
		enemy->DAMAGE = true;
		if(enemy->HP <= 0) enemy->dead = true;
		EnemyUpdateSpritesheet(enemy);
	}
}

void EnemyUpdateSpritesheet(Enemy *enemy) {
	if(enemy->DAMAGE) enemy->ssPtr = enemy->damage_ss;	
	else enemy->ssPtr = enemy->ss;
	enemy->anim.ss = enemy->ssPtr;
}

void AlienUpdate(Enemy *enemy) {
	enemy->bounds = (Rectangle) {
		enemy->position.x + 30,
		enemy->position.y + 20,
		20.0f, 64.0f 
	};

	enemy->killbox = (Rectangle) {
		enemy->position.x + 20,
		enemy->position.y + 10,
		20.0f, 20.0f
	};

	if(!enemy->dead)PlayAnimation(&enemy->anim);
	else if(enemy->dead) {
		PlayAnimation(&enemy->death_anim);
		if(enemy->death_anim.is_done) enemy->active = false;
	}	
}

void AlienDraw(Enemy *enemy) {
	if(!enemy->dead) DrawAnimation(&enemy->anim, enemy->position, 0);
	else DrawAnimation(&enemy->death_anim, enemy->position, 0);
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

	//PlayAnimation(&enemy->anim);
	if(!enemy->dead)PlayAnimation(&enemy->anim);
	else if(enemy->dead) {
		PlayAnimation(&enemy->death_anim);
		if(enemy->death_anim.is_done) enemy->active = false;
	}	
}

void UfoDraw(Enemy *enemy) {
	if(!enemy->dead) DrawAnimation(&enemy->anim, enemy->position, 0);
	else DrawAnimation(&enemy->death_anim, enemy->position, 0);
}

