#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;
	// Player Position
	Vector2f mPosition;
	// Player Sprite
	Sprite mSprite;
	// Player Texture
	Texture mTexture;
	// Screen Resolution
	Vector2f mResolution;
	// Size of the Arena
	IntRect mArena;
	// Arena individual tile size
	int mTileSize;
	// Player movement information
	bool mUpPressed;
	bool mDownPressed;
	bool mLeftPressed;
	bool mRightPressed;
	// Player health
	int mHealth;
	// Max Health
	int mMaxHealth;
	// When was the player last hit
	Time mLastHit;
	// Player speed
	float mSpeed;

	// FUNCTION DEFINITIONS
public:
	Player();
	// Player spawn
	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	// Reset call at game end
	void resetPlayerStats();
	// Handle player hit from zombie
	bool hit(Time timeHit);
	// How long ago was the player hit
	Time getLastHitTime();
	// player position
	FloatRect getPosition();
	// Get center of the player
	Vector2f getCenter();
	// Which angle is player facing
	float getRotation();
	// Get player Sprite copy
	Sprite getSprite();
	// Movement functions
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();
	// Frame update function
	void update(float elapsedTime, Vector2i mousePosition);
	// Speed boost
	void upgradeSpeed();
	// Health boost
	void upgradeHealth();
	// Increase health limit
	void increaseHealthLevel(int amount);
	// Get health
	int getHealth();
};