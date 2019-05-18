#include "stdafx.h"
#include "Pickup.h"
#include "TextureHolder.h"

Pickup::Pickup(int type) {
	mType = type;
	if (mType == 1) {
		mSprite = Sprite(TextureHolder::GetTexture("graphics/health_pickup.png"));
		mValue = HEALTH_START_VALUE;
	} else {
		mSprite = Sprite(TextureHolder::GetTexture("graphics/ammo_pickup.png"));
		mValue = AMMO_START_VALUE;
	}
	mSprite.setOrigin(25, 25);
	mSecondsToLive = START_SECONDS_TO_LIVE;
	mSecondsToWait = START_WAIT_TIME;
}

void Pickup::setArena(IntRect arena) {
	mArena.left = arena.left + 50;
	mArena.width = arena.width - 50;
	mArena.top = arena.top + 50;
	mArena.height = arena.height - 50;
	spawn();
}

void Pickup::spawn() {
	srand((int)time(0) / mType);
	int x = rand() % mArena.width;
	srand((int)time(0) * mType);
	int y = rand() % mArena.height;

	mSecondsSinceSpawn = 0;
	mSpawned = true;

	mSprite.setPosition(x, y);
}

FloatRect Pickup::getPosition() {
	return mSprite.getGlobalBounds();
}

Sprite Pickup::getSprite() {
	return mSprite;
}

bool Pickup::isSpawned() {
	return mSpawned;
}

int Pickup::gotIt() {
	mSpawned = false;
	mSecondsSinceDespawn = 0;
	return mValue;
}

void Pickup::update(float elapsedTime) {
	if (mSpawned) {
		mSecondsSinceSpawn += elapsedTime;
	}
	else {
		mSecondsSinceDespawn += elapsedTime;
	}

	// Should hide the pickup?
	if (mSecondsSinceSpawn > mSecondsToLive && mSpawned) {
		mSpawned = false;
		mSecondsSinceDespawn = 0;
	}
	// Should spawn a pickup?
	if (mSecondsSinceDespawn > mSecondsToWait && !mSpawned) {
		spawn();
	}
}

void Pickup::upgrade() {
	if (mType == 1) {
		mValue += (HEALTH_START_VALUE * 0.5);
	} else {
		mValue += (AMMO_START_VALUE * 0.5);
	}

	mSecondsToLive += (START_SECONDS_TO_LIVE / 10);
	mSecondsToWait += (START_WAIT_TIME / 10);
}