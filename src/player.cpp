#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <stdint.h>
#include <cstdio>
#include "animation.hpp"
#include "raylib.h"
#include "raymath.h"
#include "spritesheet.hpp"
#include "tilemap.hpp"
#include "player.hpp"
#include "handler.hpp"

Handler *plHandler;
void PlayerSetHandler(Handler *handler) { plHandler = handler; }

float gravity = FALL_GRAV;
float jumpreq_timer = 0;
bool jump_requested = false;

float ground_timer = 0;

float decel_timer = 0;
bool DECEL = false;

float jump_release = false;
float jump_release_timer = 0;

Vector2 start_pos;
float last_ground_y;

bool boost_used = false;
float boost_amount = 0;
float boost_max = 240;
Vector2 boost_dir = {0, 0};

float boost_rotation;
Vector2 boost_facing;
Vector2 boost_dest_v;
Vector2 boost_pres_v;
float boost_dest_a;

Vector2 vel_min = {-5, -15};
Vector2 vel_max = {5, 8};

char dir = 'r';
uint8_t draw_flags = 0;

void Player::Init(Tilemap *tilemap, Spritesheet *ss, Camera2D *cam) {
	_tilemap = tilemap;
	_ss = ss;
	_cam = cam;

	position = (Vector2){64.0f, 0.0f};
	velocity = (Vector2){0.0f, 0.0f};

	on_ground = false;

	start_pos = position;
	last_ground_y = position.y;

	HP = 3;
	player_state = PLAYER_IDLE;

	run_anim = MakeAnimation(12, 1, true, 24, _ss);
	jump_anim = MakeAnimation(15, 8, true, 24, _ss);
}

void Player::Update(float delta) {
	bounds = (Rectangle){position.x, position.y, 64, 64};
	
	grid_pos = VectorToCoords(_tilemap, (Vector2){position.x, position.y});

	if(velocity.x < vel_min.x) velocity.x = vel_min.x;
	else if(velocity.x > vel_max.x) velocity.x = vel_max.x;
	if(velocity.y < vel_min.y) velocity.y = vel_min.y;
	else if(velocity.y > vel_max.y) velocity.y = vel_max.y;

	if(position.x < 0) position.x = 0; 

	if(velocity.x != 0) MoveX(velocity.x * delta);
	if(velocity.y != 0) MoveY(velocity.y * delta);
	Collision();
	if(!on_ground) velocity.y += gravity * delta;

	if(player_state == PLAYER_JUMP) {
		if(velocity.y > -0.5f && velocity.y < 0.5f) StartFall();
	}

	if(player_state == PLAYER_BOOST) {
		gravity = 0.075f;
		vel_max = {5, 5};
		vel_min = Vector2Scale(vel_max, -1);

		//velocity = Vector2Add(velocity, Vector2Scale(boost_dir, 0.75f));
		velocity = Vector2Add(velocity, Vector2Scale(boost_facing, 0.75f));
		
		decel_timer = 20;
		DECEL = false;
		
		if(boost_amount <= 0 || on_ground) {
			StartFall();
		}

		boost_amount -= delta;
	} else {
		vel_min = {-5, -15};
		vel_max = {5, 8};
	}

	TakeInput(delta);
	ManageTimers(delta);
}

