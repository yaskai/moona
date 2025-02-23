#pragma once

#include <cstdint>
#include "animation.hpp"
#include "raylib.h"
#include "spritesheet.hpp"
#include "tilemap.hpp" 

// Set axis deadzones
const float leftStickDeadzoneX = 0.1f;
const float leftStickDeadzoneY = 0.1f;
const float rightStickDeadzoneX = 0.1f;
const float rightStickDeadzoneY = 0.1f;

enum PLAYER_STATE : uint8_t {
	PLAYER_IDLE,
	PLAYER_RUN,
	PLAYER_JUMP,
	PLAYER_FALL,
	PLAYER_LAND,
	PLAYER_BOOST,
	PLAYER_CHARGE
};

#define FALL_GRAV 0.35f
#define JUMP_GRAV 0.28f
#define CANC_GRAV 0.65f

#define BOOST_ROT_SPEED 0.9f

class Player {
public:
	uint8_t HP;
	enum PLAYER_STATE player_state;
	bool on_ground;
	bool boost_used;
	float time_mod;
	float last_ground_y;
	float milk_rotation;
	float damage_timer;
	float boost_amount;
	float boost_init_amount;
	Vector2 milk_facing;
	Vector2 milk_start;
	Vector2 milk_end;
	Vector2 position, velocity;
	Vector2 start_pos;
	Vector2 prev_pos;
	Coords grid_pos;
	Rectangle bounds;
	Camera2D *_cam;
	Spritesheet *_ss;
	Tilemap *_tilemap;
		
	void Init(Tilemap *tilemap, Spritesheet *ss, Camera2D *cam);
	void Update(float delta);	// Called once every frame
	void Draw();	// Called once every frame, after Update()
	void Close(); 	// Free memory

	void MoveX(float amount);
	void MoveY(float amount);
	void UpdateCam(uint8_t ww, uint8_t wh);
	void Die();
	void TakeDamage();
	void DrawHealthBar();
private:
	void Collision();
	void TakeInput(float delta);	 
	void StartJump();
	void StartFall();
	void InputKB(float delta);		// Keyboard input
	void InputGP(float delta);		// Gamepad input
	void ManageTimers(float delta);

	Animation run_anim;
	Animation jump_anim;
};

