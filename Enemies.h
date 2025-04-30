#pragma once
#include "Header.h"

class Enemies {
protected:
	int hp;
	float x, y;
	bool isActive;
	bool promiximity;
	sf::Texture enemyTexture;
	sf::Sprite enemySprite;
	float speed;
	bool isPlayerRight;

public:
	Enemies();
	virtual ~Enemies() = 0;


	virtual void move(int P_x, int P_y) = 0;
	virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void takeDamage(int damage) = 0;
	virtual void animateSprite() = 0;
	virtual bool proximityCheck(int P_x, int P_y) = 0;
};

class MotoBug : public Enemies {
public:
	MotoBug(int x, int y) {
		hp = 2;
		this->x = x;
		this->y = y;
		isActive = true;
		promiximity = false;
		speed = 2.0f;
		isPlayerRight = false;
	}

	void move(int P_x, int P_y) override {
		if (isActive && proximityCheck()) {
			if (P_x < x) {
				x -= 2;
				enemySprite.setPosition(x, y);
			}
			else if (P_x > x) {
				x += 2;
				enemySprite.setPosition(x, y);
		}
	}
	bool proximityCheck(int P_x, int P_y) override {
		proximity = false;
		// Check if the player is within a certain distance
		if (((P_y / 64 == y / 64) || (P_y / 64 == (y / 64) - 1) || (P_y / 64 == (y / 64) - 2)) &&
			((P_x / 64 >= (x / 64) - 3) && (P_x / 64 <= (x / 64) + 3))) {
			proximity = true;
		}
		return proximity;
	}

	void draw(sf::RenderWindow& window) override {
		if(isActive)
			window.draw(enemysprite);
	}
