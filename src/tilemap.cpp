#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <sys/types.h>
#include <string>
#include "enemy.hpp"
#include "raylib.h"
#include "spritesheet.hpp"
#include "tilemap.hpp"
#include "pickup.hpp"
#include "handler.hpp"

Handler *tmHandler;
void TilemapSetHandler(Handler *handler) { tmHandler = handler; }

uint8_t pickup_count = 0;
uint8_t enemy_count = 0;

void TilemapInit(Tilemap *tilemap, Camera2D *cam, Spritesheet *ss) {
	tilemap->width = 128;
	tilemap->height = 32;
	tilemap->tile_count = tilemap->width * tilemap->height;	

	tilemap->map_data = (char*)malloc(sizeof(char) * tilemap->tile_count);
	tilemap->spr_idx = (uint8_t*)malloc(sizeof(uint8_t) * tilemap->tile_count);
	//tilemap->solids = (Rectangle*)malloc(sizeof(Rectangle) * tilemap->tile_count);

	tilemap->cam = cam;

	tilemap->flags = 0;

	tilemap->frame_min = (Coords){0, 0};
	tilemap->frame_max = (Coords){tilemap->width, tilemap->height};

	tilemap->ss = ss;
}

void TilemapUpdate(Tilemap *tilemap) {

}

void TilemapDraw(Tilemap *tilemap) {
	for(uint16_t r = tilemap->frame_min.r; r < tilemap->frame_max.r; r++) {
		for(uint16_t c = tilemap->frame_min.c; c < tilemap->frame_max.c; c++) {
			Vector2 draw_pos = CoordsToVector(tilemap, {c, r});
			uint16_t idx = CoordsToIndex(tilemap, {c, r});

			switch(FetchTile(tilemap, (Coords){c, r})) {
				case TILE_BLOCK:
					DrawSprite(tilemap->ss, draw_pos, tilemap->spr_idx[idx], 0);
					break;

				case TILE_DOOR:
					break;
				
			}
		}
	}
}

void TilemapClose(Tilemap *tilemap) {
	free(tilemap->map_data);
	free(tilemap->spr_idx);
}

void TilemapLoad(Tilemap *tilemap, std::string path) {
	std::ifstream file(path);

	if(file.is_open()) {
		std::string sw; 
		std::string sh;
		std::getline(file, sw);
		std::getline(file, sh);
		tilemap->width = std::stoi(sw);
		tilemap->height = std::stoi(sh);
		tilemap->tile_count = tilemap->width * tilemap->height;

		tilemap->map_data = (char*)realloc(tilemap->map_data, sizeof(char) * tilemap->tile_count); 
		tilemap->spr_idx = (uint8_t*)realloc(tilemap->spr_idx, sizeof(uint8_t) * tilemap->tile_count); 
		//tilemap->solids = (Rectangle*)realloc(tilemap->solids, sizeof(Rectangle) * tilemap->tile_count); 
		
		for(uint8_t r = 0; r < tilemap->height; r++) {
			std::string line;	
			std::getline(file, line);
			std::cout << line << std::endl;

			for(uint8_t c = 0; c < tilemap->width; c++) {
				int idx = CoordsToIndex(tilemap, (Coords){c, r});
				char ch = line[c];
				tilemap->map_data[idx] = ch;
			}
		}

		tilemap->frame_min = (Coords){0, 0};
		tilemap->frame_max = (Coords){tilemap->width, tilemap->height};

		TilemapUpdateSprites(tilemap);

		file.close();
	} else std::cout << "ERROR: Could not load level file...";
}

