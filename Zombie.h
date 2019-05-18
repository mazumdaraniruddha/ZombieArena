#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Zombie {
private:
	// Zombie Type speeds
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;
	// Zombie health types
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;
	// Individual zombie speed variance
	static const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;
	// Zombie position
	Vector2f mPosition;
	// Zombie sprite
	Sprite mSprite;
	// Zombie speed
	int mSpeed;
	// Zombie health
	float mHealth;
	// Is zombie alive
	bool mAlive = true;

public:
	// Handle bullet hit on a zombie
	bool hit();
	// See if zombie is alive
	bool isAlive();
	// Spawn a new zombie
	void spawn(float startX, float startY, int type, int seed);
	// Return the zombie position as a rectangle in the world
	FloatRect getPosition();
	// Get the zombie sprite
	Sprite getSprite();
	// Update the zombie in the frame draw call
	void update(float elapsedTime, Vector2f playerLocation);
};