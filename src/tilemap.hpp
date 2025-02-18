#pragma once

#include <string>
#include <sys/types.h>
#include <stdint.h>
#include "raylib.h"
#include "spritesheet.hpp"

// Adjacency flags
// Used for autotiling calculation
#define TOP	0x01 
#define	LFT	0x02
#define	RGT	0x04
#define	BOT	0x08

#define TILE_SIZE 64

// Each char in the level file represents an object,
// ex. '0' = empty, '1' = block, etc...
// Objects with behaviours(player, enemies, etc)
// have their classes or structs/classes instantiated
// at load time, static objects are simply drawn in TilemapDraw.
enum TILE_CHARS : char {
	TILE_EMPTY 	 = '0',
	TILE_BLOCK	 = '1',
	TILE_PLAYER  = '2',
	TILE_ENEMY0  = '3',
	TILE_ENEMY1  = '4',
	TILE_DOOR	 = '5',
	TILE_PICKUP  = 'f'
};

typedef struct {
	uint16_t c;
	uint16_t r;
} Coords;

typedef struct {
	u_int8_t flags;
	uint16_t width, height;
	uint16_t tile_count;
	Coords frame_min;  // Top left visible tile
	Coords frame_max;  // Bottom right visible tile
	Camera2D *cam;
	char *map_data;  // Char array set by level file
	u_int8_t *spr_idx; // Sprite index array, 
	Rectangle *solids;
	Spritesheet *ss;
} Tilemap;

// Essential functions
void TilemapInit(Tilemap *tilemap, Camera2D *cam, Spritesheet *ss);
void TilemapUpdate(Tilemap *tilemap);	// Every frame before draw
void TilemapDraw(Tilemap *tilemap);		// Every frame after update
void TilemapClose(Tilemap *tilemap);	// Free allocated memory
void TilemapLoad(Tilemap *tilemap, std::string path);
void TilemapGenerate(Tilemap *tilemap);
void TilemapUpdateSprites(Tilemap *tilemap);

// Math, sorting, indexing, etc 
uint16_t CoordsToIndex(Tilemap *tilemap, Coords coords);
Coords IndexToCoords(Tilemap *tilemap, uint16_t index);
char FetchTile(Tilemap *tilemap, Coords coords);	// Returns char value of tile at given coordinates
Vector2 CoordsToVector(Tilemap *tilemap, Coords coords);
Coords VectorToCoords(Tilemap *tilemap, Vector2 vector);
bool HasCollider(Tilemap *tilemap, Vector2 point);
Rectangle GetCollider(Tilemap *tilemap, Coords coords);
uint8_t TileGetAdj(Tilemap *tilemap, Coords coords);

// Drawing functions
void ColorTile(Tilemap *tilemap, Coords coords, Color color);	// Paint a tile at (coords) with given color

