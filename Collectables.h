#pragma once
#include "Maps.h"

sf::SoundBuffer coinBuffer;
sf::Sound coinSound;

class Collectables {
    // Your existing variables
    int collect_x, collect_y;
    int collect_width = 16, collect_height = 16;
   /* int collect_width = 48, collect_height = 48;*/

    int collect_offset_x = 0, collect_offset_y = 0;
    int collect_hit_box_factor_x = 4, collect_hit_box_factor_y = 4;
    int currentFrame;
    sf::Clock CollectablesClock;

    // Single coin sprite (replaces array)
    sf::Sprite coinSprite;
    sf::Texture coinTexture;
    sf::IntRect coinsRect;
    char** lvl;
    //Size of texture
    Vector2u SizeOfSpritesheet;



public:
    Collectables(char** lvl) : coinsRect(0, 0, collect_width, collect_height), currentFrame(0) {
        if (!coinTexture.loadFromFile("Data/Ring.png")) 
        {
            std::cout << "Failed to load coin texture!" << std::endl;
        }
        coinSprite.setTexture(coinTexture);
        coinSprite.setTextureRect(coinsRect);
        coinSprite.setScale(2.5, 2.5);
        this->lvl = lvl;
        SizeOfSpritesheet = coinTexture.getSize();
        // Track if coin is active
        bool coinActive = false;
        if (!coinBuffer.loadFromFile("Data/Ring.wav")) {
            std::cout << "Failed to load coin sound!" << std::endl;
        }
        coinSound.setBuffer(coinBuffer);
        coinSound.setVolume(100); 
    }
    //CALCULATE THE NUMBER OF FRAMES IN THE TEXTURE
    int GetFrameNum(Texture& Txt)
    {
        int frameNum = SizeOfSpritesheet.x / SizeOfSpritesheet.y;
        return frameNum;
    }
    void placeCoins() {
        srand(time(0)); // Random seed

        int validSpaces = 0;
        for (int i = 3; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (lvl[i][j] == 's') {
                    if ((i + 1 < height && lvl[i + 1][j] == 'w')) 
                    {
                        validSpaces++;
                    }
                }
            }
        }

        int numCoins = 30; 
        if (numCoins > validSpaces)
        { 
            numCoins = validSpaces; // Adjust if not enough spaces
        }

        int coinsPlaced = 0;

        // Step 3: Randomly place coins
        while (coinsPlaced < numCoins) {
            int j = rand() % width;
            int i = rand() % height;

            if (lvl[i][j] == 's') {
                if ((i + 1 < height && lvl[i + 1][j] == 'w') ||
                    (i + 2 < height && lvl[i + 2][j] == 'w')) {

                    lvl[i][j] = 'c';
                    coinsPlaced++;
                }
            }
        }
    }


	Sprite& getCoinSprite() 
    {
		return coinSprite;
	}


    ///////
	/// @brief this needs to go into the main loop////
    ///////
    void draw(sf::RenderWindow& window, int offset) {
        for (int i = 0; i < height; i++) {
            for (int j = offset/64; j < (offset + 1300)/64; j++) {
				if (lvl[i][j] == 'c') {
					coinSprite.setPosition(j * cell_size - offset, i * cell_size);
					window.draw(coinSprite);
				}
            }
        }
    }
    void AnimateCoins() 
    {
        if (CollectablesClock.getElapsedTime().asMilliseconds ()>100)
        {
            currentFrame = (currentFrame + 1) % GetFrameNum(coinTexture);
            coinsRect.left = currentFrame * SizeOfSpritesheet.y;
			coinSprite.setTextureRect(coinsRect);
			CollectablesClock.restart();
        }

    }
    void checkCoinCollision(int playerX, int playerY, int offsetX, int offsetY, int hitX, int hitY) {
        // Player size: based on your scaling
        int playerWidth = 40 * 2.5;
        int playerHeight = 40 * 2.5;
        // Coin size: based on your scaling
        int coinWidth = 16 * 2.5;
        int coinHeight = 16 * 2.5;

        // Fix the calculation to check around the actual player position
        int startI = (playerY + hitY) / cell_size;                    // Top of player hitbox
        int endI = (playerY + hitY + playerHeight) / cell_size;       // Bottom of player hitbox
        int startJ = (playerX + offsetX + hitX) / cell_size;          // Left of player hitbox
        int endJ = (playerX + offsetX + hitX + playerWidth) / cell_size; // Right of player hitbox

        // Loop only through the area occupied by the player's hitbox
        for (int i = startI; i <= endI; i++) {
            for (int j = startJ; j <= endJ; j++) {
                if (i >= 0 && i < height && j >= 0 && j < width) { // Safety check
                    if (lvl[i][j] == 'c') { // If there's a coin
                        // Calculate coin's screen position
                        int coinX = j * cell_size - offsetX;
                        int coinY = i * cell_size;  // No need to subtract offsetY here

                        // AABB collision check
                        if ((playerX < coinX + coinWidth) &&
                            (playerX + playerWidth > coinX) &&
                            (playerY < coinY + coinHeight) &&
                            (playerY + playerHeight > coinY))
                        {
                            // Collision detected - collect the coin
                            lvl[i][j] = 's';  // Replace with empty space
                            coinSound.play();
                            std::cout << "Collected a coin at grid [" << i << "][" << j << "]!" << std::endl;
                        }
                    }
                }
            }
        }
    }




};