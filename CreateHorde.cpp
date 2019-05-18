#include "stdafx.h"
#include "ZombieArena.h"
#include "Zombie.h"

Zombie* createHorde(int numZombies, IntRect arena) {
	Zombie* zombies = new Zombie[numZombies];

	int maxY = arena.height - 20;
	int minY = arena.top + 20;
	int maxX = arena.width - 20;
	int minX = arena.left + 20;

	for (int i = 0; i < numZombies; i++) {
		// Choose which side to spawn the zombie
		srand((int)time(0) * i);
		int side = rand() % 4;

		float x, y;

		switch (side){
		case 0:
			// Left
			x = minX;
			y = (rand() % maxY) + minY;
			break;

		case 1:
			// Right
			x = maxX;
			y = (rand() % maxY) + minY;
			break;

		case 2:
			// Top
			y = minY;
			x = (rand() % maxX) + minX;
			break;

		case 3:
			// Bottom
			y = maxY;
			x = (rand() % maxX) + minX;
			break;
		}
		// Choose type of zombie
		srand((int)time(0) * i * 2);
		int type = rand() % 3;

		zombies[i].spawn(x, y, type, i);
	}
	return zombies;
}