// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "Bullet.h"
#include "TextureHolder.h"

using namespace sf;

int main() {
	// Init TextureHolder
	TextureHolder textureHolder;
	// GAME STATES
	enum class State {
		PAUSED, LEVELING_UP, GAME_OVER, PLAYING
	};
	// START STATE
	State state = State::GAME_OVER;
	// Get screen resolution and create SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

	// View for main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	
	Clock clock;
	Time gameTimeTotal;
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;
	Player player;
	IntRect arena;

	VertexArray background;
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	Time lastPressed;

	// Add mouse crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrossHair;
	Texture textureCrossHair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrossHair.setTexture(textureCrossHair);
	spriteCrossHair.setOrigin(25, 25);

	while (window.isOpen()) {
		// HANDLE INPUT
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Space && state == State::PLAYING) {
					state = State::PAUSED;
				}
				else if (event.key.code == Keyboard::Space && state == State::PAUSED) {
					state = State::PLAYING;
				}
				else if (event.key.code == Keyboard::Space && state == State::GAME_OVER) {
					state = State::LEVELING_UP;
				}
				if (state == State::PLAYING) {
					// handle reload
					if (event.key.code == Keyboard::R) {
						if (bulletsSpare >= clipSize) {
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
						} else if (bulletsSpare > 0) {
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
						} else {
							//TODO:
						}
					}
				}
			}
		}

		// Close Game on pressing ESC
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		if (state == State::PLAYING) {
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player.moveUp();
			}
			else {
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::A)) {
				player.moveLeft();
			}
			else {
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::S)) {
				player.moveDown();
			}
			else {
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::D)) {
				player.moveRight();
			}
			else {
				player.stopRight();
			}

			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate
					&& bulletsInClip > 0) {

					bullets[currentBullet].shoot(player.getCenter().x,
						player.getCenter().y,
						mouseWorldPosition.x,
						mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 99) {
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					bulletsInClip--;
				}
			}
		}

		if (state == State::LEVELING_UP) {
			if (event.key.code == Keyboard::Num1
				|| event.key.code == Keyboard::Num2
				|| event.key.code == Keyboard::Num3
				|| event.key.code == Keyboard::Num4
				|| event.key.code == Keyboard::Num5
				|| event.key.code == Keyboard::Num6) {
				state = State::PLAYING;
			}
			if (state == State::PLAYING) {
				arena.width = 1200;
				arena.height = 1200;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createBackground(background, arena);
				// Spawn Player
				player.spawn(arena, resolution, tileSize);
				// Create zombie horde
				numZombies = 100;
				// Delete previous allocated zombie memory if any
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				clock.restart();
			}
		}

		// UPDATE GAME FRAME
		if (state == State::PLAYING) {
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();

			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			spriteCrossHair.setPosition(mouseWorldPosition);

			player.update(dtAsSeconds, Mouse::getPosition());

			Vector2f playerPosition(player.getCenter());

			mainView.setCenter(player.getCenter());

			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive()) {
					zombies[i].update(dtAsSeconds, playerPosition);
				}
			}

			for (int i = 0; i < 100; i++) {
				if (bullets[i].isInFlight()) {
					bullets[i].update(dtAsSeconds);
				}
			}
		} //  END UPDATING THE SCENE

		if (state == State::PLAYING) {
			window.clear();

			window.setView(mainView);

			window.draw(background, &textureBackground);

			for (int i = 0; i < numZombies; i++) {
				window.draw(zombies[i].getSprite());
			}

			for (int i = 0; i < 100; i++) {
				if (bullets[i].isInFlight()) {
					window.draw(bullets[i].getShape());
				}
			}

			window.draw(player.getSprite());

			window.draw(spriteCrossHair);
		}
		if (state == State::LEVELING_UP) {
		}
		if (state == State::PAUSED) {
		}
		if (state == State::GAME_OVER) {
		}
		window.display();
	} // END GAME LOOP

	// Clear memory
	delete[] zombies;

	return 0;
}