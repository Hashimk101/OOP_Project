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


	virtual void move(int P_x, int P_y, int off_x, int off_y, Scores& s) = 0;
	//virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void takeDamage(int damage, Scores& s) = 0;
	virtual void animateSprite() = 0;
	virtual bool proximityCheck(int P_x, int P_y) = 0;
	virtual int giveDamage(int upVelocity, int P_x, int P_y, int off_x, Scores& s) = 0;
	Sprite& getEnemySprite() 
	{
		return enemySprite;
	}
	void setHP(int hp) {
		this->hp = hp;
	}
	int getHP() {
		return hp;
	}
	void setActive(bool active) {
		isActive = active;
	}
	bool isActiveEnemy() {
		return isActive;
	}
};

Enemies::~Enemies() {
	// Empty implementation
}

class MotoBug : public Enemies {
public:
	MotoBug() : Enemies("Data/motobug.png", 40, 30) {
	}
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

	void move(int P_x, int P_y, int off_x, int off_y, Scores& s) override
	{
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
	bool proximityCheck(int P_x, int P_y) override
	{
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

	void takeDamage(int damage, Scores& s) override
	{
		hp -= damage;
		if (hp <= 0)
		{
			s.addMotoBugKill();
			isActive = false;
		}
		//std::cout << "Motobug HP: " << hp << std::endl;
	}

	int giveDamage(int upVelocity, int P_x, int P_y, int off_x, Scores& s) override
	{
		//std::cout << isActive << " " << hp << std::endl;
		if (!isActive) return 0;

		if (checkCollision(P_x + off_x, P_y)) {
			// Check if player is falling and coming from above
			float playerBottom = P_y + 80;
			float enemyTop = y - 40 / 2.0f;

			if (upVelocity > 0 && playerBottom < enemyTop + 15) 
			{
				
				takeDamage(hp, s);
				// Return immediately to prevent side damage
				return 0;
			}

			// Don't check isActive again here; return 10 since it's not a stomp
			return 10;
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

	bool checkCollision(int P_x, int P_y) {
		float enemyLeft = x - 40 / 2.0f;
		float enemyTop = y - 40 / 2.0f;
		float enemyRight = enemyLeft + 40;
		float enemyBottom = enemyTop + 30;

		float playerLeft = P_x;
		float playerTop = P_y;
		float playerRight = playerLeft + 40;
		float playerBottom = playerTop + 80;
		
		//std::cout << enemyTop - 30 << " " << playerTop << std::endl;
		return (playerRight > enemyLeft && playerLeft < enemyRight &&
			playerBottom >= enemyTop - 20 && playerTop < enemyBottom);
	}

};


class CrabMeat : public Enemies
{
private:
	sf::Texture ProjectileTexture;
	sf::Sprite ProjectileSprite;
	sf::Clock ProjectileClock;

	sf::Clock projectileCooldown;    // Timer for cooldown between shots
	float projectileCooldownTime = 2.5f; // 2 second delay between shots
	bool isOnCooldown = false;      // Flag to track if we're currently on cooldown

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
	// Constructor
	CrabMeat() : Enemies("Data/CrabMeat.png", 44, 30)
	{
	}
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


	void move(int P_x, int P_y, int off_x, int off_y, Scores& s) override {

		//ProjectileSprite.setPosition(x - 10, y - 10);
		if (proximityCheck(P_x + off_x, P_y)) {
			//std::cout << "true" << std::endl;
			throwProjectile(off_x);
		}
		else {
			projectileActive = false;
		}
		if (!isActive) return;
		else
		{
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
		enemySprite.setScale(direction < 0 ? -spriteScale : +spriteScale, spriteScale);
	}

	void animateSprite() override
	{
		const int delay_ms = 100; // Delay in milliseconds for frame change
		// e.g. for 10 frames:
		int frameWidths[8] = { 43, 46, 45, 45, 45, 49, 49, 49 };
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

	int giveDamage(int upVelocity, int P_x, int P_y, int off_x, Scores& s) override
	{
		if (!isActive) return 0;

		// Define player bounding box (assuming player size is 49x51 as hinted in original code)
		float playerLeft = P_x + off_x;
		float playerRight = P_x + off_x + 49;
		float playerTop = P_y;
		float playerBottom = P_y + 51;

		// Define crab bounding box (based on sprite size and position)
		float crabWidth = frameRect.width * spriteScale; // Current frame width scaled
		float crabHeight = frameRect.height * spriteScale; // Frame height scaled
		float crabLeft = x - crabWidth / 2; // Centered on x
		float crabRight = x + crabWidth / 2;
		float crabTop = y - crabHeight / 2; // Centered on y
		float crabBottom = y + crabHeight / 2;

		// AABB collision check for player and crab
		bool collisionWithCrab = (playerLeft < crabRight &&
			playerRight > crabLeft &&
			playerTop < crabBottom &&
			playerBottom > crabTop);

		if (collisionWithCrab)
		{
			if (upVelocity > 0) { // Player is falling (not jumping)
				takeDamage(hp, s);   // Destroy crab
				return 0;         // No damage to player
			}
			else {
				// Player collides with crab horizontally
				return 5; // Damage to player
			}
		}

		// Keep original projectile collision check unchanged
		if (projCollision(P_x, P_y, off_x) && projectileActive) {
			projectileActive = false; // Deactivate projectile
			return 10; // Damage to player from projectile
		}

		return 0; // No damage
	}

	void takeDamage(int dmg, Scores& s) override
	{
		hp -= dmg;
		if (hp <= 0) 
		{
			s.addCrabMeatKill();
			isActive = false;
		}
	}

	void draw(sf::RenderWindow& window) override
	{
		if (isActive)
		{
			window.draw(enemySprite);
		}
		if (projectileActive && isActive) {
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

		int tileY = (y / 64);
		int playerTileY = (P_y + 49) / 64;

		if ((playerTileY <= tileY && playerTileY >= tileY - 4) &&
			((P_x + 49) / 64 >= (x / 64) - 7) &&
			((P_x + 49) / 64 <= (x / 64) + 7)) {
			proximity = true;
		}



		 //Check for walls
		if (lvl[y / 64][(x / 64) + 2] == 'w' && isPlayerRight)
			proximity = false;
		if (lvl[y / 64][(x / 64) - 1] == 'w' && !isPlayerRight)
			proximity = false;

		return proximity;
	}

	void throwProjectile(int off_x)
	{
		// Check if we're on cooldown
		if (isOnCooldown) {
			// Check if cooldown period has elapsed
			if (projectileCooldown.getElapsedTime().asSeconds() >= projectileCooldownTime) {
				isOnCooldown = false; // Reset cooldown flag
			}
			else {
				// Still on cooldown, don't throw
				return;
			}
		}

		if (!projectileActive)
		{
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

			// Check if projectile hits a wall or goes out of bounds
			if (lvl[int(proj_y / 64)][int(proj_x / 64)] == 'w') {
				projectileActive = false;
				// Start cooldown when projectile is destroyed
				projectileCooldown.restart();
				isOnCooldown = true;
			}

			if (tileX < 0 || tileX > 17 || tileY < 0 || tileY > 12) {
				projectileActive = false;
				// Start cooldown when projectile goes off-screen
				projectileCooldown.restart();
				isOnCooldown = true;
			}

			float dt = ProjectileClock.getElapsedTime().asSeconds();

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

	bool projCollision(int P_x, int P_y, int off_x) {
		if (!projectileActive) return false;

		int playerX = P_x + off_x;
		int playerY = P_y;
		int playerWidth = 35;
		int playerHeight = 40;

		// Get projectile position (top-left corner)
		int projleft = proj_x - 13; // 13 is half of 26
		int projtop = proj_y - 14;  // 14 is half of 28

		// Check for collision using the exact style from your example
		if (projleft < playerX + playerWidth &&
			projleft + 50 > playerX &&
			projtop  < playerY + playerHeight + 10 &&
			projtop + 35 > playerY) {

			std::cout << "Collision happened" << std::endl;
			//projectileActive = false;  // Deactivate projectile on collision

			projectileCooldown.restart();
			isOnCooldown = true;
			return true;
		}
		return false;
	}
	Sprite& getMeatBall() 
	{
		return ProjectileSprite;
	}
};


class BatBrain : public Enemies
{
private:
	float spriteScale;
	// where the bat first spawned
	float minX, maxX;
	float range;
	bool isReturning;
	float originX, originY;
public:
	BatBrain() : Enemies("Data/BatBrain.png", 32, 33) {}
	BatBrain(int x, int y, char** lvl) : Enemies("Data/BatBrain.png", 32, 33)
	{
		originX = x;
		originY = y;

		hp = 3;
		this->x = x;
		this->y = y;
		this->lvl = lvl;
		isActive = true;
		proximity = false;
		speed = 2.5;
		isPlayerRight = false;
		direction = 1;
		frameCount = 9;
		enemySprite.setPosition(x, y);
		spriteScale = 3;
		enemySprite.setScale(spriteScale, spriteScale);
		frameRect = sf::IntRect(0, 0, 32, 33);
		enemySprite.setTextureRect(frameRect);
		range = 200;
		maxX = x + range;
		minX = x - range;
		isReturning = false;

	}
	void animateSprite() override
	{
		const int delay_ms = 100; // Delay in milliseconds for frame change

		int frameWidths[9] = { 31, 28, 38, 33, 34, 37, 36, 36, 40 };
		int frameOffset[9];

		frameOffset[0] = 0;
		//Offset for each itrect
		for (int i = 1;i < 9;i++)
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


		animationClock.restart();
	}

	void move(int P_x, int P_y, int off_x, int off_y, Scores& s) override
	{

		if (!isActive) return;

		float sonicX = (P_x + off_x);
		float sonicY = P_y;

		if (!isReturning && sonicY > y && proximityCheck(P_x + off_x, P_y))
		{

			float dx = sonicX - x;
			float dy = sonicY - y;
			float dist = std::sqrt(dx * dx + dy * dy);
			if (dist > 0.0f) {
				x += (dx / dist) * speed;
				y += (dy / dist) * speed;
			}
			enemySprite.setScale(dx < 0 ? +spriteScale : -spriteScale, spriteScale);

			if (std::abs(x - sonicX) < 10 && std::abs(y - sonicY) < 10) {
				giveDamage(5, P_x, P_y, off_x, s );
				isReturning = true;
			}
		}
		else if (isReturning)
		{

			float dx = originX - x;
			float dy = originY - y;
			float dist = std::sqrt(dx * dx + dy * dy);
			if (dist > speed) {
				x += (dx / dist) * speed;
				y += (dy / dist) * speed;
			}
			else {

				x = originX;
				y = originY;
				direction = +1;     // reset patrol direction
				isReturning = false;
			}
			enemySprite.setScale(dx < 0 ? +spriteScale : -spriteScale, spriteScale);


		}
		else
		{

			x += direction * speed;
			if (x < minX)
			{
				x = minX; direction = +1;
			}
			else if (x > maxX)
			{
				x = maxX; direction = -1;
			}

			enemySprite.setScale(direction < 0 ? +spriteScale : -spriteScale, spriteScale);
		}
		enemySprite.setPosition(x - off_x, y);

	}


	bool proximityCheck(int P_x, int P_y)
	{
		proximity = false;
		// Check if the player is within a certain distance
		if (((P_x / 64 >= (x / 64) - 6) && (P_x / 64 <= (x / 64) + 6))) {
			proximity = true;
		}
		if (lvl[y / 64][(x / 64) + 2] == 'w' && isPlayerRight)
			proximity = false;
		if (lvl[y / 64][(x / 64)] == 'w' && !isPlayerRight)
			proximity = false;
		return proximity;
	}
	int giveDamage(int upVelocity, int P_x, int P_y, int off_x, Scores& s)
	{
		if (!isActive) return 0;
		if (checkCollision(P_x + off_x, P_y))
		{

			if (upVelocity > 0) // checks if the player is falling and falling ONLY, not jumping
			{

				takeDamage(1, s);    // take damage from enemy and die on first hit
				return 0;
			}

			else
			{
				return 10;     // Player takes damage
			}
		}
		return 0;
	}
	void draw(sf::RenderWindow& window) override {
		if (isActive)
			window.draw(enemySprite);
	}

	void takeDamage(int damage, Scores& s) override
	{
		hp -= damage;
		if (hp <= 0) 
		{
			s.addBatBrainKill();
			isActive = false;
		}

	}
	bool checkCollision(int P_x, int P_y)
	{
		float batLeft = x - 38 / 2.0f;   // Enemy width = 38
		float batTop = y - 40 / 2.0f;    // Enemy height = 40
		int playerWidth = 40;
		int playerHeight = 40;

		if ((P_x < batLeft + 38) &&       // Player's left is before bat's right
			(P_x + playerWidth > batLeft) &&  // Player's right is after bat's left
			(P_y < batTop + 40) &&        // Player's top is before bat's bottom
			(P_y + playerHeight > batTop))    // Player's bottom is after bat's top
		{
			//std::cout << "YES\n";
			return true;
		}
		return false;
	}
};


class BuzzBomber : public Enemies
{
private:
	float spriteScale;
	float originX;            // where the bat first spawned
	float minX, maxX;
	float range;
	sf::Texture projText;
	sf::Sprite projSprite;
	bool isProjActive;
	sf::Clock projClock;
	int proj_x, proj_y;
	int proj_vx, proj_vy;
	int targetX, targetY;
	 float PROJ_SPEED = 8.0f; // Constant projectile speed
	bool isInvincible; // for cases of multiple hits to the buzz
	sf::Clock invincibilityClock;  // Added to track invincibility duration
	 float INVINCIBILITY_DURATION = 1.5f; // 1.5 second of invincibility after being hit

public:
	BuzzBomber() : Enemies("Data/BuzzBomber.png", 47, 22)
	{}
	BuzzBomber(BuzzBomber& Other) : Enemies("Data/BuzzBomber.png", 47, 22)
	{
		hp = Other.hp;
		x = Other.x;
		y = Other.y;
		lvl = Other.lvl;
		isActive = Other.isActive;
		proximity = Other.proximity;
		speed = Other.speed;
		isPlayerRight = Other.isPlayerRight;
		direction = Other.direction;
		frameCount = Other.frameCount;
		frameRect = Other.frameRect;
		spriteScale = Other.spriteScale;
		projText.loadFromFile("Data/egg.png");
		projSprite.setTexture(projText);
		projSprite.setScale(0.03, 0.03);
		isInvincible = false;
	}
	BuzzBomber(int x, int y, char** lvl) : Enemies("Data/BuzzBomber.png", 47, 22)
	{
		hp = 5;
		this->x = x;
		this->y = y;
		this->lvl = lvl;
		isActive = true;
		proximity = false;
		speed = 3.4f;
		isPlayerRight = false;
		direction = 1;
		frameCount = 4;
		enemySprite.setPosition(x, y);
		spriteScale = 2.5;
		enemySprite.setScale(spriteScale, spriteScale);
		frameRect = sf::IntRect(0, 0, 47, 22);
		enemySprite.setTextureRect(frameRect);
		range = 250;
		maxX = x + range;
		minX = x - range;
		projText.loadFromFile("Data/egg.png");
		projSprite.setTexture(projText);
		projSprite.setScale(0.03, 0.03);
		isInvincible = false;

	}
	void animateSprite() override
	{
		const int delay_ms = 200; // Delay in milliseconds for frame change

		int frameWidths[4] = { 47, 47, 48, 47 };
		int frameOffset[4];

		frameOffset[0] = 0;
		//Offset for each itrect
		for (int i = 1;i < 4;i++)
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


		animationClock.restart();
	}


	Sprite& getProjSprite() 
	{
		return projSprite;
	}

	void move(int P_x, int P_y, int off_x, int off_y, Scores& s) override
	{
		// Reset invincibility after duration
		if (isInvincible && invincibilityClock.getElapsedTime().asSeconds() >= INVINCIBILITY_DURATION) {
			isInvincible = false;
			invincibilityClock.restart();
		}

		//std::cout << (P_x + off_x)/64 << " " << x/64 << " " << P_y/64 << " " << y/64 << std::endl;
		if (proximityCheck(P_x + off_x, P_y)) {
			throwProjectile(P_x, P_y, off_x);
			projCollision(P_x + off_x, P_y);
		}
		else {
			isProjActive = false;
		}
		if (!isActive) return;
		x += direction * speed;
		// if we passed either bound, clamp & reverse
		if (x < minX)
		{
			x = minX;
			direction = +1;
		}
		else if (x > maxX)
		{
			x = maxX;
			direction = -1;
		}
		// update sprite screen position
		enemySprite.setPosition(x - off_x, y);

		// flip sprite to face movement
		enemySprite.setScale(direction < 0 ? +spriteScale : -spriteScale, spriteScale);
	}

	//virtual void update() = 0;
	void draw(sf::RenderWindow& window)
	{
		if (isActive)
		{
			window.draw(enemySprite);
			if (isProjActive) {

				window.draw(projSprite);
			}
		}

	}
	void takeDamage(int damage, Scores& s) {
		if (!isInvincible && isActive) { // Only take damage if not invincible
			hp -= damage;
			std::cout << "BuzzBomber HP: " << hp << std::endl;
			if (hp <= 0) 
			{
				s.addBuzzBomberKill();
				isActive = false;
			}
			else {
				// Activate invincibility
				isInvincible = true;
				invincibilityClock.restart();
				//enemySprite.setColor(sf::Color(255, 255, 255, 128)); // Make semi-transparent during invincibility
			}
		}
	};

	int giveDamage(int upVelocity, int P_x, int P_y, int off_x, Scores& s)
	{
		if (!isActive) return 0;
		//if (!isProjActive) return 0;
		if (((P_x + off_x) / 64 >= (x / 64) - 1 && (P_x + off_x) / 64 <= (x / 64) + 1) &&
			(P_y / 64) == y / 64)
		{
			if (invincibilityClock.getElapsedTime().asSeconds() >= INVINCIBILITY_DURATION) {
				takeDamage(hp, s);
				//std::cout << invincibilityClock.getElapsedTime().asSeconds() << std::endl;
			}
		}
		if (projCollision(P_x + off_x, P_y)) {
			return 10; // damage if it hits the egg lol, ande se kon marta ha
		}
		return 0;
	}

	bool proximityCheck(int P_x, int P_y) override {

		if (((P_x) / 64 >= (x / 64) - 10 && (P_x) / 64 <= (x / 64) + 10)) {
			return true;
		}

		return false;
	}
	void throwProjectile(int P_x, int P_y, int off_x) {
		if (!isProjActive) {
			proj_x = x + 20;
			proj_y = y + 20;

			targetX = P_x + off_x + 15; // somewhat at the center of the sprite or slightly ahead
			targetY = P_y + 100;
			isProjActive = true;

			float dx = targetX - proj_x;
			float dy = targetY - proj_y;
			float distance = std::sqrt(dx * dx + dy * dy);

			// Set constant velocity components
			if (distance > 0) {
				proj_vx = ((dx / distance) * PROJ_SPEED);
				proj_vy = (dy / distance) * PROJ_SPEED;
			}
			else {
				proj_vx = 0;
				proj_vy = 0;
			}
		}
		else {
			proj_x += proj_vx;
			proj_y += proj_vy;
			//std::cout << (proj_x - off_x) / 64 << " " << proj_y / 64 << std::endl;
			if ((proj_x - off_x) / 64 < 0 || (proj_x - off_x) / 64 >= 18 || proj_y / 64 <= 0 || proj_y / 64 >= 12) {
				isProjActive = false;
			}
		}
		//std::cout << isProjActive << std::endl;
		projSprite.setPosition(proj_x - off_x, proj_y);
	}

	bool projCollision(int P_x, int P_y) {
		int projleft = proj_x;
		int projtop = proj_y;

		if (projleft < P_x + 37 &&
			projleft + 37 > P_x &&
			projtop < P_y + 35 &&
			projtop + 37 > P_y)
		{
			std::cout << "Collision happened" << std::endl;
			isProjActive = false;  // Deactivate projectile on collision
			return true;
		}

		return false;
	}
};

class EggStinger : public Enemies
{
	sf::Clock stayTime;
	float maxStayTime = 10.0f;
	bool right, attack, down;
	bool stay; // dont let me leave MURPH!!!!!!......Dramatic aaa
	float speed = 2.5f;
	int destroyedIndex = 0;
	float maxCeiling = 1;
	sf::Clock damageClock;
	float damageTime = 0.6f;
public:
	EggStinger() : Enemies("Data/EggStinger.png", 89, 86) {}
	EggStinger(int x, int y, char** lvl) : Enemies("Data/EggStinger.png", 89, 86)
	{
		this->x = x;
		this->y = y;
		this->lvl = lvl;
		enemySprite.setPosition(x, y);
		right = false;
		down = false;
		attack = false;
		stay = true;
		stayTime.restart();
		hp = 200;
		frameCount = 4;
	}
	void move(int P_x, int P_y, int off_x, int off_y, Scores& s) override
	{
		enemySprite.setPosition(x - off_x, y);
		/*std::cout << "State: " << (right ? "right" : "left") << " "
			<< (down ? "down" : "up") << " "
			<< (attack ? "attack" : "normal") << " "
			<< (stay ? "stay" : "move") << std::endl;*/

		// Check if we're at the limit of destroyed blocks
		if (destroyedIndex >= 10) {
			std::cout << "No platform to stand on\n";
			// yaha game end karna
		}

		if (stayTime.getElapsedTime().asSeconds() >= maxStayTime && !attack) {
			stay = false;
			stayTime.restart();
		}

		if (stayTime.getElapsedTime().asSeconds() >= maxStayTime / 3 && attack) {
			stay = false;
			destroyBlock();
			// Switch direction after destroying block
			right = !right;
			attack = false;
			down = false;
			stayTime.restart();
			if(right)
				destroyedIndex++;
		}

		// Don't move if we're staying in place
		if (stay) {
			return;
		}

		// MOVEMENT LOGIC sahi se dekhna isse..........
		//Moving up (both in left and right phases)
		if (!down && y / 64 > maxCeiling) {
			y -= speed; // Moving up until we reach ceiling
		}
		// Horizontal movement after reaching ceiling
		else if (!down && y / 64 <= maxCeiling) {
			if (right) {
				x += speed; // Move right
				if (x / 64 >= 19 - destroyedIndex) { // Check right bound
					down = true; // Switch to moving down
				}
			}
			else {
				x -= speed; // Move left
				if (x / 64 <= destroyedIndex) { // Check left bound
					down = true; // moving down, copy and over
				}
			}
		}
		//Moving down until finding a block to attack
		else if (down) {
			y += speed; // Moving down
			// Check box below
			if (lvl[(y / 64) + 1][x / 64] == 'w') {
				attack = true; 
				stay = true;   // Stay in place during attack cuz it is what it is
				stayTime.restart();
			}
		}
		std::cout << hp << std::endl;
	}

	void destroyBlock() {
		lvl[(y / 64) + 1][x / 64] = 's';
	}
	void draw(sf::RenderWindow& window)override
	{
		if(isActive)
			window.draw(enemySprite);
	}

	void takeDamage(int damage, Scores& s) override
	{
		if (damageClock.getElapsedTime().asSeconds() <= damageTime) {
			return;
		}
		damageClock.restart();
		hp -= damage;
		if (hp <= 0)
		{
			s.addBossKill();
			isActive = false;
		}

	}

	void animateSprite() override
	{
		const int delay_ms = 400; // Delay in milliseconds for frame change

		int frameWidths[4] = { 89, 121, 121, 128 };
		int frameOffset[4];

		frameOffset[0] = 0;
		//Offset for each itrect
		for (int i = 1;i < 4;i++)
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
		enemySprite.setScale(2, 2);


		animationClock.restart();
	}
	bool proximityCheck(int P_x, int P_y)  override
	{
		return true;
	}
	bool checkCollision(int P_x, int P_y)
	{
		float enemyTop = y - 89 / 2.0f;
		float enemyBottom = y + 89 / 2.0f;
		float enemyLeft = x - 89 / 2.0f;
		float enemyRight = x + 89 / 2.0f;

		float playerTop = P_y;
		float playerBottom = P_y + 40;
		float playerLeft = P_x;
		float playerRight = P_x + 40;
		if (playerRight > enemyLeft &&
			playerLeft < enemyRight &&
			playerBottom > enemyTop &&
			playerTop < enemyBottom)
		{
			return true;
		}

		return false;
	}

	int giveDamage(int upVelocity, int P_x, int P_y, int off_x, Scores& s) override
	{
		if (upVelocity == 0 && checkCollision(P_x + off_x, P_y) && damageClock.getElapsedTime().asSeconds() >= damageTime) {
			return 15;
		}
		else {
			if (checkCollision(P_x + off_x, P_y)) {
				takeDamage(10, s);
				//std::cout << "yes\n";
				return 0;
			}
		}
		return 0;
	}
	Sprite& GetEnemySprite() 
	{
		return enemySprite;
	}
};