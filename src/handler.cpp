#include <cstdint>
#include <raylib.h>
#include <raymath.h>
#include "handler.hpp"
#include "animation.hpp"
#include "enemy.hpp"
#include "pickup.hpp"
#include "player.hpp"
#include "spritesheet.hpp"

Spritesheet pickup_ss;
Animation pickup_anim;

Spritesheet alien_ss;
Spritesheet alien_damage_ss;
Spritesheet alien_death_ss;

Spritesheet ufo_ss;
Spritesheet ufo_damage_ss;
Spritesheet ufo_death_ss;

uint8_t pickup_total = 0;
uint8_t enemy_total = 0;

Rectangle cam_bounds;

void HandlerInit(Handler *handler, Tilemap *tilemap, Camera2D *cam, Player *player) {
	handler->tilemap = tilemap;
	handler->player = player;
	handler->cam = cam;

	pickup_ss = MakeSpritesheet(96, 96, LoadTexture("flower_sheet.png"));
	pickup_anim = MakeAnimation(12, 0, true, 10, &pickup_ss);

	alien_ss = MakeSpritesheet(64, 84, LoadTexture("alien_sheet00.png"));
	alien_damage_ss = MakeDamageSheet(&alien_ss, LoadImage("alien_sheet00.png"), WHITE);
	alien_death_ss = MakeSpritesheet(100, 100, LoadTexture("alien_death.png"));

	ufo_ss = MakeSpritesheet(144, 108, LoadTexture("UFO_sheet.png"));
	ufo_damage_ss = MakeDamageSheet(&ufo_ss, LoadImage("UFO_sheet.png"), WHITE);
	ufo_death_ss = MakeSpritesheet(96, 96, LoadTexture("UFO_death.png"));
}

void HandlerUpdate(Handler *handler, float dt) {
	cam_bounds = (Rectangle) {
		handler->cam->target.x - handler->cam->offset.x,
		handler->cam->target.y - handler->cam->offset.y,
		handler->cam->offset.x * 2,
		handler->cam->offset.y * 2
	};
	
	if(dt >= 1.0f) pickup_anim.fps = 12;
	else pickup_anim.fps = 12 * handler->player->time_mod;
	PlayAnimation(&pickup_anim);
	
	for(uint8_t i = 0; i < pickup_total; i++) {
		bool UPDATE = true;

		if(!handler->pickups[i].active) UPDATE = false;
		if(!CheckCollisionRecs(cam_bounds, handler->pickups[i].bounds)) UPDATE = false;

		if(UPDATE) {
			handler->pickups[i].anim->time_mod = handler->player->time_mod;

			if(CheckCollisionRecs(handler->player->bounds, handler->pickups[i].bounds)) {
				// PICKUP FLOWER...
				handler->pickups[i].active = false;	
				handler->player->HP++;
			}
		}
	}

	for(uint8_t i = 0; i < enemy_total; i++) {
		bool UPDATE = true;
		if(!handler->enemies[i].active) UPDATE = false;
		if(!CheckCollisionRecs(cam_bounds, handler->enemies[i].bounds)) UPDATE = false;

		if(UPDATE) { 
			handler->enemies[i].anim.time_mod = handler->player->time_mod;


			EnemyUpdate(&handler->enemies[i], dt);
			if(!handler->enemies[i].dead) {
				if(handler->enemies[i].flags & ENEMY_WALK) EnemyWalk(&handler->enemies[i], handler->tilemap);

				if(handler->player->player_state == PLAYER_BOOST && handler->player->boost_amount <= handler->player->boost_init_amount - 1) {
					
					Vector2 enemy_center = {
						handler->enemies[i].bounds.x + (handler->enemies[i].bounds.width * 0.5f),
						handler->enemies[i].bounds.y + (handler->enemies[i].bounds.height * 0.5f)
					};
					
					if(CheckCollisionCircleLine(enemy_center,
					    handler->enemies[i].bounds.height,
						handler->player->milk_start,

						handler->player->milk_end)) {
						EnemyDamage(&handler->enemies[i]);
					}
				}


				EnemyCollision(&handler->enemies[i], handler->player);
				
				if(handler->enemies[i].DAMAGE && handler->enemies[i].damage_timer <= 0) {
					handler->enemies[i].DAMAGE = false;
					EnemyUpdateSpritesheet(&handler->enemies[i]);
				}

			}
		}
	}
}

