#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>


class Enemy {
protected:
    sf::Texture    texture;
    sf::Sprite     sprite;
    sf::IntRect    frameRect;       // current sub-rectangle
    sf::Clock      animationClock;
    int            currentFrame = 0;
    unsigned int   frameCount = 0;
    int            frameDelayMs = 100; // ms between frames

public:

    Enemy(const std::string& texturePath,
        int frameW, int frameH,
        float scale = 1.f,
        int   delayMs = 100)
        : frameRect(0, 0, frameW, frameH),
        frameDelayMs(delayMs)
    {
        if (!texture.loadFromFile(texturePath))
            std::cerr << "Error loading enemy texture: " << texturePath << "\n";

        sprite.setTexture(texture);
        sprite.setTextureRect(frameRect);
        sprite.setScale(scale, scale);

        // how many frames across?
        frameCount = texture.getSize().x / frameW;
    }

    virtual ~Enemy() = default;

    virtual void update() {
        if (animationClock.getElapsedTime().asMilliseconds() >= frameDelayMs) {
            currentFrame = (currentFrame + 1) % frameCount;
            frameRect.left = currentFrame * frameRect.width;
            sprite.setTextureRect(frameRect);
            animationClock.restart();
        }
    }

  
    void setPosition(float x, float y) { sprite.setPosition(x, y); }
    void setScale(float x, float y) { sprite.setScale(x, y); }
    sf::Sprite& getSprite() { return sprite; }
    virtual void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};


/*class BatBrain : public Enemy {
public:
    BatBrain(): Enemy("Data/BatBrain.png",64,1.f,100)
    {
    }
};?*/


/// Motobug: crawls, follows player (HP=2)
class Motobug : public Enemy {
public:
    Motobug(): Enemy("Data/motobug1.png",40, 30,  1.f, 100)
    {

    }
};

/*/BeeBot: zig - zags and shoots every 5s(HP = 5)
class BeeBot : public Enemy {
public:
    BeeBot()
        : Enemy("Data/BeeBot.png", 64, 64, 1.f, 100)
    {
    }
};

class CrabMeat : public Enemy {
public:
    CrabMeat()  : Enemy("Data/CrabMeat.png", 64, 64,1.f, 100)
    {
    }
};*/
