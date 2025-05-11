#pragma once
#include "Maps.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ScoreBoard.h"

const int height = 14;


// Base Collectable class
class Collectable
{
protected:
    // Common properties
    int collect_width, collect_height;
    int collect_offset_x = 0, collect_offset_y = 0;
    int collect_hit_box_factor_x = 4, collect_hit_box_factor_y = 4;
    int currentFrame;
    sf::Clock collectableClock;
    Maps &map;

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
    Collectable(Maps &m, int width, int height, float scale,  const std::string& texturePath, const std::string& soundPath, char levelIdentifier) :map(m),  collect_width(width), collect_height(height), currentFrame(0),  scaleFactor(scale), levelChar(levelIdentifier) 
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
        this->lvl = m.getMap();
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
            for (int j = offset / 64; j < (offset + 1300) / 64 && j < map.GetLevelWidth(); j++) {
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

    virtual void checkCollision(int playerX, int playerY, int offsetX, int offsetY, int hitX, int hitY, Scores& score, MySprite& Activeplayer)
    {
        // Player dimensions (unchanged from original)
        int playerWidth = 40 * 2.5;
        int playerHeight = 40 * 2.5;

        // Collectable dimensions (unchanged)
        int collectableWidth = collect_width * scaleFactor;
        int collectableHeight = collect_height * scaleFactor;

        // Calculate player's center position in world coordinates
        int playerCenterX = playerX + offsetX + hitX + playerWidth / 2;
        int playerCenterY = playerY + hitY + playerHeight / 2;

        // Convert to grid coordinates
        int gridX = playerCenterX / cell_size;
        int gridY = playerCenterY / cell_size;

        // Boundary check
        if (gridX >= 0 && gridX < map.GetLevelWidth() && gridY >= 0 && gridY < height)
        {
            if (lvl[gridY][gridX] == levelChar)
            {
                // Calculate collectable's screen position

                if (levelChar == 'c') 
                {
                    score.addCoin();
                }
                if (levelChar == 'L')
                {
                    Activeplayer.AddHp();
                }
                if (levelChar == 'd' && Activeplayer.isKnuckles)
                {
                    cout << "You are now invisible!" << endl;
                    cout << "active player" << Activeplayer.isKnuckles << endl;
                    Activeplayer.MakeInvisible();

                }
                if (levelChar == 'd' && Activeplayer.isSonic)
                {
                    cout << "You are now running!" << endl;
                    Activeplayer.setcurrentIndex(11);
                  

                }
                    lvl[gridY][gridX] = 's';
                    sound.play();
                    onCollect(gridY, gridX);
            }
            if (lvl[gridY - 1][gridX] == levelChar)
            {
                if (levelChar == 'c')
                {
                    score.addCoin();
                }
                if (levelChar == 'L')
                {
                    Activeplayer.AddHp();
                }
				if (levelChar == 'd' && Activeplayer.isKnuckles)
				{
                    Activeplayer.MakeInvisible();	
				}
                if (levelChar == 'd' && Activeplayer.isSonic)
                {
                    cout << "You are now running!" << endl;
                    Activeplayer.setcurrentIndex(11);
                    Activeplayer.StartRunning();
                 

                }

                // Calculate collectable's screen position
                lvl[gridY - 1][gridX] = 's';
                sound.play();
                onCollect(gridY - 1, gridX);

            }
        }
    }

    // Method called when item is collected
    virtual void onCollect(int i, int j) {
        std::cout << "Collected an item at grid [" << i << "][" << j << "]!" << std::endl;
    }

    void save(std::ofstream& out)  {
        // Count collectables
        int count = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < map.GetLevelWidth(); j++) {
                if (lvl[i][j] == levelChar) {
                    count++;
                }
            }
        }

        // Write count and coordinates
        out << count << ' ';
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < map.GetLevelWidth(); j++) {
                if (lvl[i][j] == levelChar) {
                    out << i << ' ' << j << ' ';
                }
            }
        }
        out << '\n';
    }

    void load(std::ifstream& in)  {
        int count;
        if (!(in >> count)) {
            std::cout << "Error: Failed to read RingCoin count." << std::endl;
            return;
        }

        // Clear existing collectables of this type
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < map.GetLevelWidth(); j++) {
                if (lvl[i][j] == levelChar) {
                    lvl[i][j] = 's';
                }
            }
        }

        // Read and place collectables
        for (int k = 0; k < count; k++) {
            int i, j;
            if (!(in >> i >> j)) {
                std::cout << "Error: Failed to read RingCoin coordinates." << std::endl;
                return;
            }
            if (i >= 0 && i < height && j >= 0 && j < map.GetLevelWidth()) {
                lvl[i][j] = levelChar;
            }
        }
    }





};

// Child class for rings/coins
class RingCoin : public Collectable {
public:
    RingCoin(Maps& m)
        : Collectable(m, 16, 16, 2.5, "Data/Ring.png", "Data/Ring.wav", 'c') {
    }

    void place() override
    {
        srand(static_cast<unsigned int>(time(0))); // Random seed

        int validSpaces = 0;
        for (int i = 3; i < height; i++) {
            for (int j = 0; j < map.GetLevelWidth(); j++) {
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
            int j = rand() % map.GetLevelWidth();
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
public: ExtraLife(Maps& m): Collectable(m, 90, 94, 0.5, "Data/GreenPowerup.png", "Data/SpecialRing.wav", 'L')
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
            for (int j = 0; j < map.GetLevelWidth(); j++) {
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
            int j = rand() % map.GetLevelWidth();
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
    SpecialBoost(Maps& m) : Collectable(m, 48, 48, 1.5, "Data/diamonds.png", "Data/Ring.wav", 'd')
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
            for (int j = 0; j < map.GetLevelWidth(); j++) {
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
            int j = rand() % map.GetLevelWidth();
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