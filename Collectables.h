#pragma once
#include "Maps.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ScoreBoard.h"

// Base Collectable class
class Collectable {
protected:
    // Common properties
    int collect_width, collect_height;
    int collect_offset_x = 0, collect_offset_y = 0;
    int collect_hit_box_factor_x = 4, collect_hit_box_factor_y = 4;
    int currentFrame;
    sf::Clock collectableClock;

    // Sprite components
    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect textureRect;
    char** lvl;
    sf::Vector2u sizeOfSpritesheet;

    // Sound components
    sf::SoundBuffer soundBuffer;
    sf::Sound sound;

    // Scaling factor
    float scaleFactor;

    // Level identifier
    char levelChar;

public:
    // Memebr Initializer List with parameters for width, height, and other properties
    Collectable(char** lvl, int width, int height, float scale,  const std::string& texturePath, const std::string& soundPath, char levelIdentifier) : collect_width(width), collect_height(height), currentFrame(0),  scaleFactor(scale), levelChar(levelIdentifier) 
    {

        textureRect = sf::IntRect(0, 0, width, height);

        if (!texture.loadFromFile(texturePath)) {
            std::cout << "Failed to load texture: " << texturePath << std::endl;
        }
        else {
            std::cout << "Successfully loaded texture: " << texturePath << std::endl;
        }

        sprite.setTexture(texture);
        sprite.setTextureRect(textureRect);
        sprite.setScale(scale, scale);

        this->lvl = lvl;
        sizeOfSpritesheet = texture.getSize();

        if (!soundBuffer.loadFromFile(soundPath)) {
            std::cout << "Failed to load sound: " << soundPath << std::endl;
        }
        else {
            std::cout << "Successfully loaded sound: " << soundPath << std::endl;
        }

        sound.setBuffer(soundBuffer);
        sound.setVolume(100);
    }

    virtual ~Collectable() {}

    // Calculate frame count in texture
    int getFrameNum() {
        return sizeOfSpritesheet.x / collect_width;
    }

    sf::Sprite& getSprite() {
        return sprite;
    }

    // Virtual methods that can be overridden by child classes
    virtual void place() = 0; // Pure virtual - must be implemented by children

    virtual void draw(sf::RenderWindow& window, int offset) {
        for (int i = 0; i < height; i++) {
            for (int j = offset / 64; j < (offset + 1300) / 64 && j < width; j++) {
                if (lvl[i][j] == levelChar) {
                    sprite.setPosition(j * cell_size - offset, i * cell_size);
                    window.draw(sprite);
                }
            }
        }
    }

    virtual void animate()
    {
        if (collectableClock.getElapsedTime().asMilliseconds() > 100) {
            currentFrame = (currentFrame + 1) % getFrameNum();
            textureRect.left = currentFrame * collect_width;
            sprite.setTextureRect(textureRect);
            collectableClock.restart();
        }
    }

    virtual void checkCollision(int playerX, int playerY, int offsetX, int offsetY, int hitX, int hitY)
    {
        // Player size based on your scaling
        int playerWidth = 40 * 2.5;
        int playerHeight = 40 * 2.5;

        // Collectable size based on scaling
        int collectableWidth = collect_width * scaleFactor;
        int collectableHeight = collect_height * scaleFactor;

        // Fix the calculation to check around the actual player position
        int startI = (playerY + hitY) / cell_size;                    // Top of player hitbox
        int endI = (playerY + hitY + playerHeight) / cell_size;       // Bottom of player hitbox
        int startJ = (playerX + offsetX + hitX) / cell_size;          // Left of player hitbox
        int endJ = (playerX + offsetX + hitX + playerWidth) / cell_size; // Right of player hitbox

        // Loop only through the area occupied by the player's hitbox
        for (int i = startI; i <= endI; i++) {
            for (int j = startJ; j <= endJ; j++) {
                if (i >= 0 && i < height && j >= 0 && j < width) { // Safety check
                    if (lvl[i][j] == levelChar) { // Collectable exists
                        // Calculate collectable's screen position
                        int collectX = j * cell_size - offsetX;
                        int collectY = i * cell_size;  // No need to subtract offsetY here

                        // AABB collision check
                        if ((playerX < collectX + collectableWidth) &&
                            (playerX + playerWidth > collectX) &&
                            (playerY < collectY + collectableHeight) &&
                            (playerY + playerHeight > collectY))
                        {

                            lvl[i][j] = 's';
                            sound.play();
                            onCollect(i, j);
                        }
                    }
                }
            }
        }
    }

