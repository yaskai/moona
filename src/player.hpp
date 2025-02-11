#pragma once

#include <sys/types.h>
#include "raylib.h"
#include "spritesheet.hpp"
#include "tilemap.hpp"

enum PLAYER_STATE : u_int8_t {
	PLAYER_IDLE,
	PLAYER_RUN,
	PLAYER_JUMP,
	PLAYER_FALL,
	PLAYER_LAND,
	PLAYER_BOOST
};

class Player {
public:
	u_int8_t HP;
	enum PLAYER_STATE player_state;
	bool on_ground;
	Vector2 position, velocity;
	Camera2D *_cam;
	Spritesheet *_ss;
	Tilemap *_tilemap;
	
	void Init(Tilemap *tilemap, Spritesheet *ss, Camera2D *cam);
	void Update();	// Called once every frame
	void Draw();	// Called once every frame, after Update()
private:
	void Collision();
	void TakeInput();	 
	void StartJump();
	void StartFall();
	void InputKB();		// Keyboard input
	void InputGP();		// Gamepad input
};

