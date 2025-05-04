#include <iostream>
//#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "Maps.h"

using namespace sf;
int cell_size = 64;
struct SpriteSheet
{
    Texture T;
    int frameNum;
};

class MySprite
{ // Renamed to avoid conflict with sf::Sprite
protected:
    SpriteSheet* SpriteTex; // Array of textures for different animations

    float velocityY, velocityX, player_x, player_y, max_speed, acceleration;
    float offset_x, offset_y, terminal_Velocity;
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
    IntRect SpriteRect; // For texture rectangle animation
    int currentIndex; // Tracks which texture is in use
    int currentFrame; // Tracks animation frame
    sf::Clock animationClock; // For animation timing
    bool left; // Tracks direction
    // Sprites for different characters
    sf::Sprite ESprite;
    bool OnEdge = false; // Tracks if the player is on an edge


public:
    // Constructor to initialize the sprite with the texture
    MySprite()
    {

    }

    // Function to get the sprite (for rendering, etc.)
    sf::Sprite& getSprite() {
        return ESprite;
    }

    bool isOnEdge(char** lvl)
    {
        if (!onGround) return false;


        int footLeftX = (offset_x + player_x + hit_box_factor_x) / cell_size;
        int footRightX = (offset_x + player_x + hit_box_factor_x + Pwidth) / cell_size;
        int footY = (offset_y + hit_box_factor_y + Pheight + 1) / cell_size;

        // guard against OOB
        footY = std::min(std::max(footY, 0), 13);

        bool groundLeft = lvl[footY][footLeftX] == 'w';
        bool groundRight = lvl[footY][footRightX] == 'w';

        // edge = exactly one side has ground

        OnEdge = groundLeft ^ groundRight;
        return OnEdge;
    }

