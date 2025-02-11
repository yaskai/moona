#pragma once

#include <sys/types.h>
#include "raylib.h"

// The spritesheet struct stores an array of rectangles.
// To draw a sprite, the spritesheet is given an index
// and draws the rectanglar section of the image 
// corresponding to the index

typedef struct {
	u_int8_t cols, rows;
	u_int8_t frame_w, frame_h;
	u_int8_t frame_count;
	Rectangle *frame_rec;	 
	Texture2D texture;
} Spritesheet;

Spritesheet MakeSpritesheet(u_int8_t w, u_int8_t h, Texture2D texture);						// Returns spritesheet instance with all rects set
void SpritesheetClose(Spritesheet *spritesheet);											// Free allocated memory
u_int8_t FrameIndex(Spritesheet *spritesheet, u_int8_t c, u_int8_t r);						// Returns index of {column, row}
void DrawSprite(Spritesheet *spritesheet, Vector2 position, u_int8_t index, float alpha);	// Draw Sprite of given index

