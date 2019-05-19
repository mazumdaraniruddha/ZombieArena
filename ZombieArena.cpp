// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <fstream>
#include "Player.h"
#include "ZombieArena.h"
#include "Bullet.h"
#include "TextureHolder.h"
#include "Pickup.h"

using namespace sf;
using namespace std;

int main() {
	// Save game file
	const string GAME_SAVED_FILE = "gamedata/scores.txt";

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

	// Couple of pickups in game
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	// Game state
	int score = 0;
	int hiScore = 0;

	// Home/GameOver screen
	Sprite spriteGameOver(TextureHolder::GetTexture("graphics/background.png"));
	spriteGameOver.setPosition(0, 0);
	// View for the HUD
	View hudView(FloatRect(0, 0, resolution.x, resolution.y));
	// Srpite for Ammo Icon
	Sprite spriteAmmoIcon(TextureHolder::GetTexture("graphics/ammo_icon.png"));
	spriteAmmoIcon.setPosition(20, 980);
	// Font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");
	// Paused Text
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press [Space] to continue...");
	// Game Over Text
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press [Space] to start");
	// Level Up Text
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	stringstream levelUpStream;
	levelUpStream << "1 - Increased rate of fire"
		<< "\n2 - Increased clip size"
		<< "\n3 - Increased max health";
	levelUpText.setString(levelUpStream.str());
	// Ammo Text
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);
	// Score Text
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);
	// Load high score from saved file
	ifstream inputFile(GAME_SAVED_FILE);
	if (inputFile.is_open()) {
		inputFile >> hiScore;
		inputFile.close();
	}
	// Hi Score Text
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	stringstream s;
	s << "Hi Score: " << hiScore;
	hiScoreText.setString(s.str());
	// Zombies Remaining Text
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");
	// Wave Number Text
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");
	// Health Bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	int framesSinceLastHUDUpdate = 0;
	int fpsMeasurementFrameInterval = 1000;

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

				// Setup pickups in game
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				// Create zombie horde
				numZombies = 40;
				// Delete previous allocated zombie memory if any
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				clock.restart();
			}
		}

		/// UPDATE GAME FRAME
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

			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			// Check for collisions
			// For each bullet
			for (int i = 0; i < 100; i++) {
				// For each zombie
				for (int j = 0; j < numZombies; j++) {
					// Check for valid/alive data
					if (bullets[i].isInFlight() && zombies[j].isAlive()) {
						// Detect if there is a bullet hit
						if (bullets[i].getPosition().intersects(
							zombies[j].getPosition())) {
							// Stop the bullet
							bullets[i].stop();
							// Register the zombie hit, and see if it was a kill
							if (zombies[j].hit()) {
								score += 10;
								if (score > hiScore) {
									hiScore = score;
								}
								numZombiesAlive--;
								// Finish level when all zombies killed
								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}
							}
						}
					}
				}
			} // End Zombie shot

			// check for player/zombie collision
			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive() && player.getPosition().intersects(
					zombies[i].getPosition())) {
					if (player.hit(gameTimeTotal)) {

					} 
					if (player.getHealth() <= 0) {
						// Death scenario
						state = State::GAME_OVER;

						ofstream outputFile(GAME_SAVED_FILE);
						outputFile << hiScore;
						outputFile.close();
					}
				}
			} // End Player/Zombie Collision check

			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned()) {
				player.increaseHealthLevel(healthPickup.gotIt());
			}

			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) {
				bulletsSpare += ammoPickup.gotIt();
			}

			// Health bar update
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			framesSinceLastHUDUpdate++;
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval) {
				stringstream ssAmmo;
				stringstream ssScore;
				stringstream ssHiScore;
				stringstream ssWave;
				stringstream ssZombiesAlive;

				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				ssScore << "Score: " << score;
				scoreText.setString(ssScore.str());

				ssHiScore << "Hi Score: " << hiScore;
				hiScoreText.setString(ssHiScore.str());

				ssWave << "Wave: " << wave;
				waveNumberText.setString(ssWave.str());

				ssZombiesAlive << "Zombies: " << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				framesSinceLastHUDUpdate = 0;
			} // End HUD update

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

			if (ammoPickup.isSpawned()) {
				window.draw(ammoPickup.getSprite());
			}

			if (healthPickup.isSpawned()) {
				window.draw(healthPickup.getSprite());
			}

			window.draw(spriteCrossHair);

			// Switch to HUD view
			window.setView(hudView);
			// draw HUD stuff
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);

		}
		if (state == State::LEVELING_UP) {
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		if (state == State::PAUSED) {
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER) {
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		window.display();
	} // END GAME LOOP

	// Clear memory
	delete[] zombies;

	return 0;
}