void HandlerDraw(Handler *handler) {
	for(uint8_t i = 0; i < pickup_total; i++) {
		bool DRAW = true;
		if(!handler->pickups[i].active) DRAW = false;
		
		if(DRAW) DrawAnimation(&pickup_anim, handler->pickups[i].position, 0);	
	}


	for(uint8_t i = 0; i < enemy_total; i++) {
		bool DRAW = true;
		if(!handler->enemies[i].active) DRAW = false;

		if(DRAW) {
			EnemyDraw(&handler->enemies[i]);
			
			/*
			Vector2 enemy_center = {
				handler->enemies[i].bounds.x + (handler->enemies[i].bounds.width * 0.5f),
				handler->enemies[i].bounds.y + (handler->enemies[i].bounds.height * 0.5f)
			};

			DrawCircleLinesV(enemy_center, handler->enemies->bounds.height * 0.75f, RED);
			*/
		}

	}

	//DrawRectangleRec(cam_bounds, ColorAlpha(BLUE, 0.25f));
}

void HandlerClose(Handler *handler) {
	free(handler->pickups);
	SpritesheetClose(&pickup_ss);
	CloseAnimation(&pickup_anim);

	for(uint8_t i = 0; i < enemy_total; i++) CloseAnimation(&handler->enemies[i].anim);
	free(handler->enemies);

	SpritesheetClose(&alien_ss);
	SpritesheetClose(&alien_damage_ss);
	SpritesheetClose(&alien_death_ss);

	SpritesheetClose(&ufo_ss);
	SpritesheetClose(&ufo_damage_ss);
	SpritesheetClose(&ufo_death_ss);
}

void ResetLevel(Handler *handler) {
	for(uint8_t i = 0; i < pickup_total; i++)
		handler->pickups[i].active = true;

	for(uint8_t i = 0; i < enemy_total; i++) {
		handler->enemies[i].active = true;
		handler->enemies[i].dead = false;
		handler->enemies[i].position = handler->enemies[i].start_position;
		if(handler->enemies[i].type == 0) handler->enemies[i].HP = 1; else handler->enemies[i].HP = 2;
		ResetAnimation(&handler->enemies[i].death_anim);
		handler->enemies[i].DAMAGE = false;
		EnemyUpdateSpritesheet(&handler->enemies[i]);
	}
}

void NewPickup(Handler *handler, Vector2 position) {
	Pickup pickup = MakePickup(position, &pickup_ss, &pickup_anim);
	handler->pickups[pickup_total] = pickup;
	pickup_total++;
}

void NewEnemy(Handler *handler, Vector2 position, uint8_t type) {
	Enemy enemy = MakeEnemy(position, type);
	
	if(type == 0) {
		enemy.ss = &alien_ss;
		enemy.anim = MakeAnimation(23, 0, true, 10, &alien_ss);
		enemy.position.y -= 20;
		enemy.start_position.y -= 20;
		enemy.death_anim = MakeAnimation(15, 0, false, 20, &alien_death_ss);
		enemy.HP = 2;
		enemy.damage_ss = &alien_damage_ss;
		enemy.flags = (ENEMY_WALK);
	} else if(type == 1) {
		enemy.ss = &ufo_ss;
		enemy.anim = MakeAnimation(14, 0, true, 20, &ufo_ss);
		enemy.death_anim = MakeAnimation(4, 0, false, 2, &ufo_death_ss);
		enemy.HP = 2;
		enemy.damage_ss = &ufo_damage_ss;
		enemy.flags = (ENEMY_STATIC);
	}

	if(enemy.flags & ENEMY_WALK) {
		enemy.velocity.x = -4;
	}

	enemy.animFPS = enemy.anim.fps;
	enemy.DAMAGE = false;

	handler->enemies[enemy_total] = enemy;
	enemy_total++;
}

