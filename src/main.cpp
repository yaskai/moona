#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "handler.hpp"
#include "raylib.h"
#include "include/resource_dir.h"
#include "spritesheet.hpp"
#include "tilemap.hpp"
#include "player.hpp"
#include "handler.hpp"
#include "audioPlayer.hpp"

#define WINDOW_A 0x01	
#define WINDOW_B 0x02

uint8_t WINDOW_FLAGS = (0);

void TilemapSetHandler(Handler *handler);
void PlayerSetHandler(Handler *handler);

int main () {
	//SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_FULLSCREEN_MODE);
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	
	int ww = 1920;
	int wh = 1080;
	int fps = 60;

	if(WINDOW_FLAGS & WINDOW_A) {
		ww = 1920;
		wh = 1080;
		fps = 100;
	} else if(WINDOW_FLAGS & WINDOW_B) {
		ww = GetMonitorWidth(0);	
		wh = GetMonitorHeight(0);
		fps = GetMonitorRefreshRate(0);
	}

	InitWindow(ww, wh, "Milk Time");
	SetTargetFPS(fps);
	HideCursor();

	SearchAndSetResourceDir("resources");

	AudioPlayer ap;
	AudioPlayerInit(&ap);

	if(IsGamepadAvailable(0)) puts("Gamepad connected!");
	else puts("Gamepad not connected...");

	Spritesheet tile_ss = MakeSpritesheet(64, 64, LoadTexture("tileset01.png"));
	Spritesheet player_ss = MakeSpritesheet(64, 84, LoadTexture("moona_sheet_00.png"));

	Camera2D cam = {
		.offset = {ww * 0.5f, wh * 0.5f},
		.target = {0, 0},
		.rotation = 0.0f,
		.zoom = 1.0f
	};

	Tilemap tilemap;
	TilemapInit(&tilemap, &cam, &tile_ss);
	TilemapLoad(&tilemap, "test.mlf");

	Player player;
	player.Init(&tilemap, &player_ss, &cam);

	Handler handler;
	HandlerInit(&handler, &tilemap, &cam, &player);

	handler.ap  = &ap;

	TilemapSetHandler(&handler);
	TilemapGenerate(&tilemap);

	PlayerSetHandler(&handler);
	// Music *music = ap->music;
	// PlayMusicStream(ap.music);
	while (!WindowShouldClose()) {
		// Update logic
		float delta = GetFrameTime() * 100;
		float delta1 = delta * player.time_mod;

		UpdateMusicStream(ap.music[0]);		

		player.Update(delta);
		HandlerUpdate(&handler, delta1);
		player.UpdateCam(ww, wh);

		// Draw
		BeginDrawing();
			ClearBackground(BLACK);

			BeginMode2D(cam);
				TilemapDraw(&tilemap);
				HandlerDraw(&handler);
				player.Draw();
			EndMode2D();

			player.DrawHealthBar();

		EndDrawing();
	}

	TilemapClose(&tilemap);
	SpritesheetClose(&tile_ss);
	SpritesheetClose(&player_ss);
	player.Close();
	AudioPlayerClose(&ap);

	CloseWindow();
	return 0;
}

