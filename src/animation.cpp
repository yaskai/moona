#include <cstdint>
#include <cstdlib>
#include "raylib.h"
#include "animation.hpp"
#include "spritesheet.hpp"

Animation MakeAnimation(uint8_t frame_count, uint8_t start_frame, bool infinite, uint8_t fps, Spritesheet *ss) {
	Rectangle *frames = (Rectangle*)malloc(sizeof(Rectangle) * frame_count);
	for(uint8_t i = start_frame; i < frame_count; i++) frames[i] = ss->frame_rec[i];

	return (Animation) {
		.frame_count = frame_count,
		.start_frame = start_frame,
		.fps = fps,
		.infinite = infinite,
		.is_done = false,
		.last_frame_time = (float)GetTime(),
		.init_time = 0,
		.interval = (((frame_count + 1) * 100.0f) / fps) / 2,
		.acc = 0.0f,
		.time_mod = 1.0f,
		.ss = ss,
		.frames = frames,
	};
}

void PlayAnimation(Animation *anim) {
	float ms = GetTime() * 1000.0f;
	anim->acc += (GetFrameTime() * 1000) * anim->time_mod;

	if(!anim->is_playing) anim->is_playing = true;
	if(anim->current_frame == 0 && anim->init_time == 0) {
		//anim->init_time = (float)GetTime();
		anim->init_time = ms; 
		anim->is_done = false;
	}

	/*	
	if(GetTime() > anim->last_frame_time + (1.0f / anim->fps)) {
		anim->current_frame++;
		anim->last_frame_time = (float)GetTime();
	}
	*/
	/*
	if(GetTime() > anim->last_frame_time + ((float)anim->frame_count / anim->fps)) {
		anim->current_frame++;
		anim->last_frame_time = (float)GetTime();
	}
	*/

	if(anim->acc >= anim->interval) {
		anim->acc = 0.0f;	
		anim->current_frame++;
	}

	if(anim->current_frame > anim->frame_count) {
		anim->current_frame = 0;
		//if(!anim->infinite && anim->init_time != (float)GetTime()) anim->is_done = true; 
		if(!anim->infinite && anim->init_time != ms) anim->is_done = true; 
	}
	
}

void DrawAnimation(Animation *anim, Vector2 position, uint8_t flags) {
	DrawSprite(anim->ss, position, anim->current_frame, flags);
}

void CloseAnimation(Animation *anim) {
	free(anim->frames);
}

void ResetAnimation(Animation *anim) {
	anim->current_frame = 0;
	anim->init_time = 0;
	anim->is_done = false;
}

