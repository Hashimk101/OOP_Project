#pragma once
//#include "Header.h"

class Enemies {
protected:
	int hp;
	int x, y;
	bool isActive;
	bool proximity;
	sf::Texture enemyTexture;
	sf::Sprite enemySprite;
	sf::IntRect    frameRect;
	sf::Clock      animationClock;
	unsigned int frameCount;
	int  currentFrame = 0;
	int  direction = +1;
	float speed;
	bool isPlayerRight;
	char** lvl;

public:
	Enemies(const std::string& texturePath, int frameW, int frameH) : frameRect(0, 0, frameW, frameH)
	{
		if (!enemyTexture.loadFromFile(texturePath))
			std::cerr << "Error loading enemy texture: " << texturePath << "\n";

		enemySprite.setTexture(enemyTexture);
		enemySprite.setTextureRect(frameRect);


		frameCount = enemyTexture.getSize().x / frameW;
	}
	virtual ~Enemies();


	virtual void move(int P_x, int P_y, int off_x, int off_y) = 0;
	//virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void takeDamage(int damage) = 0;
	virtual void animateSprite() = 0;
	virtual bool proximityCheck(int P_x, int P_y) = 0;
	virtual int giveDamage(bool onGround, int P_x, int P_y) = 0;
};

Enemies::~Enemies() {
	// Empty implementation
}

class MotoBug : public Enemies {
public:
	MotoBug(int x, int y, char** lvl) : Enemies("Data/motobug.png", 40, 30)
	{
		hp = 2;
		this->x = x;
		this->y = y;
		this->lvl = lvl;
		isActive = true;
		proximity = false;
		speed = 2.0f;
		isPlayerRight = false;
		direction = 1;
		// how many frames across?
		frameCount = enemyTexture.getSize().x / 40;
		// set the initial position
		enemySprite.setPosition(x, y);
		enemySprite.setScale(2.2, 2.24);
	}

	void move(int P_x, int P_y, int off_x, int off_y) override {
		if (isActive && proximityCheck(P_x, P_y)) {
			if (P_x < x) {
				isPlayerRight = true;
				x -= 2;
				enemySprite.setPosition(x, y);
			}
			else if (P_x > x) {
				isPlayerRight = true;
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

	void takeDamage(int damage) override
	{
		hp -= damage;
		if (hp <= 0) {
			isActive = false;
		}
	}

	int giveDamage(bool onGround, int P_x, int P_y) override
	{
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
		return 0;
	}
	void animateSprite()
	{
		const int DELAY_MS = 100;

		if (animationClock.getElapsedTime().asMilliseconds() < DELAY_MS)
			return;

		// if we hit either end, flip direction
		if (currentFrame == int(frameCount) - 1)
		{
			direction = -1;
		}

		else if (currentFrame == 0)
		{
			direction = +1;
		}

		// step one frame
		currentFrame += direction;

		// update the sub-rectangle and sprite
		frameRect.left = currentFrame * frameRect.width;
		enemySprite.setTextureRect(frameRect);

		animationClock.restart();
	}



};