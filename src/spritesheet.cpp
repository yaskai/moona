#include <stdlib.h>
#include <sys/types.h>
#include "raylib.h"
#include "spritesheet.hpp"

Spritesheet MakeSpritesheet(u_int8_t w, u_int8_t h, Texture2D texture) {
	Spritesheet spritesheet;

	spritesheet.texture = texture;
	
	spritesheet.frame_w = w;
	spritesheet.frame_h = h;

	spritesheet.cols = texture.width / w;
	spritesheet.rows = texture.height / h;
	spritesheet.frame_count = spritesheet.cols * spritesheet.rows;
	
	spritesheet.frame_rec = (Rectangle*)malloc(sizeof(Rectangle) * spritesheet.frame_count);

	for(u_int8_t i = 0; i < spritesheet.frame_count; i++) {
		u_int8_t c = i % spritesheet.cols;
		u_int8_t r = i / spritesheet.cols;

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

u_int8_t FrameIndex(Spritesheet *spritesheet, u_int8_t c, u_int8_t r) {
	return c + r * spritesheet->cols;
}

void DrawSprite(Spritesheet *spritesheet, Vector2 position, u_int8_t index, float alpha) {
	DrawTextureRec(spritesheet->texture, spritesheet->frame_rec[index], position, ColorAlpha(WHITE, alpha));
}

