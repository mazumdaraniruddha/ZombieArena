#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Pickup {
private:
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;
	// Sprite for the pickup
	Sprite mSprite;
	// THe arena where this pickup drops
	IntRect mArena;
	// Value of the pickup
	int mValue;
	// Type of pickup
	int mType;
	// Whether spawned
	bool mSpawned;
	float mSecondsSinceSpawn;
	float mSecondsSinceDespawn;
	float mSecondsToLive;
	float mSecondsToWait;

public:
	Pickup(int type);
	// Pickup preps and setting the arena
	void setArena(IntRect arena);
	void spawn();
	FloatRect getPosition();
	Sprite getSprite();
	void update(float elapsedTime);
	bool isSpawned();
	int gotIt();
	void upgrade();
};