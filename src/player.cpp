#include "raylib.h"
#include "tilemap.hpp"
#include <cstdio>
#include "player.hpp"

float gravity = 0.3f;

void Player::Init(Tilemap *tilemap, Spritesheet *ss, Camera2D *cam) {
	_tilemap = tilemap;
	_ss = ss;
	_cam = cam;

	position = (Vector2){0.0f, 0.0f};
	velocity = (Vector2){0.0f, 0.0f};
}

void Player::Update(float delta) {
	position.x += velocity.x * delta;
	position.y += velocity.y * delta;

	//velocity.y += gravity * delta;

	TakeInput();
}

void Player::Draw() {
	DrawRectangleV(position, (Vector2){TILE_SIZE, TILE_SIZE}, YELLOW);
}

void Player::Collision() {

}

void Player::TakeInput() {
	if(IsGamepadAvailable(0)) InputGP();
	else InputKB();
}

void Player::StartJump() {

}

void Player::StartFall() {

}

void Player::InputKB() {

}

void Player::InputGP()  { 
	float leftStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
	float leftStickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

	if(leftStickX < 0) {
		velocity.x -= 5;
	} else if (leftStickX > 0) {
		velocity.x += 5;
	}
}