    virtual bool movement(char** lvl) {
        borderCheck();
        bool isMoving = false;
        //std::cout << velocityX << std::endl;

        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            // Collision checks go brrrrr
            bool leftCollision = false;

            // Check multiple points along the left side of the character
            for (int checkY = 0; checkY < Pheight; checkY += cell_size / 3)
            {
                // Limit to within character height
                if (checkY > Pheight - hit_box_factor_y) checkY = Pheight - hit_box_factor_y;

                // Calculate the position to check (left side of character)
                int worldY = (offset_y + hit_box_factor_y + checkY) / cell_size;
                int worldX = (offset_x + player_x + hit_box_factor_x - 17) / cell_size; // Check slightly ahead

                // Stay within bounds
                if (worldY < 0) worldY = 0;

                // Check if there's a wall
                if (lvl[worldY][worldX] == 'w') {
                    leftCollision = true;
                    break;
                }
                else if (lvl[worldY][worldX] == 'k') {
                    leftCollision = true;
                    takeDamage(5);
                    break;
                }
            }

            if (leftCollision)
            {
                velocityX = 0;
            }
            else
            {
                // Apply acceleration :(
                //if (velocityX > 0) velocityX = 0; // Reset velocity if moving right
                velocityX -= acceleration;
                if (velocityX < -max_speed) velocityX = -max_speed;

                // Animation
                ESprite.setTexture(SpriteTex[1].T);
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
        else if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            // Collision checks go brrrrr
            bool rightCollision = false;

            // Check multiple points along the right side of the character
            for (int checkY = 0; checkY < Pheight; checkY += cell_size / 2) {
                // Limit to within character height
                if (checkY > Pheight - hit_box_factor_y) checkY = Pheight - hit_box_factor_y;

                // Calculate the position to check (right side of character)
                int worldY = (offset_y + hit_box_factor_y + checkY) / cell_size;
                int worldX = (offset_x + player_x + hit_box_factor_x + Pwidth + 15) / cell_size; // Check slightly ahead

                // Stay within bounds
                if (worldY < 0) worldY = 0;

                // Check if there's a wall
                if (lvl[worldY][worldX] == 'w') {
                    rightCollision = true;
                    break;
                }
                else if (lvl[worldY][worldX] == 'k') {
                    rightCollision = true;
                    takeDamage(5);
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
                    ESprite.setTexture(SpriteTex[3].T);
                }
                else {
                    currentIndex = 5;
                    ESprite.setTexture(SpriteTex[5].T);
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
            if (isOnEdge(lvl)) {
                // choose left or right edge pose
                currentIndex = left ? 6 : 7;            // 6 = edgeL strip, 7 = edgeR strip
                ESprite.setTexture(SpriteTex[currentIndex].T);
                AnimateSprite(true);                    // cycle through its frames
                return true;
            }

            currentIndex = (currentIndex == 1) ? 0 : 2;
            if (left) {
                ESprite.setTexture(SpriteTex[0].T);
            }
            else {
                ESprite.setTexture(SpriteTex[2].T);
            }
            isMoving = false;
            AnimateSprite(isMoving); // Moved here to ensure animation updates when not moving
        }

        if (onGround) {
            if ((Keyboard::isKeyPressed(Keyboard::Up)) || (Keyboard::isKeyPressed(Keyboard::Space))) {
                velocityY = -20;
            }
        }
        else if (!onGround) {
            if ((currentIndex == 0 || currentIndex == 1)) {
                currentIndex = 4;
                ESprite.setTexture(SpriteTex[4].T);
            }
            else if ((currentIndex == 2 || currentIndex == 3)) {
                currentIndex = 5;
                ESprite.setTexture(SpriteTex[5].T);
            }
            isMoving = true;

        }


        if (isInvincible)
        {
            velocityY = -2;
            velocityX = -3;
            currentIndex = 8;
            ESprite.setTexture(SpriteTex[8].T);
            AnimateSprite(isInvincible);
        }

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

        //std::cout << player_y << std::endl;
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
            if (velocityY > terminal_Velocity)
            {
                velocityY = terminal_Velocity;
            }
        }
    }
    //CALCULATE THE NUMBER OF FRAMES IN THE TEXTURE
    int GetFrameWidth(SpriteSheet& S)
    {
        Vector2u  size = S.T.getSize();
        int width = ceil(size.x / S.frameNum);
        return width;

    }
    int GetFrameNum(Texture& Txt)
    {
        Vector2u size = Txt.getSize();
        int frameNum = size.x / 40;
        return frameNum;
    }

   virtual  void AnimateSprite(bool isMoving) {

        if (isMoving) {
            if (animationClock.getElapsedTime().asMilliseconds() > 80) {
                currentFrame = (currentFrame + 1) % SpriteTex[currentIndex].frameNum;
                SpriteRect.left = currentFrame * SpriteRect.width;
                ESprite.setTextureRect(SpriteRect);
                animationClock.restart();
            }
        }
        else {
            // reset to first frame of current strip
            currentFrame = 0;
            SpriteRect.left = 0;
            ESprite.setTextureRect(SpriteRect);
        }
    }

    void takeDamage(int dmg)
    {
        if (isInvincible)
        {

            return;                // ignore if still invincible

        }

        hp -= dmg;
        isInvincible = true;

        invClock.restart();
        ESprite.setColor(sf::Color(255, 255, 255, 200));


        if (hp <= 0) {

            return;
        }


    }

    void borderCheck()
    {
        if (player_y < 32) {
            player_y = 42;
        }
    }


    void update()
    {
        // clear invincibility after duration
        if (isInvincible && invClock.getElapsedTime().asSeconds() >= invDuration)
        {
            isInvincible = false;
            ESprite.setColor(sf::Color(255, 255, 255, 255));
        }

     
    }

   

    int getX() const {
        return player_x;
    }
    int getY() const {
        return player_y;
    }
    void draw_player(RenderWindow& window) {

        ESprite.setPosition(player_x, player_y);
        window.draw(ESprite);
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
    int getVelocityY() const {
        return velocityY;
    }
};
class Sonic : public MySprite
{
private:
    bool isGliding;

public:
    Sonic() : MySprite()
    {
        SpriteTex = new SpriteSheet[9];
        // Load textures into the array
        if (!SpriteTex[0].T.loadFromFile("Data/0left_still.png"))
        {
            std::cout << "Failed to load 0left_still.png!" << std::endl;
            SpriteTex[0].frameNum = 1;
        }
        if (!SpriteTex[1].T.loadFromFile("Data/0jog_left.png"))
        {
            std::cout << "Failed to load 0jog_left.png!" << std::endl;
            SpriteTex[1].frameNum = 10;
        }
        if (!SpriteTex[2].T.loadFromFile("Data/0right_still.png")) {
            std::cout << "Failed to load 0right_still.png!" << std::endl;
            SpriteTex[2].frameNum = 1;
        }
        if (!SpriteTex[3].T.loadFromFile("Data/0jog_right.png")) {
            std::cout << "Failed to load 0jog_right.png!" << std::endl;
            SpriteTex[3].frameNum = 10;
        }
        if (!SpriteTex[4].T.loadFromFile("Data/0upL.png")) {
            std::cout << "Failed to load 0upL.png!" << std::endl;
            SpriteTex[4].frameNum = 8;
        }
        if (!SpriteTex[5].T.loadFromFile("Data/0upR.png")) {
            std::cout << "Failed to load 0upR.png!" << std::endl;
            SpriteTex[5].frameNum = 8;
        }
        if (!SpriteTex[6].T.loadFromFile("Data/0edgeL.png"))
        {
            std::cout << "Failed to load0edgeL .png!" << std::endl;
            SpriteTex[6].frameNum = 7;
        }
        if (!SpriteTex[7].T.loadFromFile("Data/0edgeR.png")) {
            std::cout << "Failed to load 0edgeR.png!" << std::endl;
            SpriteTex[7].frameNum = 8;
        }
        if (!SpriteTex[8].T.loadFromFile("Data/Sonic_Injured.png")) {
            std::cout << "Failed to load Sonic_Injured.png!" << std::endl;
            SpriteTex[8].frameNum = 6;
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
        friction = 0.80;
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
        isGliding = false;

        // after you set raw_img_x = 24; raw_img_y = 35;  (or whatever your actual frame size is)
        SpriteRect = IntRect(0, 0, 40, 40);
        ESprite.setTextureRect(SpriteRect);


        // Set up SonicSprite
        ESprite.setTexture(SpriteTex[0].T);
        ESprite.setTextureRect(SpriteRect);
        ESprite.setPosition(player_x, player_y);
        ESprite.setScale(scale_x, scale_y);
    }

    void AnimateSprite( bool isMoving)override
    {
        if (isMoving) {
            if (animationClock.getElapsedTime().asMilliseconds() > 80) {
                currentFrame = (currentFrame + 1) % SpriteTex[currentIndex].frameNum;
                SpriteRect.left = currentFrame * SpriteRect.width;
                ESprite.setTextureRect(SpriteRect);
                animationClock.restart();
            }
        }
        else {
            // reset to first frame of current strip
            currentFrame = 0;
            SpriteRect.left = 0;
            ESprite.setTextureRect(SpriteRect);
        }
    }

 
};

class Knuckles : public MySprite
{
private:
    bool isGliding;
public:
    Knuckles() : MySprite()
    {

        SpriteTex = new SpriteSheet[12];
      //  IDLE
        if (!SpriteTex[0].T.loadFromFile("Data/Knuckles_Idle_L.png")) { //40x40 => 1 frame
            std::cout << "Failed to load Knuckles_Idle_L.png!" << std::endl;
            SpriteTex[0].frameNum = 1;
        }
        //LEFT RUN
        if (!SpriteTex[1].T.loadFromFile("Data/Knuckles_Jog_Walk _L.png")) { //493x40 => 10 frames
            std::cout << "Failed to load Knuckles_Jog_Walk _L.png!" << std::endl;
            SpriteTex[1].frameNum = 10;
        }
      //   IDLE
        if (!SpriteTex[2].T.loadFromFile("Data/Knuckles_Idle_R.png")) {//40x40 => 1 frame
            std::cout << "Failed to load Knuckles_Idle_R.png!" << std::endl;
            SpriteTex[2].frameNum = 1;
        }
        //RIGHT RUN
        if (!SpriteTex[3].T.loadFromFile("Data/Knuckles_Jog_Walk_R.png")) //493x40 => 10 frames
        {
            std::cout << "Failed to load Knuckles_Jog_Walk_R.png!" << std::endl;
            SpriteTex[3].frameNum = 10;
        }
        //ROLL LEFT
        if (!SpriteTex[4].T.loadFromFile("Data/Knuckles_0up_L.png")) //492x40 => 10 frames
        {
            std::cout << "Failed to load 0upL.png!" << std::endl;
            SpriteTex[4].frameNum = 10;
        }
        //ROLL RIGHT
        if (!SpriteTex[5].T.loadFromFile("Data/Knuckles_0up_R.png"))//492x40 => 10 frames
        {
            std::cout << "Failed to load 0upR.png!" << std::endl;
            SpriteTex[5].frameNum = 10;
        }
        //PUNCH LEFT
        if (!SpriteTex[6].T.loadFromFile("Data/Knuckles_Punch_L.png")) //850x40 => 17frames
        {
            std::cout << "Failed to load Knuckles_Punch_L.png!" << std::endl;
            SpriteTex[6].frameNum = 17;
        }
        //PUNCH RIGHT
        if (!SpriteTex[7].T.loadFromFile("Data/Knuckles_Punch_R.png"))//850x40 => 17frames
        {
            std::cout << "Failed to load Knuckles_Punch_R.png!" << std::endl;
            SpriteTex[7].frameNum = 17;
        }
        //CLIMB UP
        if (!SpriteTex[8].T.loadFromFile("Data/Knuckles_climb_Up.png")) //575x42 =>12 frames
        {
            std::cout << "Failed to load Knuckles_climb_Up.png!" << std::endl;
            SpriteTex[8].frameNum = 12;
        }
        //CLIMB DOWN
        if (!SpriteTex[9].T.loadFromFile("Data/Knuckles_climb_R.png")) //75x42 =>2 frames
        {
            std::cout << "Failed to load Knuckles_climb_R.png!" << std::endl;
            SpriteTex[9].frameNum = 2;
        }
        //GLIDE IN MID AIR
        if (!SpriteTex[10].T.loadFromFile("Data/Knuckles_Glide.png")) //517x46 =>11 frames
        {
            SpriteTex[10].frameNum = 11;
            std::cout << "Failed to load Knuckles_Glide.png!" << std::endl;
        }
        //HANG TO TAILS
        if (!SpriteTex[11].T.loadFromFile("Data/Knuckles_Hang.png")) //966x50 =>20 frames
        {
            SpriteTex[11].frameNum = 20;
            std::cout << "Failed to load Knuckles_Hang.png!" << std::endl;
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
        raw_img_x = 40;
        raw_img_y = 40;
        Pheight = raw_img_y * scale_y;
        Pwidth = raw_img_x * scale_x;
        hit_box_factor_x = 8 * scale_x;
        hit_box_factor_y = 5 * scale_y;
        offset_x = 0;
        offset_y = 0;
        isGliding = false;

        // after you set raw_img_x = 24; raw_img_y = 35;  (or whatever your actual frame size is)
        SpriteRect = IntRect(0, 0, 40, 40);
        ESprite.setTextureRect(SpriteRect);


        // Set up SonicSprite
        ESprite.setTexture(SpriteTex[0].T);
        ESprite.setTextureRect(SpriteRect);
        ESprite.setPosition(player_x, player_y);
        ESprite.setScale(scale_x, scale_y);



    }

    bool movement(char** lvl) override {
        bool isMoving = MySprite::movement(lvl);
        if (!isGliding && Keyboard::isKeyPressed(Keyboard::F)) {
            //velocityX = velocityX > 0 ? +10 : -10;
            velocityY = -3;

            isGliding = true;
        }
        if (isGliding) {
            gravity = 0.15;
        }

        if (onGround) {
            isGliding = false;
            gravity = 1;
        }
        //std::cout << gravity << std::endl;
        //std::cout << velocityX << " " << velocityY << std::endl;

        return isMoving;
    }

};