void Player::Draw() {
	Vector2 draw_pos = {position.x, position.y - 20};
	if(dir == 'l') draw_flags = (FLIP_X);
	else if(dir == 'r') draw_flags = 0;

	switch(player_state) {
		case PLAYER_IDLE:
			DrawSprite(_ss, draw_pos, 0, draw_flags);
			break;

		case PLAYER_RUN:
			PlayAnimation(&run_anim);
			DrawAnimation(&run_anim, draw_pos, draw_flags);
			break;

		case PLAYER_JUMP:
			PlayAnimation(&jump_anim);
			DrawAnimation(&jump_anim, draw_pos, draw_flags);
			break;

		case PLAYER_FALL:
			DrawSprite(_ss, draw_pos, 17, draw_flags);
			break;

		case PLAYER_LAND:
			break;

		case PLAYER_BOOST:
			draw_flags = 0;
			//if(boost_dir.x > 0) draw_flags |= FLIP_X;
			//if(boost_dir.y > 0) draw_flags |= FLIP_Y;
			if(boost_facing.x > 0) draw_flags |= FLIP_X;
			if(boost_facing.y > 0) draw_flags |= FLIP_Y;
			
			Vector2 milk_start;
			milk_start = {draw_pos.x + 32, draw_pos.y + 32};

			Vector2 milk_end;
			//milk_end = Vector2Add(milk_start, Vector2Scale(boost_dir, -2000));
			milk_end = Vector2Add(milk_start, Vector2Scale(boost_facing, -5000));

			DrawLineEx(milk_start, milk_end, 20, RAYWHITE);

			DrawSprite(_ss, draw_pos, 15, draw_flags);
			DrawCircleLinesV(milk_start, 128, WHITE);
			//DrawCircleV(Vector2Add(milk_start, Vector2Scale(Vector2Rotate({0, -1}, boost_rotation * DEG2RAD), 128)), 16, WHITE);
			//DrawCircleV(boost_dest_v, 16, BLUE);

			float sc;
			if(boost_dest_v.y == 0 || boost_dest_v.y == 0) sc = 128;
			else sc = 90;
			if(boost_dest_v.y == 1 && boost_dest_v.x == 0) sc = 138;
			if(boost_dest_v.y == -1 && boost_dest_v.x == 0) sc = 138;
			DrawCircleV(Vector2Add(milk_start, Vector2Scale(boost_dest_v, sc)), 16, WHITE);
			break;

		case PLAYER_CHARGE:
			DrawSprite(_ss, draw_pos, 25, draw_flags);
			DrawCircleLinesV(Vector2Add(position, {32, 32}), 128, WHITE);
			//DrawCircleV(Vector2Add(milk_start, Vector2Scale(Vector2Rotate({0, -1}, boost_rotation * DEG2RAD), 128)), 16, WHITE);
			//DrawCircleV(boost_dest_v, 16, BLUE);

			float scb;
			if(boost_dest_v.y == 0 || boost_dest_v.y == 0) scb = 128;
			else scb = 90;
			if(boost_dest_v.y == 1 && boost_dest_v.x == 0) scb = 138;
			if(boost_dest_v.y == -1 && boost_dest_v.x == 0) scb = 138;
			DrawCircleV(Vector2Add(Vector2Add(position, {32, 32}), Vector2Scale(boost_dest_v, scb)), 16, WHITE);
			break;
	}
}

void Player::Close() {
	CloseAnimation(&run_anim);
}

void Player::MoveX(float amount) {
	uint8_t passes = 4;
	float small_amount = amount / passes;
	float remain = 0;
	float next_x = position.x + amount;
	if(amount > 0) next_x += 64;

	for(uint8_t i = 0; i < passes; i++) {
		bool coll = false;

		for(int n = 8; n < 58; n += 4) {
			if(HasCollider(_tilemap, {next_x, position.y + n})) {
				coll = true;
				break;
			}
		}

		if(!coll) remain += small_amount;
	}

	position.x += remain;
}

void Player::MoveY(float amount) {
	uint8_t passes = 4;
	float small_amount = amount / passes;
	float remain = 0;
	float next_y = position.y;
	if(amount > 0) next_y += 64;

	for(uint8_t i = 0; i < passes; i++) {
		bool coll = false;

		for(int n = 8; n < 60; n += 4) {
			if(HasCollider(_tilemap, {position.x + n, next_y})) {
				coll = true;
				break;
			} 
		}

		if(!coll) remain += small_amount;
		else if(coll) {
			if(amount > 0) { 
				on_ground = true;
				//uint16_t grid_down = grid_pos.r + 1;
				position.y = GetCollider(_tilemap, grid_pos).y;
				gravity = 0;
			}

			remain = 0;
			velocity.y = 0;
		}
	}

	position.y += remain;
}

