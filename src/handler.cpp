#include <cstdint>
#include <raylib.h>
#include "handler.hpp"
#include "animation.hpp"
#include "enemy.hpp"
#include "pickup.hpp"
#include "spritesheet.hpp"

Spritesheet pickup_ss;
Animation pickup_anim;

Spritesheet alien_ss;
Spritesheet ufo_ss;

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
	ufo_ss = MakeSpritesheet(144, 108, LoadTexture("UFO_sheet.png"));
}

void HandlerUpdate(Handler *handler) {
	cam_bounds = (Rectangle) {
		handler->cam->target.x - handler->cam->offset.x,
		handler->cam->target.y - handler->cam->offset.y,
		handler->cam->offset.x * 2,
		handler->cam->offset.y * 2
	};

	PlayAnimation(&pickup_anim);
	
	for(uint8_t i = 0; i < pickup_total; i++) {
		bool UPDATE = true;

		if(!handler->pickups[i].active) UPDATE = false;
		if(!CheckCollisionRecs(cam_bounds, handler->pickups[i].bounds)) UPDATE = false;

		if(UPDATE) {
			if(CheckCollisionRecs(handler->player->bounds, handler->pickups[i].bounds))
				handler->pickups[i].active = false;	
		}
	}

	for(uint8_t i = 0; i < enemy_total; i++) {
		bool UPDATE = true;
		if(!handler->enemies[i].active) UPDATE = false;
		//if(!CheckCollisionRecs(cam_bounds, handler->enemies[i].bounds)) UPDATE = false;

		if(UPDATE) { 
			EnemyUpdate(&handler->enemies[i]);
			EnemyCollision(&handler->enemies[i], handler->player);
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

		if(DRAW) EnemyDraw(&handler->enemies[i]);
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
	SpritesheetClose(&ufo_ss);
}

void ResetLevel(Handler *handler) {
	for(uint8_t i = 0; i < pickup_total; i++) {
		handler->pickups[i].active = true;
	}

	for(uint8_t i = 0; i < enemy_total; i++) {
		handler->enemies[i].active = true;
		handler->enemies[i].position = handler->enemies[i].start_position;
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
		enemy.anim = MakeAnimation(23, 0, true, 24, &alien_ss);
		enemy.position.y -= 20;
		enemy.start_position.y -= 20;
	} else if(type == 1) {
		enemy.ss = &ufo_ss;
		enemy.anim = MakeAnimation(14, 0, true, 24, &ufo_ss);
	}

	handler->enemies[enemy_total] = enemy;
	enemy_total++;
}

