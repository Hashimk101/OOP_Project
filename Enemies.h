#pragma once
#include "Header.h"

class Enemies {
protected:
	int hp;
	float x, y;
	bool isActive;
	bool proximity;
	sf::Texture enemyTexture;
	sf::Sprite enemySprite;
	float speed;
	bool isPlayerRight;
	char** lvl;

public:
	Enemies();
	virtual ~Enemies() = 0;


	virtual void move(int P_x, int P_y) = 0;
	virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void takeDamage(int damage) = 0;
	virtual void animateSprite() = 0;
	virtual bool proximityCheck(int P_x, int P_y) = 0;
	virtual int giveDamage(bool onGround) = 0;
};

class MotoBug : public Enemies {
public:
	MotoBug(int x, int y, char** lvl) {
		hp = 2;
		this->x = x;
		this->y = y;
		this->lvl = lvl;
		isActive = true;
		proximity = false;
		speed = 2.0f;
		isPlayerRight = false;
	}

	void move(int P_x, int P_y) override {
		if (isActive && proximityCheck(P_x, P_y)) {
			if (P_x < x) {
				x -= 2;
				enemySprite.setPosition(x, y);
			}
			else if (P_x > x) {
				x += 2;
				enemySprite.setPosition(x, y);

			}
		}
	}
	bool proximityCheck(int P_x, int P_y) override {
		proximity = false;
		// Check if the player is within a certain distance
		if (((P_y / 64 == y / 64) || (P_y / 64 == (y / 64) - 1) || (P_y / 64 == (y / 64) - 2)) &&
			((P_x / 64 >= (x / 64) - 3) && (P_x / 64 <= (x / 64) + 3))) {
			proximity = true;
		}
		if (lvl[y / 64][(x / 64) + 1] == 'w' && isPlayerRight)
			proximity = false;
		if (lvl[y / 64][(x / 64) - 1] == 'w' && !isPlayerRight)
			proximity = false;
		return proximity;
	}

	void draw(sf::RenderWindow& window) override {
		if (isActive)
			window.draw(enemySprite);
	}

	void takeDamage(int damage) override {
		hp -= damage;
		if (hp <= 0) {
			isActive = false;
		}
	}

	int giveDamage(bool onGround, int P_x, int P_y) override {
		// Only process if enemy is active and player is in same grid cell
		if (isActive && P_x / 64 == x / 64 && P_y / 64 == y / 64) {
			// Player is jumping/falling onto enemy (classic "stomp" mechanic)
			if (!onGround) {
				takeDamage(hp); // Enemy takes damage
				return 0;      // Player takes no damage
			}
			// Player runs into enemy while on ground (side collision)
			else {
				return 10;     // Player takes damage
			}
		}

		// No collision, no damage
		return 0;
	}
};
