#pragma once

#include <cstdint>
#include "raylib.h"

// The spritesheet struct stores an array of rectangles.
// To draw a sprite, the spritesheet is given an index
// and draws the rectanglar section of the image 
// corresponding to the index

// Draw flags...
#define FLIP_X 0x01
#define FLIP_Y 0x02

typedef struct {
	uint8_t cols, rows;
	uint8_t frame_w, frame_h;
	uint8_t frame_count;
	Rectangle *frame_rec;	 
	Texture2D texture;
} Spritesheet;

Spritesheet MakeSpritesheet(uint8_t w, uint8_t h, Texture2D texture);						// Returns spritesheet instance with all rects set
void SpritesheetClose(Spritesheet *spritesheet);											// Free allocated memory
uint8_t FrameIndex(Spritesheet *spritesheet, uint8_t c, uint8_t r);						 	// Returns index of {column, row}
void DrawSprite(Spritesheet *spritesheet, Vector2 position, uint8_t index, uint8_t flags);	// Draw Sprite of given index
Spritesheet MakeDamageSheet(Spritesheet *source, Image img, Color color);