void Player::Collision() {
	if(on_ground) {
		// Check if player should fall
		if(!(HasCollider(_tilemap, (Vector2){position.x, position.y + 65}) ||
		     HasCollider(_tilemap, (Vector2){position.x + 32, position.y + 65}) ||
		     HasCollider(_tilemap, (Vector2){position.x + 62, position.y + 65}))) {
			StartFall();
		} else {
			last_ground_y = position.y;	
			if((int)position.y % 64 != 0) position.y = CoordsToVector(_tilemap, grid_pos).y - 1; 
			//velocity.y = 0;
			//boost_amount = 100;
			boost_amount = 0;
			boost_used = false;
			if(player_state == PLAYER_BOOST) player_state = PLAYER_IDLE;
		}
	}

	if(position.y > (_tilemap->height + 12) * TILE_SIZE) Die();
}

void Player::TakeInput(float delta) {
	if(IsGamepadAvailable(0)) InputGP(delta);
	else InputKB(delta);
}

void Player::StartJump() {
	on_ground = false;
	velocity.y = -11;
	player_state = PLAYER_JUMP;
	gravity = JUMP_GRAV;
	jumpreq_timer = 0;
	jump_anim.current_frame = 0;
}

void Player::StartFall() {
	on_ground = false;
	player_state = PLAYER_FALL;
	gravity = FALL_GRAV;
}

void Player::InputKB(float delta) {
	// *KEYBOARD*
	
	if(player_state != PLAYER_BOOST) { 
		// HORIZONTAL MOVEMENT
		if(boost_amount <= 1) {
			if(IsKeyDown(KEY_A)) {
				velocity.x -= delta;
				dir = 'l';
			}

			if(IsKeyDown(KEY_D)) {
				velocity.x += delta;
				dir = 'r';
			}
		}

		if(IsKeyUp(KEY_A) && IsKeyUp(KEY_D)) {
			//velocity.x *= 0.9f;
			DECEL = true;
			if(on_ground) player_state = PLAYER_IDLE;
		} else {
			DECEL = false;
			if(on_ground) player_state = PLAYER_RUN;
		}

		// BOOST
		if(!on_ground && !boost_used) {
			if(IsKeyDown(KEY_ENTER)) {
				boost_amount += 2 * delta;
				if(velocity.y > 0.5f) velocity.y = 0.5f;
				if(velocity.x > 1) velocity.x = 1;
				else if(velocity.x < -1) velocity.x = -1;
				player_state = PLAYER_CHARGE;
				/*
				if(IsKeyDown(KEY_A)) {
					//boost_facing = {-1, 0};
				} else if(IsKeyDown(KEY_D)) {
					//boost_facing = {1, 0};
					//boost_rotation = (2 * PI);
				} else if(IsKeyDown(KEY_W)) {
					//boost_facing = {0, -1};
				} else if(IsKeyDown(KEY_S)) {
					//boost_facing = {0, 1};
				}
				*/
			}

			if(IsKeyReleased(KEY_ENTER) || boost_amount >= boost_max) {
				if(boost_amount <= 1) { 
					boost_amount = 0;
					boost_used = false;
				} else {
					//boost_dir = Vector2Zero();
					//boost_rotation = 0.0f;
					//boost_dest_v = {0, -1};
					//boost_pres_v = {0, -1};
					if(IsKeyDown(KEY_A)) boost_pres_v.x = -1;
					if(IsKeyDown(KEY_D)) boost_pres_v.x = 1;
					if(IsKeyDown(KEY_W)) boost_pres_v.y = -1;
					if(IsKeyDown(KEY_S)) boost_pres_v.y = 1;

					player_state = PLAYER_BOOST;
					boost_used = true;
				}
			}
		}
	}
	
	// JUMP
	if(player_state == PLAYER_JUMP) {
		if(IsKeyUp(KEY_SPACE)) gravity = CANC_GRAV;
	} else {
		if(ground_timer > 0 && IsKeyDown(KEY_SPACE)) StartJump();
		else if(ground_timer > 0 && jumpreq_timer > 0) StartJump();

		if(!on_ground && IsKeyPressed(KEY_SPACE)) jumpreq_timer = 10;
	}

	if(IsKeyDown(KEY_SPACE)) {
		jump_release = false;
		jump_release_timer = 1;
	} else jump_release = true;
	
	if(player_state == PLAYER_CHARGE) {
		if(IsKeyDown(KEY_A)) {
			boost_dest_v.x = -1;
		} else if(IsKeyDown(KEY_D)) {
			boost_dest_v.x = 1;
		} else boost_dest_v.x = 0;

		if(IsKeyDown(KEY_W)) {
			boost_dest_v.y = -1;
		} else if (IsKeyDown(KEY_S)) {
			boost_dest_v.y = 1;
		} else boost_dest_v.y = 0;
	}

	if(player_state == PLAYER_BOOST) {
		//boost_facing = Vector2Rotate((Vector2){0, -1}, boost_rotation * DEG2RAD);
		//boost_facing = Vector2Rotate(boost_dest_v, boost_rotation * DEG2RAD);
		boost_facing = Vector2Rotate(boost_pres_v, boost_rotation * DEG2RAD);

		if(IsKeyDown(KEY_A)) {
			boost_dest_v.x = -1;
		} else if(IsKeyDown(KEY_D)) {
			boost_dest_v.x = 1;
		} else boost_dest_v.x = 0;

		if(IsKeyDown(KEY_W)) {
			boost_dest_v.y = -1;
		} else if (IsKeyDown(KEY_S)) {
			boost_dest_v.y = 1;
		} else boost_dest_v.y = 0;

		if(IsKeyUp(KEY_A) && IsKeyUp(KEY_D) && IsKeyUp(KEY_W) && IsKeyUp(KEY_S)) {
			boost_dest_v = boost_pres_v;
		}
		/*
		if(boost_dir.x < -1) boost_dir.x = -1;
		else if(boost_dir.x > 1) boost_dir.x = 1;
		if(boost_dir.y < -1) boost_dir.y = -1;
		else if(boost_dir.y > 1) boost_dir.y = 1;
		*/
		boost_pres_v = Vector2Lerp(boost_pres_v, boost_dest_v, delta * 0.015f);
	}

	// *KEYBOARD*
}

