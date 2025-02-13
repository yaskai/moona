#pragma once

#include <sys/types.h>
#include "raylib.h"
#include "spritesheet.hpp"
#include "tilemap.hpp" 

// Set axis deadzones
const float leftStickDeadzoneX = 0.1f;
const float leftStickDeadzoneY = 0.1f;
const float rightStickDeadzoneX = 0.1f;
const float rightStickDeadzoneY = 0.1f;

enum PLAYER_STATE : u_int8_t {
	PLAYER_IDLE,
	PLAYER_RUN,
	PLAYER_JUMP,
	PLAYER_FALL,
	PLAYER_LAND,
	PLAYER_BOOST
};

#define FALL_GRAV 0.35f;
#define JUMP_GRAV 0.28f;
#define CANC_GRAV 0.65f;

class Player {
public:
	u_int8_t HP;
	enum PLAYER_STATE player_state;
	bool on_ground;
	Vector2 position, velocity;
	Coords grid_pos;
	Rectangle bounds;
	Camera2D *_cam;
	Spritesheet *_ss;
	Tilemap *_tilemap;
	
	void Init(Tilemap *tilemap, Spritesheet *ss, Camera2D *cam);
	void Update(float delta);	// Called once every frame
	void Draw();	// Called once every frame, after Update()
	
	void MoveX(float amount);
	void MoveY(float amount);
private:
	void Collision();
	void TakeInput(float delta);	 
	void StartJump();
	void StartFall();
	void InputKB(float delta);		// Keyboard input
	void InputGP(float delta);		// Gamepad input
	void ManageTimers(float delta);
};