void TilemapGenerate(Tilemap *tilemap) {
	for(uint16_t i = 0; i < tilemap->tile_count; i++) {
		switch(tilemap->map_data[i]) {
			case TILE_PICKUP: pickup_count++; break;
			case TILE_ENEMY0: case TILE_ENEMY1: enemy_count++; break;
		}
	}

	tmHandler->pickups = (Pickup*)malloc(sizeof(Pickup) * pickup_count);
	tmHandler->enemies = (Enemy*)malloc(sizeof(Enemy) * enemy_count);

	for(uint16_t i = 0; i < tilemap->tile_count; i++) {
		uint16_t c = i % tilemap->width;
		uint16_t r = i / tilemap->width;

		if(tilemap->map_data[i] == TILE_PICKUP) NewPickup(tmHandler, CoordsToVector(tilemap, {c, r}));
		if(tilemap->map_data[i] == TILE_ENEMY0) NewEnemy(tmHandler, CoordsToVector(tilemap, {c, r}), 0);
		if(tilemap->map_data[i] == TILE_ENEMY1) NewEnemy(tmHandler, CoordsToVector(tilemap, {c, r}), 1);
	}
}

void TilemapUpdateSprites(Tilemap *tilemap) {
	for(uint16_t i = 0; i < tilemap->tile_count; i++) {
		uint16_t c = i % tilemap->width;
		uint16_t r = i / tilemap->width;
		tilemap->spr_idx[i] = TileGetAdj(tilemap, (Coords){c, r});
	}
}

uint16_t CoordsToIndex(Tilemap *tilemap, Coords coords) {
	return coords.c + coords.r * tilemap->width;
}

Coords IndexToCoords(Tilemap *tilemap, uint16_t index) {
	uint16_t c = index % tilemap->width;
	uint16_t r = index / tilemap->width;
	return (Coords){c, r};
}

char FetchTile(Tilemap *tilemap, Coords coords) {
	return tilemap->map_data[CoordsToIndex(tilemap, coords)];
}

Vector2 CoordsToVector(Tilemap *tilemap, Coords coords) {
	return (Vector2) {
		(float)coords.c * TILE_SIZE,
		(float)coords.r * TILE_SIZE 
	};
}

Coords VectorToCoords(Tilemap *tilemap, Vector2 vector) {
	Coords coords;
	coords.c = vector.x / TILE_SIZE;
	coords.r = vector.y / TILE_SIZE;
	return coords;
}

bool HasCollider(Tilemap *tilemap, Vector2 point) {
	if(FetchTile(tilemap, VectorToCoords(tilemap, point)) == '1') return true;
	else return false;
}

Rectangle GetCollider(Tilemap *tilemap, Coords coords) {
	return (Rectangle) {
		(float)coords.c * TILE_SIZE,
		(float)coords.r * TILE_SIZE,
		TILE_SIZE,
		TILE_SIZE
	};
}

uint8_t TileGetAdj(Tilemap *tilemap, Coords pos) {
	uint8_t adj = 0;

	if(FetchTile(tilemap, (Coords){pos.c, (uint8_t)(pos.r - 1)}) == '1' || pos.r - 1 < 0) 				 		adj |= TOP; 
	if(FetchTile(tilemap, (Coords){pos.c, (uint8_t)(pos.r + 1)}) == '1' || pos.r + 1 > tilemap->height - 1)     adj |= BOT; 
	if(FetchTile(tilemap, (Coords){(uint8_t)(pos.c - 1), pos.r}) == '1' || pos.c - 1 < 0) 				 		adj |= LFT; 
	if(FetchTile(tilemap, (Coords){(uint8_t)(pos.c + 1), pos.r}) == '1' || pos.c + 1 > tilemap->width - 1)      adj |= RGT;

	// TODO:
	// Add, corner checks... 
	/*
	if(FetchTile(tilemap, (Coords){pos.c - 1, pos.r - 1}) == '1') adj |= T_L;
	if(FetchTile(tilemap, (Coords){pos.c + 1, pos.r - 1}) == '1') adj |= T_R;
	if(FetchTile(tilemap, (Coords){pos.c - 1, pos.r + 1}) == '1') adj |= B_L;
	if(FetchTile(tilemap, (Coords){pos.c + 1, pos.r + 1}) == '1') adj |= B_R;
	*/

	return adj;
}

void ColorTile(Tilemap *tilemap, Coords coords, Color color) {
	DrawRectangleV(CoordsToVector(tilemap, coords), (Vector2){TILE_SIZE, TILE_SIZE}, color);
}

