#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet {
private:
	// Bullet position
	Vector2f mPosition;
	// Bullet shape
	RectangleShape mBulletShape;
	// Is the bullet flying
	bool mInFlight = false;
	// Bullet speed
	float mBulletSpeed = 1000;
	// Bullet travel distance
	float mBulletDistanceX;
	float mBulletDistanceY;
	// Boundaries
	float mMaxX;
	float mMaxY;
	float mMinY;
	float mMinX;
public:
	Bullet();
	void stop();
	bool isInFlight();
	void shoot(float startX, float startY, float targetX, float targetY);
	FloatRect getPosition();
	RectangleShape getShape();
	void update(float elapsedTime);
};