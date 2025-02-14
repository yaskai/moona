#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <stdint.h>
#include <cstdio>
#include "raylib.h"
#include "tilemap.hpp"
#include "player.hpp"

float gravity = FALL_GRAV;
float jumpreq_timer = 0;
bool jump_requested = false;

float ground_timer = 0;

float jump_release = false;
float jump_release_timer = 0;

Vector2 start_pos;

void Player::Init(Tilemap *tilemap, Spritesheet *ss, Camera2D *cam) {
	_tilemap = tilemap;
	_ss = ss;
	_cam = cam;

	position = (Vector2){64.0f, 512.0f};
	velocity = (Vector2){0.0f, 0.0f};

	on_ground = false;

	start_pos = position;

	HP = 3;
}

void Player::Update(float delta) {
	bounds = (Rectangle){position.x, position.y, 64, 64};
	
	grid_pos = VectorToCoords(_tilemap, (Vector2){position.x, position.y});

	if(velocity.x < -5) velocity.x = -5;
	else if(velocity.x > 5) velocity.x = 5;
	if(velocity.y < -10) velocity.y = -10;
	else if(velocity.y > 8) velocity.y = 8;

	if(position.x < 0) position.x = 0; 

	MoveX(velocity.x * delta);
	MoveY(velocity.y * delta);
	Collision();
	if(!on_ground) velocity.y += gravity * delta;

	if(player_state == PLAYER_JUMP) {
		if(velocity.y > -0.5f && velocity.y < 0.5f) StartFall();
	}

	TakeInput(delta);
	ManageTimers(delta);
	
}

void Player::Draw() {
	DrawRectangleV(position, (Vector2){TILE_SIZE, TILE_SIZE}, YELLOW);
	/*
	for(uint16_t r = grid_pos.r - 1; r < grid_pos.r + 2; r++) {
		for(uint16_t c = grid_pos.c - 1; c < grid_pos.c + 2; c++) {
			DrawRectangleLinesEx(_tilemap->solids[CoordsToIndex(_tilemap, {c, r})], 10.0f, BLUE);
			//DrawText(TextFormat("%d, %d", c, r), c * 64, r * 64, 10, BLACK);
		}
	}
	*/
}

void Player::MoveX(float amount) {
	uint8_t passes = 4;
	float small_amount = amount / passes;
	float remain = 0;
	float next_x = position.x + amount;
	if(amount > 0) next_x += 64;

	for(uint8_t i = 0; i < passes; i++) {
		bool coll = false;

		for(int n = 2; n < 60; n += 4) {
			if(HasCollider(_tilemap, {next_x, position.y + n})) {
				coll = true;
				break;
			}
		}

		if(!coll) remain += small_amount;
	}

	position.x += remain * (GetFrameTime() * 100);
}

void Player::MoveY(float amount) {
	uint8_t passes = 4;
	float small_amount = amount / passes;
	float remain = 0;
	float next_y = position.y;
	if(amount > 0) next_y += 64;

	for(uint8_t i = 0; i < passes; i++) {
		bool coll = false;

		for(int n = 2; n < 60; n += 4) {
			if(HasCollider(_tilemap, {position.x + n, next_y})) {
				coll = true;
				break;
			} 
		}

		if(!coll) remain += small_amount;
		else if(coll) {
			if(amount > 0) { 
				on_ground = true;
				
				uint16_t grid_down = grid_pos.r + 1;
				//position.y = GetCollider(_tilemap, {grid_pos.c, grid_down}).y - 64;
				
				//position.y -= velocity.y;
				
				if(position.y + 64 > GetCollider(_tilemap, {grid_pos.c, grid_down}).y) {
					position.y = GetCollider(_tilemap, {grid_pos.c, grid_down}).y - 64;
				}
			}

			remain = 0;
			velocity.y = 0;
		}
	}

	position.y += remain * (GetFrameTime() * 100);
}

