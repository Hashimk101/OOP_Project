#include <iostream>
//#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "Maps.h"


using namespace sf;
int cell_size = 64;

class MySprite
{ // Renamed to avoid conflict with sf::Sprite
    Texture SonicTex[6]; // Array of textures for different animations
    Texture TailsTex, KnucklesTex; // Kept for potential future use
    int velocityY, velocityX, player_x, player_y, max_speed, acceleration;
    int offset_x, offset_y, terminal_Velocity;
    float scale_x, scale_y;
    int raw_img_x, raw_img_y;
    int Pheight, Pwidth;
    int hit_box_factor_x, hit_box_factor_y;
    int hp;
    bool     isInvincible = false;
    sf::Clock invClock;                     // tracks invincibility time
    float   invDuration = 1.0;
    float window_x, window_y;
    bool onGround;
    float gravity, friction;
    IntRect SonicRect; // For texture rectangle animation
    int currentIndex; // Tracks which texture is in use
    int currentFrame; // Tracks animation frame
    sf::Clock animationClock; // For animation timing
    bool left; // Tracks direction
    // Sprites for different characters
    sf::Sprite SonicSprite, TailsSprite, KnucklesSprite;


public:
    // Constructor to initialize the sprite with the texture
    MySprite() : SonicRect(0, 0, 40, 40), currentIndex(0), currentFrame(0), left(false) {
        // Load textures into the array
        if (!SonicTex[0].loadFromFile("Data/0left_still.png")) {
            std::cout << "Failed to load 0left_still.png!" << std::endl;
        }
        if (!SonicTex[1].loadFromFile("Data/0jog_left.png")) {
            std::cout << "Failed to load 0jog_left.png!" << std::endl;
        }
        if (!SonicTex[2].loadFromFile("Data/0right_still.png")) {
            std::cout << "Failed to load 0right_still.png!" << std::endl;
        }
        if (!SonicTex[3].loadFromFile("Data/0jog_right.png")) {
            std::cout << "Failed to load 0jog_right.png!" << std::endl;
        }
        if (!SonicTex[4].loadFromFile("Data/0upL.png")) {
            std::cout << "Failed to load 0upL.png!" << std::endl;
        }
        if (!SonicTex[5].loadFromFile("Data/0upR.png")) {
            std::cout << "Failed to load 0upR.png!" << std::endl;
        }

        // Initialize variables
        velocityY = 0;
        velocityX = 0;
        player_x = 100;
        player_y = 100;
        window_x = 0; // Note: There was a duplicate assignment in the original
        window_y = 0; // Corrected to window_y
        acceleration = 1.1;
        max_speed = 12;
        hp = 100;
        isInvincible = false;
        onGround = false;
        gravity = 1;
        friction = 0.85;
        terminal_Velocity = 20;
        scale_x = 2.5;
        scale_y = 2.5;
        raw_img_x = 24;
        raw_img_y = 35;
        Pheight = raw_img_y * scale_y;
        Pwidth = raw_img_x * scale_x;
        hit_box_factor_x = 8 * scale_x;
        hit_box_factor_y = 5 * scale_y;
        offset_x = 0;
        offset_y = 0;


        // Set up SonicSprite
        SonicSprite.setTexture(SonicTex[0]);
        SonicSprite.setTextureRect(SonicRect);
        SonicSprite.setPosition(player_x, player_y);
        SonicSprite.setScale(scale_x, scale_y);
    }

    // Function to get the sprite (for rendering, etc.)
    sf::Sprite& getSprite() {
        return SonicSprite;
    }

