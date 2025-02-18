#pragma once

#include <cstdint>
#include "enemy.hpp"
#include "raylib.h"
#include "tilemap.hpp"
#include "player.hpp"
#include "pickup.hpp"

typedef struct {
	Tilemap *tilemap;
	Player *player;
	Camera2D *cam;
	Pickup *pickups;
	Enemy *enemies;
} Handler;

void HandlerInit(Handler *handler, Tilemap *tilemap, Camera2D *cam, Player *player);
void HandlerUpdate(Handler *handler);
void HandlerDraw(Handler *handler);
void HandlerClose(Handler *handler);
void ResetLevel(Handler *handler);

void NewPickup(Handler *handler, Vector2 position);
void NewEnemy(Handler *handler, Vector2 position, uint8_t type);