void Player::InputGP(float delta) {
	// *GAMEPAD*
	float stick_x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
	// *GAMEPAD*
}

void Player::ManageTimers(float delta) {
	if(DECEL) {
		if(decel_timer <= 0) {
			if(on_ground) {
				velocity.x *= 0.9f;
				decel_timer = 0.5f;
			} else { 
				velocity.x *= 0.95f;
				decel_timer = 1.0f;
			}
		}

		decel_timer -= delta;
	}

	jumpreq_timer -= delta;
	if(jumpreq_timer < 0) jump_requested = false;

	if(!on_ground) ground_timer -= delta;
	else ground_timer = 5;

	if(jump_release) jump_release_timer -= delta;
}

void Player::UpdateCam(uint8_t ww, uint8_t wh) {
	_cam->target.x = position.x + 64;

	if(position.y < (last_ground_y + (512)) && position.y >= last_ground_y - (512)) 
		_cam->target.y += ((last_ground_y - _cam->target.y)) * (GetFrameTime() * 5);
	else _cam->target.y += ((position.y) - _cam->target.y) * (GetFrameTime() * 10);

	if(_cam->target.x < _cam->offset.x) 
		_cam->target.x = _cam->offset.x;
	
	if(_cam->target.x > (_tilemap->width * TILE_SIZE) - _cam->offset.x) 
		_cam->target.x = (_tilemap->width * TILE_SIZE) - _cam->offset.x;
	
	if(_cam->target.y < _cam->offset.y) 
		_cam->target.y = _cam->offset.y;

	if(_cam->target.y > ((_tilemap->height + 1) * TILE_SIZE) - _cam->offset.y) 
		_cam->target.y = ((_tilemap->height + 1) * TILE_SIZE) - _cam->offset.y;
}

void Player::Die() {
	position = start_pos;
	velocity = Vector2Zero();
	player_state = PLAYER_IDLE;
	ResetLevel(plHandler);
}