    bool movement(char** lvl) {
        borderCheck();
        bool isMoving = false;
        //std::cout << velocityX << std::endl;

        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            // Collision checks go brrrrr
            bool leftCollision = false;

            // Check multiple points along the left side of the character
            for (int checkY = 0; checkY < Pheight; checkY += cell_size / 3) {
                // Limit to within character height
                if (checkY > Pheight - hit_box_factor_y) checkY = Pheight - hit_box_factor_y;

                // Calculate the position to check (left side of character)
                int worldY = (offset_y + hit_box_factor_y + checkY) / cell_size;
                int worldX = (offset_x + player_x + hit_box_factor_x - 5) / cell_size; // Check slightly ahead

                // Stay within bounds
                if (worldY < 0) worldY = 0;

                // Check if there's a wall
                if (lvl[worldY][worldX] == 'w') {
                    leftCollision = true;
                    break;
                }
            }

            if (leftCollision) {
                velocityX = 0;
            }
            else {
                // Apply acceleration :(
                //if (velocityX > 0) velocityX = 0; // Reset velocity if moving right
                velocityX -= acceleration;
                if (velocityX < -max_speed) velocityX = -max_speed;

                // Animation
                SonicSprite.setTexture(SonicTex[1]);
                currentIndex = 1;
                isMoving = true;
                left = true;
                AnimateSprite(isMoving);

                // Movement logic lol pata nai kaise ban gai
                if (player_x <= 450) {
                    offset_x += velocityX; // Scroll world (velocityX is negative)
                    if (offset_x < 0) {
                        player_x += velocityX;
                        if (player_x < 5) player_x = 5;
                        offset_x = 0;
                    }
                }
                else {
                    player_x += velocityX;
                }
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right)) {
            // Collision checks go brrrrr
            bool rightCollision = false;

            // Check multiple points along the right side of the character
            for (int checkY = 0; checkY < Pheight; checkY += cell_size / 2) {
                // Limit to within character height
                if (checkY > Pheight - hit_box_factor_y) checkY = Pheight - hit_box_factor_y;

                // Calculate the position to check (right side of character)
                int worldY = (offset_y + hit_box_factor_y + checkY) / cell_size;
                int worldX = (offset_x + player_x + hit_box_factor_x + Pwidth + 10) / cell_size; // Check slightly ahead

                // Stay within bounds
                if (worldY < 0) worldY = 0;

                // Check if there's a wall
                if (lvl[worldY][worldX] == 'w') {
                    rightCollision = true;
                    break;
                }
            }

            if (rightCollision) {
                velocityX = 0;
            }
            else {
                // Applying acceleration :)
                //if (velocityX < 0) velocityX = 0; // Reset velocity if moving left
                velocityX += acceleration;
                if (velocityX > max_speed) velocityX = max_speed;

                // Animation be like ......
                if (onGround) {
                    currentIndex = 3;
                    SonicSprite.setTexture(SonicTex[3]);
                }
                else {
                    currentIndex = 5;
                    SonicSprite.setTexture(SonicTex[5]);
                }
                isMoving = true;
                left = false;
                AnimateSprite(isMoving);

                // Movement logic
                if (player_x >= 750) {
                    offset_x += velocityX; //velocity is positive so no need to apply any negative
                    if (offset_x > 11600) {
                        player_x += velocityX;
                        if (player_x > 1100) player_x = 1100;
                        offset_x = 11600;
                    }
                }
                else {
                    player_x += velocityX;
                }
            }
        }
        else {
            // Apply friction when no key is pressed
            velocityX *= friction;
            if (player_x >= 450 && player_x <= 750) {
                player_x += velocityX;
            }
            // same logic of window movement, but now in friction too ;(
            else {
                offset_x += velocityX;
                if (offset_x < 0) {
                    player_x += velocityX;
                    if (player_x < 5) player_x = 5;
                    offset_x = 0;
                }
                if (offset_x > 11600) {
                    player_x += velocityX;
                    if (player_x > 1100) player_x = 1100;
                    offset_x = 11600;

                }
            }
            // Only zero out very small velocities
            if (std::abs(velocityX) < 0.1f) {
                velocityX = 0;
            }
            currentIndex = (currentIndex == 1) ? 0 : 2;
            if (left) {
                SonicSprite.setTexture(SonicTex[0]);
            }
            else {
                SonicSprite.setTexture(SonicTex[2]);
            }
            isMoving = false;
            AnimateSprite(isMoving); // Moved here to ensure animation updates when not moving
        }

