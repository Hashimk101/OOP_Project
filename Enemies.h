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
	virtual int giveDamage(int upVelocity, int P_x, int P_y, int off_x) = 0;
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
		speed = 3.4f;
		isPlayerRight = false;
		direction = 1;
		// how many frames across?
		frameCount = enemyTexture.getSize().x / 40;
		// set the initial position
		enemySprite.setPosition(x, y);
		enemySprite.setScale(2.2, 2.24);
		enemySprite.setOrigin(frameRect.width / 2.0f, frameRect.height / 2.0f);
	}

	void move(int P_x, int P_y, int off_x, int off_y) override {
		if (isActive && proximityCheck(P_x + off_x, P_y)) {
			if (P_x + off_x + 40 < x) {
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
		if (lvl[y / 64][(x / 64)] == 'w' && !isPlayerRight)
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
		//std::cout << "Motobug HP: " << hp << std::endl;
	}

	int giveDamage(int upVelocity, int P_x, int P_y, int off_x) override
	{
		//std::cout << (P_x + off_x + 40) << " " << P_y << " " << x/64 << " " << y/64 << std::endl;
		// Only process if enemy is active and player is in same grid cell
		if (isActive && ((P_x + off_x + 40) / 64 == x / 64) && ((P_y + 64) / 64 == y / 64)) {
			// Player is jumping/falling onto enemy (classic "stomp" mechanic)
// In MotoBug::giveDamage:
			if (upVelocity > 0) // checks if the player is falling and falling ONLY, not jumping
			{
				takeDamage(hp);    // take damage from enemy and die on first hit
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
		//std::cout << isPlayerRight << std::endl;
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

		// Update the texture rectangle
		frameRect.left = currentFrame * frameRect.width;
		enemySprite.setTextureRect(frameRect);

		// Set the origin to center of sprite (do this once after changing the texture rect)

		// Flip sprite horizontally based on movement direction
		if (isPlayerRight) {
			enemySprite.setScale(-2.2, 2.24);  // Flipped orientation
		}
		else {
			enemySprite.setScale(2.2, 2.24); // Normal orientation
		}

		// Update position with centered origin
		enemySprite.setPosition(x, y);

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
	float spriteScale = 3;
	int cell_size = 64; 
	int frameWidth = 44; // Consistent frame width
	bool movingForward = true; // Track animation direction
	float originX;            // where the crab first spawned
	float minX, maxX;
	float range;
	float proj_x, proj_y;
	float prj_speed_y = -4.5;
	float prj_speed_x = 3.2;
	float gravity = 10;
	bool projectileActive = false;

public:
	CrabMeat(int x, int y, char** lvl) : Enemies("Data/CrabMeat.png", 44, 30)
	{
		hp = 4;
		this->x = x;
		this->y = y;
		this->lvl = lvl;
		isActive = true;
		proximity = false;
		speed = 3.4;
		isPlayerRight = false;
		frameCount = 8;
		range = 128;
		enemySprite.setPosition(x, y);
		enemySprite.setScale(spriteScale, spriteScale);

		frameRect = sf::IntRect(0, 0, 43, 30);
		enemySprite.setTextureRect(frameRect);
		originX = x;
		minX = originX - range;
		maxX = originX + range;

		ProjectileTexture.loadFromFile("Data/CrabMeatBall.png");
		ProjectileSprite.setTexture(ProjectileTexture);
		ProjectileSprite.setOrigin(ProjectileTexture.getSize().x / 2.0f, ProjectileTexture.getSize().y / 2.0f);
		ProjectileSprite.setScale(2, 2);
	}
	

	void move(int P_x, int P_y, int off_x, int off_y) override {

		//ProjectileSprite.setPosition(x - 10, y - 10);
		if (proximityCheck(P_x + off_x, P_y)) {
			//std::cout << "true" << std::endl;
			throwProjectile(off_x);
		}
		else {
			//std::cout << "false\n";
		}
		if (!isActive) return;
		else {
			animateSprite();
		}

		// step horizontally
		x += direction * patrolSpeed;

		// if we passed either bound, clamp & reverse
		if (x < minX) {
			x = minX;
			direction = +1;
		}
		else if (x > maxX) {
			x = maxX;
			direction = -1;
		}

		// update sprite screen position
		enemySprite.setPosition(x - off_x, y);

		// flip sprite to face movement
		enemySprite.setScale(
			direction < 0 ? -spriteScale : +spriteScale,
			spriteScale
		);
	}

	void animateSprite() override
	{
		const int delay_ms = 100; // Delay in milliseconds for frame change
		// e.g. for 10 frames:
		int frameWidths[8] = { 43, 46, 45, 45, 45, 49, 49, 49};
		int frameOffset[8];

		frameOffset[0] = 0;
		//Offset for each itrect
		for (int i = 1;i < 8;i++)
		{
			frameOffset[i] = frameOffset[i - 1] + frameWidths[i - 1];
		}

		if (animationClock.getElapsedTime().asMilliseconds() < delay_ms)
			return;

		
		currentFrame = (currentFrame + 1) % frameCount;
		int w = frameWidths[currentFrame];
		int l = frameOffset[currentFrame];

		// update the rect
		frameRect = sf::IntRect(l, 0, w, frameRect.height);
		enemySprite.setTextureRect(frameRect);

		// re-center origin on that new width
		enemySprite.setOrigin(w * 0.5, frameRect.height * 0.5);
		//enemySprite.setPosition(x, y);

		animationClock.restart();
	}

	int giveDamage(int upVelocity, int P_x, int P_y, int off_x) override
	{
		if (!isActive) return 0;
		//std::cout << P_x + off_x + 49 << " " << x << " " << P_y + 49 << " " << y << std::endl;

		if (((P_x + off_x + 49) / cell_size == x / cell_size ||
			(P_x + off_x + 49) / cell_size == (x + 49)/ cell_size)
			&& ((P_y + 51) / cell_size == y / cell_size))
		{
			/*std::cout << "Damage to player" << std::endl;*/
			if (upVelocity > 0) { // checks if the player is falling and falling ONLY, not jumping
				takeDamage(hp);
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
		if (projectileActive) {
			window.draw(ProjectileSprite);
		}
	}

	bool proximityCheck(int P_x, int P_y) override {
		proximity = false;
		// Check if the player is within a certain distance
		//std::cout << x << " " << P_x + off_x + 49 << std::endl;
		if ((P_x + 49) < x)
			isPlayerRight = false;
		else
			isPlayerRight = true;

		if ((((P_y + 49) / 64 == y / 64) || ((P_y + 49) / 64 == (y / 64) - 1)) &&
			(((P_x  + 49)/ 64 >= (x / 64) - 6) && ((P_x + 49) / 64 <= (x / 64) + 6))) {
			proximity = true;
		}

		// Check for walls
		/*if (lvl[y / 64][(x / 64) + 2] == 'w' && isPlayerRight)
			proximity = false;
		if (lvl[y / 64][(x / 64) - 1] == 'w' && !isPlayerRight)
			proximity = false;*/

		return proximity;
	}

	void throwProjectile(int off_x)
	{
		if (!projectileActive) {
			ProjectileClock.restart();
			projectileActive = true;
			if (isPlayerRight) {
				proj_x = x + 30;
				proj_y = y - 20;
			}
			else {
				proj_x = x - 30;
				proj_y = y - 20;
			}
		}
		else {
			int tileX = (proj_x - off_x) / 64;
			int tileY = proj_y / 64;
			//std::cout << int(proj_y / 64) << " " << int(proj_x / 64) <<" " << y / 64 <<  " " << x/64 <<std::endl;
			//std::cout << << " " << x / 64 << std::endl;
			if (lvl[int(proj_y / 64)][int(proj_x / 64)] == 'w') {
				projectileActive = false;
			}
			//std::cout << tileX << std::endl;
			if (tileX < 0 || tileX > 17 || tileY < 0 || tileY > 12) {
				projectileActive = false;
			}
			//std::cout << projectileActive << std::endl;
			float dt = ProjectileClock.getElapsedTime().asSeconds();
			//projectileActive = true;
			if (!isPlayerRight) {
				proj_x -= prj_speed_x * dt;
			}
			else {
				proj_x += prj_speed_x * dt;
			}
			proj_y += prj_speed_y + 0.5 * gravity * dt * dt;
			ProjectileSprite.setPosition(proj_x - off_x, proj_y);
		}
	}

};