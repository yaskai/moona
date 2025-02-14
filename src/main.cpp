#include <cstring>
#include "animation.hpp"
#include "raylib.h"
#include "include/resource_dir.h"
#include "spritesheet.hpp"
#include "tilemap.hpp"
#include "player.hpp"

int main () {
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	int ww = 1920;
	int wh = 1080;
	int fps = 100;

	InitWindow(ww, wh, "Raylib Project");
	SetTargetFPS(fps);

	SearchAndSetResourceDir("resources");

	if(IsGamepadAvailable(0)) puts("Gamepad connected!");
	else puts("Gamepad not connected...");

	Spritesheet tile_ss = MakeSpritesheet(64, 64, LoadTexture("tileset00.png"));

	Camera2D cam = {
		.offset = {ww * 0.5f, wh * 0.5f},
		.target = {ww * 0.5f, wh * 0.5f},
		.rotation = 0.0f,
		.zoom = 1.0f
	};

	Tilemap tilemap;
	TilemapInit(&tilemap, &cam, &tile_ss);
	TilemapLoad(&tilemap, "bla5.mlf");
	TilemapGenerate(&tilemap);

	Player player;
	player.Init(&tilemap, NULL, &cam);

	Spritesheet butter_ss = MakeSpritesheet(128, 64, LoadTexture("buttah_sheet01.png"));
	Animation butter_anim = MakeAnimation(butter_ss.cols - 1, 0, true, 30, &butter_ss);
	
	while (!WindowShouldClose()) {
		// Update logic
		float delta = GetFrameTime() * 100;	
		
		player.Update(delta);
		PlayAnimation(&butter_anim);

		// Draw
		BeginDrawing();
			ClearBackground(BLACK);

			BeginMode2D(cam);
				TilemapDraw(&tilemap);
				player.Draw();
			EndMode2D();

			DrawAnimation(&butter_anim, {100, 100});

		EndDrawing();
	}

	TilemapClose(&tilemap);
	SpritesheetClose(&tile_ss);
	SpritesheetClose(&butter_ss);
	CloseAnimation(&butter_anim);

	CloseWindow();
	return 0;
}

