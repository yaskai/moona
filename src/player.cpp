#include "raylib.h"
#include "player.hpp"

void Player::Init(Tilemap *tilemap, Spritesheet *ss, Camera2D *cam) {
	_tilemap = tilemap;
	_ss = ss;
	_cam = cam;
}

void Player::Update() {

}

void Player::Draw() {

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

void Player::InputGP() {

}

