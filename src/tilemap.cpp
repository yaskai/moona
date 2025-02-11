#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <sys/types.h>
#include <string>
#include "raylib.h"
#include "tilemap.hpp"

void TilemapInit(Tilemap *tilemap, Camera2D *cam) {
	tilemap->width = 128;
	tilemap->height = 32;
	tilemap->tile_count = tilemap->width * tilemap->height;	

	tilemap->map_data = (char*)malloc(sizeof(char) * tilemap->tile_count);
	tilemap->spr_idx = (u_int8_t*)malloc(sizeof(u_int8_t) * tilemap->tile_count);

	tilemap->cam = cam;

	tilemap->flags = 0;

	tilemap->frame_min = (Coords){0, 0};
	tilemap->frame_max = (Coords){tilemap->width, tilemap->height};
}

void TilemapUpdate(Tilemap *tilemap) {

}

void TilemapDraw(Tilemap *tilemap) {
	for(u_int16_t r = tilemap->frame_min.r; r < tilemap->frame_max.r; r++) {
		for(u_int16_t c = tilemap->frame_min.c; c < tilemap->frame_max.c; c++) {
			switch(FetchTile(tilemap, (Coords){c, r})) {
				case TILE_BLOCK:
					ColorTile(tilemap, (Coords){c, r}, WHITE);
					break;
				case TILE_DOOR:
					break;
			}
		}
	}
}

void TilemapClose(Tilemap *tilemap) {
	free(tilemap->map_data);
	free(tilemap->spr_idx);
}

void TilemapLoad(Tilemap *tilemap, std::string path) {
	std::ifstream file(path);

	if(file.is_open()) {
		std::string sw; 
		std::string sh;
		std::getline(file, sw);
		std::getline(file, sh);
		tilemap->width = std::stoi(sw);
		tilemap->height = std::stoi(sh);
		tilemap->tile_count = tilemap->width * tilemap->height;
		
		for(u_int8_t r = 0; r < tilemap->height; r++) {
			std::string line;	
			std::getline(file, line);
			std::cout << line << std::endl;

			for(u_int8_t c = 0; c < tilemap->width; c++) {
				int idx = CoordsToIndex(tilemap, (Coords){c, r});
				char ch = line[c];
				tilemap->map_data[idx] = ch;
			}
		}

		tilemap->frame_min = (Coords){0, 0};
		tilemap->frame_max = (Coords){tilemap->width, tilemap->height};

		file.close();
	} else std::cout << "ERROR: Could not load level file...";
}

u_int16_t CoordsToIndex(Tilemap *tilemap, Coords coords) {
	return coords.c + coords.r * tilemap->width;
}

Coords IndexToCoords(Tilemap *tilemap, u_int16_t index) {
	u_int16_t c = index % tilemap->width;
	u_int16_t r = index / tilemap->width;
	return (Coords){c, r};
}

char FetchTile(Tilemap *tilemap, Coords coords) {
	return tilemap->map_data[CoordsToIndex(tilemap, coords)];
}

Vector2 CoordsToVector(Tilemap *tilemap, Coords coords) {
	return (Vector2) {
		(float)coords.c * TILE_SIZE,
		(float)coords.r * TILE_SIZE 
	};
}

void ColorTile(Tilemap *tilemap, Coords coords, Color color) {
	DrawRectangleV(CoordsToVector(tilemap, coords), (Vector2){TILE_SIZE, TILE_SIZE}, color);
}

