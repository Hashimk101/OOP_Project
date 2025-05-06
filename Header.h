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

    void updateTextureRectForCurrentIndex() {

        int frameWidth = SpriteTex[currentIndex].T.getSize().x / SpriteTex[currentIndex].frameNum;
        int frameHeight = SpriteTex[currentIndex].T.getSize().y;

        // Update the SpriteRect
        SpriteRect.width = frameWidth;
        SpriteRect.height = frameHeight;
        ESprite.setTextureRect(SpriteRect);
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
                if (lvl[worldY][worldX] == 'w' || lvl[worldY][worldX] == 'b') {
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
                updateTextureRectForCurrentIndex();
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
                if (lvl[worldY][worldX] == 'w' || lvl[worldY][worldX] == 'b') {
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
                updateTextureRectForCurrentIndex();
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
                updateTextureRectForCurrentIndex();
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
            updateTextureRectForCurrentIndex();
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
            updateTextureRectForCurrentIndex();
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

        collision = (bottom_left == 'w' || bottom_right == 'w' || bottom_mid == 'w'
            || bottom_left == 'b' || bottom_right == 'b' || bottom_mid == 'b');

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
        std::cout << dmg << std::endl;
        if (dmg != 0) {
            hp -= dmg;
            isInvincible = true;

            invClock.restart();
            ESprite.setColor(sf::Color(255, 255, 255, 200));

        }
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

        // Debug hitbox
        sf::RectangleShape hitbox(sf::Vector2f(Pwidth, Pheight));
        hitbox.setPosition(player_x + hit_box_factor_x, player_y + hit_box_factor_y);
        hitbox.setFillColor(sf::Color::Transparent);
        hitbox.setOutlineColor(sf::Color::Blue);
        hitbox.setOutlineThickness(3);
        window.draw(hitbox);
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

        }
        else
        {
            SpriteTex[0].frameNum = 1;
        }
        if (!SpriteTex[1].T.loadFromFile("Data/0jog_left.png"))
        {
            std::cout << "Failed to load 0jog_left.png!" << std::endl;

        }
        else
        {
            SpriteTex[1].frameNum = 10;
        }
        if (!SpriteTex[2].T.loadFromFile("Data/0right_still.png")) {
            std::cout << "Failed to load 0right_still.png!" << std::endl;

        }
        else
        {
            SpriteTex[2].frameNum = 1;
        }
        if (!SpriteTex[3].T.loadFromFile("Data/0jog_right.png")) {
            std::cout << "Failed to load 0jog_right.png!" << std::endl;

        }
        else
        {
            SpriteTex[3].frameNum = 10;
        }
        if (!SpriteTex[4].T.loadFromFile("Data/0upL.png")) {
            std::cout << "Failed to load 0upL.png!" << std::endl;

        }
        else
        {
            SpriteTex[4].frameNum = 8;
        }
        if (!SpriteTex[5].T.loadFromFile("Data/0upR.png")) {
            std::cout << "Failed to load 0upR.png!" << std::endl;

        }
        else
        {
            SpriteTex[5].frameNum = 8;
        }
        if (!SpriteTex[6].T.loadFromFile("Data/0edgeL.png"))
        {
            std::cout << "Failed to load0edgeL .png!" << std::endl;

        }
        else
        {
            SpriteTex[6].frameNum = 7;
        }
        if (!SpriteTex[7].T.loadFromFile("Data/0edgeR.png")) {
            std::cout << "Failed to load 0edgeR.png!" << std::endl;

        }
        else
        {
            SpriteTex[7].frameNum = 8;
        }
        if (!SpriteTex[8].T.loadFromFile("Data/Sonic_Injured.png")) {
            std::cout << "Failed to load Sonic_Injured.png!" << std::endl;

        }
        else
        {
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

    void AnimateSprite(bool isMoving)override
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

        }
        else
        {
            SpriteTex[0].frameNum = 1;
        }
        //LEFT RUN
        if (!SpriteTex[1].T.loadFromFile("Data/Knuckles_Jog_Walk _L.png")) { //493x40 => 10 frames
            std::cout << "Failed to load Knuckles_Jog_Walk _L.png!" << std::endl;

        }
        else
        {
            SpriteTex[1].frameNum = 10;
        }
        //   IDLE
        if (!SpriteTex[2].T.loadFromFile("Data/Knuckles_Idle_R.png")) {//40x40 => 1 frame
            std::cout << "Failed to load Knuckles_Idle_R.png!" << std::endl;

        }
        else
        {
            SpriteTex[2].frameNum = 1;
        }
        //RIGHT RUN
        if (!SpriteTex[3].T.loadFromFile("Data/Knuckles_Jog_Walk_R.png")) //493x40 => 10 frames
        {
            std::cout << "Failed to load Knuckles_Jog_Walk_R.png!" << std::endl;

        }
        else
        {
            SpriteTex[3].frameNum = 10;
        }
        //ROLL LEFT
        if (!SpriteTex[4].T.loadFromFile("Data/Knuckles_0up_L.png")) //492x40 => 10 frames
        {
            std::cout << "Failed to load 0upL.png!" << std::endl;

        }
        else
        {
            SpriteTex[4].frameNum = 10;
        }
        //ROLL RIGHT
        if (!SpriteTex[5].T.loadFromFile("Data/Knuckles_0up_R.png"))//492x40 => 10 frames
        {
            std::cout << "Failed to load 0upR.png!" << std::endl;

        }
        else
        {
            SpriteTex[5].frameNum = 10;
        }
        
        if (!SpriteTex[6].T.loadFromFile("Data/Knuckles_Tripping_L.png")) //240x40=> 6 frames
        {
            std::cout << "Failed to load Knuckles_Tripping_L.png!" << std::endl;

        }
        else
        {
            SpriteTex[6].frameNum = 6;
        }
       
        if (!SpriteTex[7].T.loadFromFile("Data/Knuckles_Tripping_R.png")) //240x40=> 6 frames
        {
            std::cout << "Failed to load Knuckles_Tripping_R.png!" << std::endl;

        }
        else
        {
            SpriteTex[7].frameNum = 6;
        }

        //Injured aa 
        if (!SpriteTex[8].T.loadFromFile("Data/Knuckles_Tripping_L.png"))
        {
            std::cout << "Failed to load Knuckles_Tripping_L.png!" << std::endl;

        }
        else
        {
            SpriteTex[8].frameNum = 6;
        }
        //PUNCH RIGHT
        if (!SpriteTex[9].T.loadFromFile("Data/Knuckles_Punch_R.png")) //850x40 => 17frames
        {
            std::cout << "Failed to load Knuckles_Punch_R.png!" << std::endl;

        }
        else
        {
            SpriteTex[9].frameNum = 17;
        }
        //PUNCH LEFT
        if (!SpriteTex[10].T.loadFromFile("Data/Knuckles_Punch_L.png")) // 850x40 = > 17frames
        {

            std::cout << "Failed to load Knuckles_Punch_L.png!" << std::endl;
        }
        else
        {
            SpriteTex[10].frameNum = 17;
        }
        //HANG TO TAILS
        if (!SpriteTex[11].T.loadFromFile("Data/Knuckles_Hang.png")) //966x50 =>20 frames
        {

            std::cout << "Failed to load Knuckles_Hang.png!" << std::endl;
        }
        else
        {
            SpriteTex[11].frameNum = 20;
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

        ESprite.setTexture(SpriteTex[0].T);
        ESprite.setTextureRect(SpriteRect);
        ESprite.setPosition(player_x, player_y);
        ESprite.setScale(scale_x, scale_y);



    }

    bool movement(char** lvl) override {
        std::cout << player_x + offset_x << " " << player_y << std::endl;
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
        /*std::cout << gravity << std::endl;
        std::cout << velocityX << " " << velocityY << std::endl;*/

        return isMoving;
    }

  
    void punching(char** lvl)
    {
        bool Punch = false;
        //std::cout << ((player_x + hit_box_factor_x + Pwidth + offset_x + 15) / cell_size) << std::endl;
        if (Keyboard::isKeyPressed(Keyboard::T)) 
        {
            Punch = true;
            int targetXRight = (player_x + hit_box_factor_x + Pwidth + offset_x + 15) / cell_size;
            int targetYRight = (player_y + hit_box_factor_y) / cell_size;
            int targetXLeft = (player_x + hit_box_factor_x - Pwidth + offset_x) / cell_size;
            int targetYLeft = (player_y + hit_box_factor_y) / cell_size;
           
            if (lvl[targetYRight][targetXRight] == 'b')
            {
                lvl[targetYRight][targetXRight] = 's';
              
            }
            if (lvl[targetYRight + 1][targetXRight] == 'b')
            {
                lvl[targetYRight + 1][targetXRight] = 's';
                
            }
            if (lvl[targetYLeft][targetXLeft] == 'b')
            {
                lvl[targetYLeft][targetXLeft] = 's';
                
            }
            if (lvl[targetYLeft + 1][targetXLeft] == 'b')
            {
                lvl[targetYLeft + 1][targetXLeft] = 's';
                
            }
            if (left && Punch)
            {
                ESprite.setTexture(SpriteTex[10].T);
                currentIndex = 10;
                updateTextureRectForCurrentIndex();
                AnimateSprite(true);
            }
            else if (Punch)
            {
                ESprite.setTexture(SpriteTex[9].T);
                currentIndex = 9;
                updateTextureRectForCurrentIndex();
                AnimateSprite(true);
            }
        }

        Punch = false;
    }
    


    void AnimateSprite(bool isMoving)override
    {
        if (isMoving) {
            
            if (animationClock.getElapsedTime().asMilliseconds() > 80)
            {
                if (currentIndex == 1 || currentIndex == 3) 
                {
					ESprite.setScale(3.5, 3.5);
                }
                else
                {
                    ESprite.setScale(2.75, 2.75);
                }
                if (currentIndex == 9) 
                {
                    currentFrame = SpriteTex[9].frameNum;
                    
                    currentFrame = (currentFrame - 1 + SpriteTex[currentIndex].frameNum) % SpriteTex[currentIndex].frameNum;
                    SpriteRect.left = currentFrame * SpriteRect.width;
                    ESprite.setTextureRect(SpriteRect);
                    animationClock.restart();
                }
                else
                {
                    currentFrame = (currentFrame + 1) % SpriteTex[currentIndex].frameNum;
                    SpriteRect.left = currentFrame * SpriteRect.width;
                    ESprite.setTextureRect(SpriteRect);
                    animationClock.restart();
                }

            }
        }
        else {
            // reset to first frame of current strip
            currentFrame = 0;
            SpriteRect.left = 0;
            ESprite.setTextureRect(SpriteRect);
            ESprite.setScale(2.75, 2.75);
        }
        
    }



};

class Tails : public MySprite
{
private:
    bool isFlying;

public:
    Tails() : MySprite()
    {
        SpriteTex = new SpriteSheet[11];
        // Load texture for idle left
		if (!SpriteTex[0].T.loadFromFile("Data/Tails_Idle_L.png")) { //40x40 => 1 frame
			std::cout << "Failed to load Tails_Idle_L.png!" << std::endl;
		}
		else
		{
			SpriteTex[0].frameNum = 1;
		}
        // Load texture for jog/walk left
		if (!SpriteTex[1].T.loadFromFile("Data/Tails_Jog_L.png"))
		{
			std::cout << "Failed to load Tails_Jog_L.png!" << std::endl;
		}
		else
		{
			SpriteTex[1].frameNum = 10;
		}
        // Load texture for idle right
        if (!SpriteTex[2].T.loadFromFile("Data/Tails_Idle_R.png")) { //40x40 => 1 frame
            std::cout << "Failed to load Tails_Idle_R.png!" << std::endl;
        }
        else
        {
            SpriteTex[2].frameNum = 1;
        }
        // Load texture for jog/walk right
		if (!SpriteTex[3].T.loadFromFile("Data/Tails_Jog_R.png"))
		{
			std::cout << "Failed to load Tails_Jog_R.png!" << std::endl;
		}
		else
		{
			SpriteTex[3].frameNum = 10;
		}
        // Load texture for jump left 
        if (!SpriteTex[4].T.loadFromFile("Data/Tail_Jump"))
        {
            std::cout << "Failed to load Tail_Jump.png!" << std::endl;
			
        }
        else
        {
            SpriteTex[4].frameNum = 6;
        }
        // Load texture for jump right 
        if (!SpriteTex[5].T.loadFromFile("Data/Tail_Jump"))
        {
            std::cout << "Failed to load Tail_Jump.png!" << std::endl;
           
        }
        else
        {
            SpriteTex[5].frameNum = 6;
        }
         // Load texture for edge left
        if (!SpriteTex[6].T.loadFromFile("Data/Tails_Edge_L.png")) 
        {
			std::cout << "Failed to load Tails_Edge_L.png!" << std::endl;
		}
		else
		{
			SpriteTex[6].frameNum = 8;

        }
        // Load texture for edge right
        if (!SpriteTex[7].T.loadFromFile("Data/Tails_Edge_R.png"))
        {

            std::cout << "Failed to load =Tails_Edge_R.png!" << std::endl;
        }
        else
        {
            SpriteTex[7].frameNum = 8;

        }
        // Load texture for fly left
		if (!SpriteTex[8].T.loadFromFile("Data/Tails_Fly_L.png"))
		{
			std::cout << "Failed to load Tails_Fly_L.png!" << std::endl;
		}
		else
		{
			SpriteTex[8].frameNum = 4;
		}
        // Load texture for fly right
        if (!SpriteTex[9].T.loadFromFile("Data/Tails_Fly_R.png"))
        {
            std::cout << "Failed to load Tails_Fly_R.png!" << std::endl;
        }
        else
        {
            SpriteTex[9].frameNum = 4;
        }

        // Load texture for injured
        if (!SpriteTex[10].T.loadFromFile("Data/Tails_Hurt.png"))
        {
            std::cout << "Failed to load Tails_Hurt.png!" << std::endl;
        }
        else
        {
            SpriteTex[9].frameNum = 5;
        }

        // Initialize variables
        velocityY = 0;
        velocityX = 0;
        player_x = 100;
        player_y = 100;
        window_x = 0;
        window_y = 0;
        acceleration = 1.1;
        max_speed = 8;
        hp = 90;
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
        isFlying = false;

        // Set up sprite rectangle
        SpriteRect = IntRect(0, 0, 40, 40);
        ESprite.setTextureRect(SpriteRect);

        // Set up Tails sprite
        // ESprite.setTexture(SpriteTex[0].T); // Set to idle left
        ESprite.setTextureRect(SpriteRect);
        ESprite.setPosition(player_x, player_y);
        ESprite.setScale(scale_x, scale_y);
    }
};