        if (onGround) {
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                velocityY = -21;
            }
        }
        else if (!onGround) {
            if ((currentIndex == 0 || currentIndex == 1)) {
                currentIndex = 4;
                SonicSprite.setTexture(SonicTex[4]);
            }
            else if ((currentIndex == 2 || currentIndex == 3)) {
                currentIndex = 5;
                SonicSprite.setTexture(SonicTex[5]);
            }
            isMoving = true;

        }
        std::cout << "Player X: " << offset_x << " Player Y: " << offset_y << std::endl;
        return isMoving; // Always return isMoving
		
    }

    void player_gravity(char** lvl)
    {
        // Store previous position
        offset_y = player_y;

        // Apply vertical velocity
        offset_y += velocityY;

        int check_y = (int)((offset_y + hit_box_factor_y + Pheight) / cell_size); // gives the row of the lvl the character is currently on

        // Calculate x positions of collision points
        int left_x = (int)(((offset_x + player_x + hit_box_factor_x) / cell_size));
        int right_x = (int)(((offset_x + player_x + hit_box_factor_x + Pwidth) / cell_size));
        int mid_x = (int)(((offset_x + player_x + hit_box_factor_x + Pwidth / 2) / cell_size));

        // Check for ground collision
        bool collision = false;
        char bottom_left = lvl[check_y][left_x];
        char bottom_right = lvl[check_y][right_x];
        char bottom_mid = lvl[check_y][mid_x];

        collision = (bottom_left == 'w' || bottom_right == 'w' || bottom_mid == 'w');


        if (collision)
        {
            // Snap player to top of the collided tile
            player_y = check_y * cell_size - (hit_box_factor_y + Pheight);
            onGround = true;
            velocityY = 0;
            //std::cout << "On ground: " << player_y << std::endl;
        }
        else
        {
            player_y = offset_y;
            onGround = false;
            //std::cout << "Not on ground: " << player_y << std::endl;
            // Apply gravity if in air
            velocityY += gravity;
            if (velocityY > terminal_Velocity) {
                velocityY = terminal_Velocity;
            }
        }
    }


    //CALCULATE THE NUMBER OF FRAMES IN THE TEXTURE
    int GetFrameNum(Texture& Txt)
    {
        Vector2u size = Txt.getSize();
        int frameNum = size.x / 40;
        return frameNum;
    }

    void AnimateSprite(bool isMoving)
    {
        if (isMoving) {
            if (animationClock.getElapsedTime().asMilliseconds() > 80) {
                currentFrame = (currentFrame + 1) % GetFrameNum(SonicTex[currentIndex]);
                SonicRect.left = currentFrame * 40;
                SonicSprite.setTextureRect(SonicRect);
                animationClock.restart();
            }
        }
        else {

            currentFrame = 0;
            SonicRect.left = 0;

            SonicSprite.setTextureRect(SonicRect);
        }
    }
    void takeDamage(int dmg = 1) 
    {
        //std::cout << hp << std::endl;
        if (isInvincible)
            return;                // ignore if still invincible

        hp -= dmg;                  // subtract health (or rings)
                   

        if (hp <= 0) {
            
            return;
        }

        // enter invincibility state
        isInvincible = true;
        invClock.restart();

        // optional: tint sprite to show invulnerability
        SonicSprite.setColor(sf::Color(255, 255, 255, 128));
    }

    void update() 
    {
        // clear invincibility after duration
        if (isInvincible && invClock.getElapsedTime().asSeconds() >= invDuration)
        {
            isInvincible = false;
            SonicSprite.setColor(sf::Color(255, 255, 255, 255));
        }

        // … your other update logic (movement, animation, etc.) …
    }

    void borderCheck() 
    {
        if (player_y < 32) {
            player_y = 42;
        }
    }

    int getX() const {
        return player_x;
    }
    int getY() const {
        return player_y;
    }
    void draw_player(RenderWindow& window) {

        SonicSprite.setPosition(player_x, player_y);
        window.draw(SonicSprite);
    }
    int getOffsetX() const {
        return offset_x;
    }
    int getOffsetY() const {
        return offset_y;
    }
    int gethitX() const {
        return hit_box_factor_x;
    }
    int gethitY() const {
        return hit_box_factor_y;
    }
	bool getonGround() const {
		return onGround;
	}
};

