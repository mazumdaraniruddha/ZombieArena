#include "stdafx.h"
#include "Bullet.h"

using namespace sf;

Bullet::Bullet() {
	mBulletShape.setSize(Vector2f(10, 5));
	mBulletShape.setFillColor(Color::Red);
}

void Bullet::shoot(float startX, float startY, float targetX, float targetY) {
	mInFlight = true;
	mPosition.x = startX;
	mPosition.y = startY;

	float gradient = (startX - targetX) / (startY - targetY);

	if (gradient < 0) {
		gradient *= -1;
	}

	float ratioXY = mBulletSpeed / (1 + gradient);

	mBulletDistanceY = ratioXY;
	mBulletDistanceX = ratioXY * gradient;

	if (targetX <  startX) {
		mBulletDistanceX *= -1;
	}

	if (targetY < startY) {
		mBulletDistanceY *= -1;
	}

	float range = 1000;
	mMinX = startX - range;
	mMaxX = startX + range;
	mMinY = startY - range;
	mMaxY = startY + range;

	mBulletShape.setPosition(mPosition);
}

void Bullet::stop() {
	mInFlight = false;
}

bool Bullet::isInFlight() {
	return mInFlight;
}

FloatRect Bullet::getPosition() {
	return mBulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape() {
	return mBulletShape;
}

void Bullet::update(float elapsedTime) {
	mPosition.x += mBulletDistanceX * elapsedTime;
	mPosition.y += mBulletDistanceY * elapsedTime;

	mBulletShape.setPosition(mPosition);

	if (mPosition.x > mMaxX || mPosition.x < mMinX || mPosition.y > mMaxY || mPosition.y < mMinY) {
		mInFlight = false;
	}
}