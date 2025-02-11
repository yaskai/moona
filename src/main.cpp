#include "raylib.h"
#include <cstring>
#include "include/resource_dir.h"
#include "tilemap.hpp"

int main () {
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	int ww = 1920;
	int wh = 1080;
	int fps = 100;

	InitWindow(ww, wh, "Raylib Project");
	SetTargetFPS(60);

	SearchAndSetResourceDir("resources");

	Camera2D cam = {
		.offset = {ww * 0.5f, wh * 0.5f},
		.target = {ww * 0.5f, wh * 0.5f},
		.rotation = 0.0f,
		.zoom = 1.0f
	};

	Tilemap tilemap;
	TilemapInit(&tilemap, &cam);
	TilemapLoad(&tilemap, "bla5.mlf");
	
	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(BLACK);

			BeginMode2D(cam);
				TilemapDraw(&tilemap);
			EndMode2D();


		EndDrawing();
	}

	TilemapClose(&tilemap);

	CloseWindow();
	return 0;
}