    // Method called when item is collected
    virtual void onCollect(int i, int j) {
        std::cout << "Collected an item at grid [" << i << "][" << j << "]!" << std::endl;
    }
};

// Child class for rings/coins
class RingCoin : public Collectable {
public:
    RingCoin(char** lvl)
        : Collectable(lvl, 16, 16, 2.5, "Data/Ring.png", "Data/Ring.wav", 'c') {
    }

    void place() override
    {
        srand(static_cast<unsigned int>(time(0))); // Random seed

        int validSpaces = 0;
        for (int i = 3; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (lvl[i][j] == 's') {
                    if ((i + 1 < height && lvl[i + 1][j] == 'w')) {
                        validSpaces++;
                    }
                }
            }
        }

        int numCoins = 50;
        if (numCoins > validSpaces) {
            numCoins = validSpaces; 
        }

        int coinsPlaced = 0;

        // Randomly place coins
        while (coinsPlaced < numCoins) {
            int j = rand() % width;
            int i = rand() % (height - 3) + 3; 

            if (lvl[i][j] == 's') {
                if ((i + 1 < height && lvl[i + 1][j] == 'w') ||
                    (i + 2 < height && lvl[i + 2][j] == 'w')) {
                    lvl[i][j] = 'c';
                    coinsPlaced++;
                }
            }
        }
    }

    void onCollect(int i, int j) override {
        std::cout << "Collected a ring at grid [" << i << "][" << j << "]!" << std::endl;
    }
};

// Child class for diamonds
class ExtraLife : public Collectable 
{
public: ExtraLife(char** lvl): Collectable(lvl, 90, 94, 0.5, "Data/GreenPowerup.png", "Data/SpecialRing.wav", 'L')
{
 }

    void place() override
    {
   srand(static_cast<unsigned int>(time(0))); // Random seed

        // Use different seed offset for diamonds to prevent same placement as coins
        for (int i = 0; i < 10; i++) 
        {
            rand();
        }

        int validSpaces = 0;
        for (int i = 3; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (lvl[i][j] == 's') {
                    // For diamonds, we'll use different placement criteria
                    if ((i + 1 < height && lvl[i + 1][j] == 'w')) 
                    {
                        validSpaces++;
                    }
                }
            }
        }

        int ExtraLifeNume = 5;
        if (ExtraLifeNume > validSpaces)
        {
            ExtraLifeNume = validSpaces;
        }

        int ExtraLifePlaced = 0;

        // Randomly place diamonds - try to place in harder-to-reach areas
        while (ExtraLifePlaced < ExtraLifeNume) {
            int j = rand() % width;
            int i = rand() % (height - 3) + 3; // Start from row 3

            // Make diamonds appear higher up or in more challenging locations
            if (lvl[i][j] == 's' && i <7)
            { // Higher elevation
                if ((i + 1 < height && lvl[i + 1][j] == 'w')) {
                    lvl[i][j] = 'L';
                    ExtraLifePlaced++;
                }
            }
        }
    }

    void onCollect(int i, int j) override 
    {
        std::cout << "Collected an Entra Life at grid [" << i << "][" << j << "]! +5 points!" << std::endl;
    }
};
// Child class for special Boost
class SpecialBoost : public Collectable
{
public:
    SpecialBoost(char** lvl) : Collectable(lvl, 48, 48, 1.5, "Data/diamonds.png", "Data/Ring.wav", 'd')
    {

    }

    void place() override {
        srand(static_cast<unsigned int>(time(0))); // Random seed

        // Use different seed offset for diamonds to prevent same placement as coins
        for (int i = 0; i < 10; i++)
        {
            rand();
        }

        int validSpaces = 0;
        for (int i = 3; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (lvl[i][j] == 's') {
                    // For diamonds, we'll use different placement criteria
                    if ((i + 1 < height && lvl[i + 1][j] == 'w')) {
                        validSpaces++;
                    }
                }
            }
        }

        int numDiamonds = 5;
        if (numDiamonds > validSpaces)
        {
            numDiamonds = validSpaces;
        }

        int diamondsPlaced = 0;

        // Randomly place diamonds - try to place in harder-to-reach areas
        while (diamondsPlaced < numDiamonds) {
            int j = rand() % width;
            int i = rand() % (height - 3) + 3; // Start from row 3

            // Make diamonds appear higher up or in more challenging locations
            if (lvl[i][j] == 's' && i < 7) { // Higher elevation
                if ((i + 1 < height && lvl[i + 1][j] == 'w')) {
                    lvl[i][j] = 'd';
                    diamondsPlaced++;
                }
            }
        }
    }
};