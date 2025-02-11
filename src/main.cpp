#include <cstring>
#include "raylib.h"
#include "include/resource_dir.h"
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

	if(IsGamepadAvailable(0)) puts("Gamepad connected!\n");
	else puts("Gamepad not connected...\n");

	Camera2D cam = {
		.offset = {ww * 0.5f, wh * 0.5f},
		.target = {ww * 0.5f, wh * 0.5f},
		.rotation = 0.0f,
		.zoom = 1.0f
	};

	Tilemap tilemap;
	TilemapInit(&tilemap, &cam);
	TilemapLoad(&tilemap, "bla5.mlf");

	Player player;
	player.Init(&tilemap, NULL, &cam);
	
	while (!WindowShouldClose()) {
		// Update logic
		float delta = GetFrameTime() * 100;	
		
		player.Update(delta);

		// Draw
		BeginDrawing();
			ClearBackground(BLACK);

			BeginMode2D(cam);
				TilemapDraw(&tilemap);
				player.Draw();
			EndMode2D();

		EndDrawing();
	}

	TilemapClose(&tilemap);

	CloseWindow();
	return 0;
}

