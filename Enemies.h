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
	int currentFrame = 0;
	int direction = 1;
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
	virtual int giveDamage(bool onGround, int P_x, int P_y, int off_x) = 0;
};

Enemies::~Enemies() {
	// Empty implementation
}

class MotoBug : public Enemies {
public:
	MotoBug(int x, int y, char** lvl) : Enemies("Data/motobug.png", 42, 30)
	{
		hp = 2;
		this->x = x;
		this->y = y;
		this->lvl = lvl;
		isActive = true;
		proximity = false;
		speed = 3.4f;
		isPlayerRight = false;
		direction = 1;
		// how many frames across?
		frameCount = enemyTexture.getSize().x / 40;
		// set the initial position
		enemySprite.setPosition(x, y);
		enemySprite.setScale(2.2, 2.24);
	}

	void move(int P_x, int P_y, int off_x, int off_y) override {
		if (isActive && proximityCheck(P_x + off_x, P_y)) {
			if (P_x + off_x < x) {
				isPlayerRight = false;
				x -= speed;
			}
			else if (P_x + off_x > x) {
				isPlayerRight = true;
				x += speed;
			}
		}
		enemySprite.setPosition(x - off_x, y);

	}
	bool proximityCheck(int P_x, int P_y) override {
		proximity = false;
		// Check if the player is within a certain distance
		if (((P_y / 64 == y / 64) || (P_y / 64 == (y / 64) - 1)) &&
			((P_x / 64 >= (x / 64) - 6) && (P_x / 64 <= (x / 64) + 6))) {
			proximity = true;
		}
		if (lvl[y / 64][(x / 64) + 2] == 'w' && isPlayerRight)
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

	int giveDamage(bool onGround, int P_x, int P_y, int off_x) override
	{
		// Only process if enemy is active and player is in same grid cell
		if (isActive && (P_x + off_x) / 64 == x / 64 && P_y / 64 == y / 64) {
			// Player is jumping/falling onto enemy (classic "stomp" mechanic)
// In MotoBug::giveDamage:
			if (!onGround) 
			{
				takeDamage(1);    // subtract exactly 1 HP, not all of it
				return 0;
			}

			// Player runs into enemy while on ground (side collision)
			else 
			{
				return 10;     // Player takes damage
			}
		}
		return 0;
	}
	void animateSprite()
	{
		// Adjust animation speed based on whether the motobug is moving
		float animationSpeed = proximity ? 150.0f : 250.0f; // Faster when in proximity mode

		if (animationClock.getElapsedTime().asMilliseconds() < animationSpeed)
			return;

		// Update frame based on direction
		currentFrame += direction;

		// Handle direction changes at the boundaries with proper clamping
		if (currentFrame >= int(frameCount)) {
			currentFrame = frameCount - 1;
			direction = -1;
		}
		else if (currentFrame < 0) {
			currentFrame = 0;
			direction = 1;
		}

		// Flip sprite horizontally based on movement direction
		if (isPlayerRight) {
			enemySprite.setScale(2.2, 2.24);  // Normal orientation
		}
		else {
			enemySprite.setScale(-2.2, 2.24); // Flipped orientation
		}

		// Update the texture rectangle
		frameRect.left = currentFrame * frameRect.width;
		enemySprite.setTextureRect(frameRect);
		animationClock.restart();
	}



};



class CrabMeat : public Enemies
{
private:
	sf::Texture ProjectileTexture;
	sf::Sprite ProjectileSprite;
	sf::Clock ProjectileClock;
	float patrolSpeed = 1.5;
	float spriteScale = 2.5;
	int cell_size = 64; 
	int frameWidth = 43; // Consistent frame width
	bool movingForward = true; // Track animation direction

public:
	CrabMeat(int x, int y, char** lvl) : Enemies("Data/CrabMeat.png", 44, 31)
	{
		hp = 4;
		this->x = x;
		this->y = y;
		this->lvl = lvl;
		isActive = true;
		proximity = false;
		speed = 3.4;
		isPlayerRight = false;
		frameCount = 10; // Total frames in sprite sheet
		currentFrame = 0;
		

		
		enemySprite.setPosition(x, y);
		enemySprite.setScale(spriteScale, spriteScale);

        frameRect = sf::IntRect(0,0,43,31);
        enemySprite.setTextureRect(frameRect);
		enemySprite.setOrigin(frameRect.width / 2.f, frameRect.height / 2.f);
   

    }
	

	void move(int P_x, int P_y, int off_x, int off_y) override 
	{
		if (!isActive) return;

		// Simple movement logic
		x += direction * patrolSpeed;

		// Wall collision detection
		int cellY = y / cell_size;
		int aheadPixel = (direction > 0) ? x + frameWidth : x - 1;
		int cellX_ahead = aheadPixel / cell_size;

		if (lvl[cellY][cellX_ahead] == 'w') 
		{
			direction *= -1; // Reverse direction at wall
			x += direction * patrolSpeed; // Move away from wall
		}

		// Update sprite position and flip based on direction
		enemySprite.setPosition(x - off_x, y);
		//enemySprite.setScale(direction < 0 ? -spriteScale : spriteScale, spriteScale);
	}

	void animateSprite() override
	{
		if (!isActive) return;

		const int DELAY_MS = 120;
		if (animationClock.getElapsedTime().asMilliseconds() < DELAY_MS)
			return;

		// advance frame index
		currentFrame = (currentFrame + 1) % frameCount;

		// move the rect to the correct frame
		frameRect.left = currentFrame * frameRect.width;
		enemySprite.setTextureRect(frameRect);

		animationClock.restart();
	}

	int giveDamage(bool onGround, int P_x, int P_y, int off_x) override
	{
		if (!isActive) return 0;

		if ((P_x + off_x) / cell_size == x / cell_size
			&& P_y / cell_size == y / cell_size)
		{
			if (!onGround) {
				// Player is jumping on crab
				takeDamage(1);
				return 0;
			}
			else {
				// Player collides with crab horizontally
				return 1; // Damage to player
			}
		}
		return 0;
	}

	void takeDamage(int dmg) override
	{
		hp -= dmg;
		if (hp <= 0) isActive = false;
	}

	void draw(sf::RenderWindow& window) override 
	{
		if (isActive) {
			window.draw(enemySprite);
		}
	}

	bool proximityCheck(int P_x, int P_y) override {
		proximity = false;
		// Check if the player is within a certain distance
		if (((P_y / 64 == y / 64) || (P_y / 64 == (y / 64) - 1)) &&
			((P_x / 64 >= (x / 64) - 6) && (P_x / 64 <= (x / 64) + 6))) {
			proximity = true;
		}

		// Check for walls
		if (lvl[y / 64][(x / 64) + 2] == 'w' && isPlayerRight)
			proximity = false;
		if (lvl[y / 64][(x / 64) - 1] == 'w' && !isPlayerRight)
			proximity = false;

		return proximity;
	}
};