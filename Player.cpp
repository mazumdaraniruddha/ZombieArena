#include "stdafx.h"
#include "Player.h"
#include "TextureHolder.h"

Player::Player() {
	mSpeed = START_SPEED;
	mHealth = START_HEALTH;
	mMaxHealth = START_HEALTH;

	mSprite.setTexture(TextureHolder::GetTexture("graphics/player.png"));
	mSprite.setOrigin(25, 25);
}

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize) {
	mPosition.x = arena.width / 2;
	mPosition.y = arena.height / 2;

	mArena.left = arena.left;
	mArena.top = arena.top;
	mArena.height = arena.height;
	mArena.width = arena.width;

	mTileSize = tileSize;

	mResolution = resolution;
}

void Player::resetPlayerStats() {
	mSpeed = START_SPEED;
	mHealth = START_HEALTH;
	mMaxHealth = START_HEALTH;
}

Time Player::getLastHitTime() {
	return mLastHit;
}

bool Player::hit(Time timeHit) {
	if (timeHit.asMilliseconds() - mLastHit.asMilliseconds() > 200) {
		mLastHit = timeHit;
		mHealth -= 10;
		return true;
	}
	else {
		return false;
	}
}

FloatRect Player::getPosition() {
	return mSprite.getGlobalBounds();
}

Vector2f Player::getCenter() {
	return mPosition;
}

float Player::getRotation() {
	return mSprite.getRotation();
}

Sprite Player::getSprite() {
	return mSprite;
}

int Player::getHealth() {
	return mHealth;
}

void Player::moveLeft() {
	mLeftPressed = true;
}

void Player::moveRight() {
	mRightPressed = true;
}

void Player::moveUp() {
	mUpPressed = true;
}

void Player::moveDown() {
	mDownPressed = true;
}

void Player::stopLeft() {
	mLeftPressed = false;
}

void Player::stopRight() {
	mRightPressed = false;
}

void Player::stopUp() {
	mUpPressed = false;
}

void Player::stopDown() {
	mDownPressed = false;
}

void Player::update(float elapsedTime, Vector2i mousePosition) {
	if (mUpPressed) {
		mPosition.y -= mSpeed * elapsedTime;
	}
	if (mDownPressed) {
		mPosition.y += mSpeed * elapsedTime;
	}
	if (mRightPressed) {
		mPosition.x += mSpeed * elapsedTime;
	}
	if (mLeftPressed) {
		mPosition.x -= mSpeed * elapsedTime;
	}

	mSprite.setPosition(mPosition);

	// Keep Player in the arena
	if (mPosition.x > mArena.width - mTileSize) {
		mPosition.x = mArena.width - mTileSize;
	}
	if (mPosition.x < mArena.left + mTileSize) {
		mPosition.x = mArena.left + mTileSize;
	}
	if (mPosition.y > mArena.height - mTileSize) {
		mPosition.y = mArena.height - mTileSize;
	}
	if (mPosition.y < mArena.top + mTileSize) {
		mPosition.y = mArena.top + mTileSize;
	}

	// Set player sprite angle
	float angle = (atan2(mousePosition.y - mResolution.y / 2, 
		mousePosition.x - mResolution.x / 2) * 180) / 3.141;
	mSprite.setRotation(angle);
}

void Player::upgradeSpeed() {
	mSpeed += (START_SPEED * 0.2);
}

void Player::upgradeHealth() {
	mHealth += START_HEALTH * 0.2;
}

void Player::increaseHealthLevel(int amount) {
	mHealth += amount;

	if (mHealth > mMaxHealth) {
		mHealth = mMaxHealth;
	}
}