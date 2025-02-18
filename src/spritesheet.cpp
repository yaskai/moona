#include <stdlib.h>
#include <cstdint>
#include "raylib.h"
#include "spritesheet.hpp"

Spritesheet MakeSpritesheet(uint8_t w, uint8_t h, Texture2D texture) {
	Spritesheet spritesheet;

	spritesheet.texture = texture;
	spritesheet.frame_w = w;
	spritesheet.frame_h = h;

	spritesheet.cols = texture.width / w;
	spritesheet.rows = texture.height / h;
	spritesheet.frame_count = spritesheet.cols * spritesheet.rows;
	spritesheet.frame_rec = (Rectangle*)malloc(sizeof(Rectangle) * spritesheet.frame_count);

	for(uint8_t i = 0; i < spritesheet.frame_count; i++) {
		uint8_t c = i % spritesheet.cols;
		uint8_t r = i / spritesheet.cols;

		spritesheet.frame_rec[i] = (Rectangle) {
			(float)c * spritesheet.frame_w,
			(float)r * spritesheet.frame_h, 
			(float)spritesheet.frame_w,
			(float)spritesheet.frame_h
		};
	}
	
	return spritesheet;
}

void SpritesheetClose(Spritesheet *spritesheet) {
	free(spritesheet->frame_rec);
	UnloadTexture(spritesheet->texture);
}

uint8_t FrameIndex(Spritesheet *spritesheet, uint8_t c, uint8_t r) {
	return c + r * spritesheet->cols;
}

void DrawSprite(Spritesheet *spritesheet, Vector2 position, uint8_t index, uint8_t flags) {
	Rectangle rec = spritesheet->frame_rec[index];	
	if(flags & FLIP_X) rec.width  = -spritesheet->frame_w;
	if(flags & FLIP_Y) rec.height = -spritesheet->frame_h;
	DrawTextureRec(spritesheet->texture, rec, position, WHITE);
}