void Player::Collision() {
	if(on_ground) {
		if(!(HasCollider(_tilemap, (Vector2){position.x, position.y + 65}) ||
		     HasCollider(_tilemap, (Vector2){position.x + 32, position.y + 65}) ||
		     HasCollider(_tilemap, (Vector2){position.x + 62, position.y + 65}))) {
			StartFall();
		} else {
			if((int)position.y % 64 != 0) position.y = CoordsToVector(_tilemap, grid_pos).y; 
			velocity.y = 0;
		}
	}

	if(position.y > _tilemap->height * TILE_SIZE) {
		position = start_pos;
		velocity = {0, 0};
	}
}

void Player::TakeInput(float delta) {
	if(IsGamepadAvailable(0)) InputGP(delta);
	else InputKB(delta);
}

void Player::StartJump() {
	on_ground = false;
	velocity.y = -10;
	player_state = PLAYER_JUMP;
	gravity = JUMP_GRAV;
	jumpreq_timer = 0;
}

void Player::StartFall() {
	on_ground = false;
	player_state = PLAYER_FALL;
	gravity = FALL_GRAV;
}

void Player::InputKB(float delta) {
	// *KEYBOARD*
	
	// HORIZONTAL MOVEMENT
	if(IsKeyDown(KEY_A)) {
		velocity.x -= delta;
	}

	if(IsKeyDown(KEY_D)) {
		velocity.x += delta;
	}

	if(IsKeyUp(KEY_A) && IsKeyUp(KEY_D)) {
		velocity.x *= 0.9f;
	}
	
	// JUMP
	if(player_state == PLAYER_JUMP) {
		if(IsKeyUp(KEY_SPACE)) gravity = CANC_GRAV;
	} else {
		/*
		if(ground_timer > 0 && (IsKeyDown(KEY_SPACE) || jumpreq_timer > 0)) {
			//if(jump_release_timer <= 0) StartJump();
			StartJump();
		}
		*/
		if(ground_timer > 0 && IsKeyDown(KEY_SPACE)) {
			StartJump();
		} else if(ground_timer > 0 && jumpreq_timer > 0) {
			StartJump();
		}

		if(!on_ground && IsKeyPressed(KEY_SPACE)) jumpreq_timer = 10;
	}

	if(IsKeyDown(KEY_SPACE)) {
		jump_release = false;
		jump_release_timer = 1;
	} else {
		jump_release = true;
	}

	// BOOST
	if(IsKeyDown(KEY_ENTER)) {
		
	}

	// *KEYBOARD*
}

void Player::InputGP(float delta) {
	// *GAMEPAD*
	float stick_x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);

	if(stick_x < -0.1) {
		velocity.x -= delta;
	}

	if(stick_x > 0.1) {
		velocity.x += delta;
	}

	if(stick_x <= 01. && stick_x >= -0.1) {
		velocity.x *= 0.9f;
	}

	// BUTTON 7 = A
	if(player_state == PLAYER_JUMP) {
		if(IsGamepadButtonUp(0, 7)) gravity = CANC_GRAV;
	} else {
		/*
		if(ground_timer > 0 && (IsKeyDown(KEY_SPACE) || jumpreq_timer > 0)) {
			//if(jump_release_timer <= 0) StartJump();
			StartJump();
		}
		*/
		if(ground_timer > 0 && IsGamepadButtonDown(0, 7)) {
			StartJump();
		} else if(ground_timer > 0 && jumpreq_timer > 0) {
			StartJump();
		}

		if(!on_ground && IsGamepadButtonPressed(0, 7)) jumpreq_timer = 25;
	}
	
	if(IsGamepadButtonUp(0, 7)) {
		jump_release = false;
		jump_release_timer = 1;
	} else {
		jump_release = true;
	}

	// *GAMEPAD*
}

void Player::ManageTimers(float delta) {
	jumpreq_timer -= delta;
	if(jumpreq_timer < 0) jump_requested = false;

	if(!on_ground) ground_timer -= delta;
	else ground_timer = 5;

	if(jump_release) jump_release_timer -= delta;
}

