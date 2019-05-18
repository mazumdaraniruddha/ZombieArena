#include "stdafx.h"
#include "zombie.h"
#include "textureholder.h"
#include <cstdlib>
#include <ctime>

using namespace std;

void Zombie::spawn(float startX, float startY, int type, int seed) {
	switch (type) {
	case 0:
		// Bloater
		mSprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));
		mSpeed = 40;
		mHealth = 5;
		break;

	case 1:
		// Chaser
		mSprite = Sprite(TextureHolder::GetTexture("graphics/chaser.png"));
		mSpeed = 70;
		mHealth = 1;
		break;

	case 2:
		// Crawler
		mSprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		mSpeed = 20;
		mHealth = 3;
		break;
	}
	// Vary the speed slightly
	srand((int)time(0) * seed);
	float modifier = (rand() % MAX_VARIANCE) + OFFSET;
	modifier /= 100;
	mSpeed *= modifier;

	mPosition.x = startX;
	mPosition.y = startY;

	mSprite.setOrigin(25, 25);

	mSprite.setPosition(mPosition);
}

bool Zombie::hit() {
	mHealth--;
	if (mHealth < 0) {
		mAlive = false;
		mSprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;
	}
	return false;
}

bool Zombie::isAlive() {
	return mAlive;
}

FloatRect Zombie::getPosition() {
	return mSprite.getGlobalBounds();
}

Sprite Zombie::getSprite() {
	return mSprite;
}

void Zombie::update(float elapsedTime, Vector2f playerLocation) {
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	// Move the zombie towards the player
	if (playerX > mPosition.x) {
		mPosition.x = mPosition.x + mSpeed * elapsedTime;
	}
	if (playerY > mPosition.y) {
		mPosition.y = mPosition.y + mSpeed * elapsedTime;
	}
	if (playerX < mPosition.x) {
		mPosition.x = mPosition.x - mSpeed * elapsedTime;
	}
	if (playerY < mPosition.y) {
		mPosition.y = mPosition.x - mSpeed * elapsedTime;
	}

	mSprite.setPosition(mPosition);

	// Face the zombie sprite in the player's direction
	float angle = (atan2(playerY - mPosition.y, playerX = mPosition.x) / 180) * 3.141;
	mSprite.setRotation(angle);
}
