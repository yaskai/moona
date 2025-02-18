#pragma once

#include <cstdint>
#include "raylib.h"
#include "spritesheet.hpp"

typedef struct {
	uint8_t frame_count;
	uint8_t start_frame;
	uint8_t current_frame;
	uint8_t fps;
	bool infinite;
	bool is_playing;
	bool is_done;
	float last_frame_time;
	float init_time;
	Spritesheet *ss;
	Rectangle *frames;
} Animation;

Animation MakeAnimation(uint8_t frame_count, uint8_t start_frame, bool infinite, uint8_t fps, Spritesheet *ss);
void PlayAnimation(Animation *anim);
void DrawAnimation(Animation *anim, Vector2 position, uint8_t flags);
void CloseAnimation(Animation *anim);
void ResetAnimation(Animation *anim);

