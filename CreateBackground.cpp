#include "stdafx.h"
#include "ZombieArena.h"

int createBackground(VertexArray& rVA, IntRect arena) {
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE;

	rVA.setPrimitiveType(Quads);

	rVA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

	int currentVertex = 0;

	for (int w = 0; w < worldWidth; w++) {
		for (int h = 0; h < worldHeight; h++) {
			// Initialise the vector coordinates
			rVA[currentVertex + 0].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 1].position = Vector2f((w + 1) * TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 2].position = Vector2f((w + 1) * TILE_SIZE, (h + 1) * TILE_SIZE);
			rVA[currentVertex + 3].position = Vector2f(w * TILE_SIZE, (h + 1) * TILE_SIZE);
			// Set the texture positions for walls/edges
			if (w == 0 || h == 0 || w == worldWidth - 1 || h == worldHeight - 1) {
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_SIZE * TILE_TYPES);
			} else {
				srand((int)time(0) + h * w - h);
				int mudOrGrass = rand() % TILE_TYPES;
				int verticalOffset = mudOrGrass * TILE_SIZE;

				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffset);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffset);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset);
			}

			// Move current Vertex to Next position
			currentVertex += VERTS_IN_QUAD;
		}
	}

	return TILE_SIZE;
}