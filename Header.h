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
    static int hp;
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
    //bool isFlying = false;
    bool Punch = false;
    bool isFlying = false; // Tracks if the player is flying
    bool isMoving = false; // Tracks if the player is moving
    int totalWidth;
    bool InvspowerUp = false; // Tracks if the player has a power-up
    sf::Clock KnuInvClock;
    // Tracks if the player is Knuckles
    sf::Vector2f lastOnGround;
    float lastx;
    sf::Clock powerUPCLK;
    int power;
    float old;
public:
    bool isKnuckles = true;
    bool isSonic=true;
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

    virtual bool movement(char** lvl, bool check, bool check2)
    {
        //std::cout << velocityX <<  " " << velocityY << std::endl;
        /*if (check2) {
            return true;
        }*/
        borderCheck();
        bool isMoving = false;
        //std::cout << velocityX << std::endl;
        if (Punch) {
            return true;
        }

        if (isFlying && left)
        {
            currentIndex = 10;
            isMoving = true;
            // AnimateSprite(true);

        }
        else if (isFlying && !left)

        {
            currentIndex = 9;
            isMoving = true;
            // AnimateSprite(true);

        }
        ESprite.setTexture(SpriteTex[currentIndex].T);
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            left = true;
            // Collision checks go brrrrr
            bool leftCollision = false;
            // Check multiple points along the left side of the character
                // Limit to within character height

                // Calculate the position to check (left side of character)
                int worldY = (player_y + hit_box_factor_y) / cell_size;
                int worldX = (offset_x + player_x + hit_box_factor_x - 17) / cell_size; // Check slightly ahead

                // Stay within bounds
                if (worldY < 0) worldY = 0;

                // Check if there's a wall
                if (worldY >= 0 && worldY < 14 && worldX >= 0 && worldX < totalWidth) {
                    if (lvl[worldY][worldX] == 'w' || lvl[worldY][worldX] == 'b') {
                        leftCollision = true;
                    }
                    if (lvl[worldY+1][worldX] == 'w' || lvl[worldY+1][worldX] == 'b') {
                        leftCollision = true;
                    }
                }
                if (lvl[worldY+1][worldX] == 'k' || lvl[worldY+1][worldX+1] == 'k') {
                    leftCollision = true;
                    takeDamage(5);
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
                if (!isFlying)
                {
                    ESprite.setTexture(SpriteTex[1].T);
                    currentIndex = 1;
                    isMoving = true;
                    left = true;

                    updateTextureRectForCurrentIndex();
                    AnimateSprite(isMoving);
                }

                // Movement logic lol pata nai kaise ban gai
                if (player_x <= 450)
                {
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

                // Calculate the position to check (right side of character)
                int worldY = (player_y + hit_box_factor_y) / cell_size;
                int worldX = (offset_x + player_x + hit_box_factor_x + Pwidth + 15) / cell_size; // Check slightly ahead

                // Stay within bounds
                if (worldY < 0) worldY = 0;

                // Check if there's a wall
                if (worldY >= 0 && worldY < 14 && worldX >= 0 && worldX < totalWidth) {
                    if (lvl[worldY][worldX] == 'w' || lvl[worldY][worldX] == 'b') {
                        rightCollision = true;
                    }
                    if (lvl[worldY+ 1][worldX] == 'w' || lvl[worldY+1][worldX] == 'b') {
                        rightCollision = true;
                    }
                }
                 if (lvl[worldY+1][worldX] == 'k' || lvl[worldY+1][worldX+1] == 'k') {
                    rightCollision = true;
                    takeDamage(5);
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
                if (onGround && !isFlying)
                {
                    currentIndex = 3;
                    ESprite.setTexture(SpriteTex[3].T);
                }
                else if (!isFlying)
                {
                    currentIndex = 5;
                    ESprite.setTexture(SpriteTex[5].T);
                }
                isMoving = true;
                left = false;
                updateTextureRectForCurrentIndex();
                AnimateSprite(isMoving);

                // Movement logic
                if (player_x >= 750 && !check2) {
                    offset_x += velocityX; //velocity is positive so no need to apply any negative
                    if (offset_x > (totalWidth * 64 - 1200)) {
                        player_x += velocityX;
                        if (player_x > 1100) player_x = 1100;
                        offset_x = (totalWidth * 64 - 1200);
                    }
                }
                else {
                    player_x += velocityX;
                    if (player_x >= 1100) {
                        player_x = 1100;
                    }
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
                if (!check2) {
                    offset_x += velocityX;
                    if (offset_x < 0) {
                        player_x += velocityX;
                        if (player_x < 5) player_x = 5;
                        offset_x = 0;
                    }
                    if (offset_x > (totalWidth * 64 - 1200)) {
                        player_x += velocityX;
                        if (player_x > 1100) player_x = 1100;
                        offset_x = (totalWidth * 64 - 1200);

                    }
                }
                else {
                    player_x += velocityX;
                    if (player_x >= 1100) {
                        player_x = 1100;
                    }
                }
            }
            if (check2) {
                offset_x = 0;
            }
            // Only zero out very small velocities
            if (std::abs(velocityX) < 0.1f) {
                velocityX = 0;
            }
            if (isOnEdge(lvl))
            {
                // choose left or right edge pose
                currentIndex = left ? 6 : 7;            // 6 = edgeL strip, 7 = edgeR strip
                ESprite.setTexture(SpriteTex[currentIndex].T);
                updateTextureRectForCurrentIndex();
                AnimateSprite(true);                    // cycle through its frames
                return true;
            }
            if (!isFlying)

            {
                currentIndex = (currentIndex == 1) ? 0 : 2;
                if (left) {
                    ESprite.setTexture(SpriteTex[0].T);
                }
                else
                {
                    ESprite.setTexture(SpriteTex[2].T);
                }
                isMoving = false;

                updateTextureRectForCurrentIndex();
                AnimateSprite(isMoving);
            }

        }

        if (onGround && !isFlying)
        {
            if ((Keyboard::isKeyPressed(Keyboard::Up)) || (Keyboard::isKeyPressed(Keyboard::Space))) {
                velocityY = -20;
            }
        }
        else if (!onGround && !isFlying)
        {
            if ((currentIndex == 0 || currentIndex == 1)) {
                currentIndex = 4;

                ESprite.setTexture(SpriteTex[4].T);
            }
            else if ((currentIndex == 2 || currentIndex == 3))
            {
                currentIndex = 5;

                ESprite.setTexture(SpriteTex[5].T);
            }
            isMoving = true;

        }
        if (isFlying)
        {
            //std::cout << "HELLO\n";
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                //player_y -= 10;
                offset_y -= 10;
            }
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                offset_y += 10;
            }
            //isMoving = true;
            ESprite.setPosition(player_x, player_y);

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

        return isMoving;

    }

    void MakeInvisible()
    {
        InvspowerUp = true;
        ESprite.setColor(sf::Color(255, 255, 255, 40));
        KnuInvClock.restart();

    }

    void player_gravity(char** lvl)
    {
        if (isFlying) {
            player_y = offset_y;
            onGround = false;
            return;
        }
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
            lastOnGround.x = player_x;
            lastOnGround.y = player_y;
            lastx = offset_x;
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

    virtual  void AnimateSprite(bool isMoving)
    {
        if (isInvincible && !InvspowerUp)
            ESprite.setColor({ 255,255,255,200 });

        if (isSonic && InvspowerUp) 
        {
            if (left) 
            {
                currentIndex = 11;
                isMoving = true;

            }
            else 
            {
                currentIndex = 12;
                isMoving = true;
            }
        }
        if (isMoving)
        {
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

    void AddHp()
    {
        hp += 5;
        if (hp > 100) hp = 100;

    }

    void takeDamage(int dmg)
    {
        if (isInvincible)
        {

            return;                // ignore if still invincible
        }
        if (isKnuckles && InvspowerUp) 
        {
            return;
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
    void StartRunning() 
    {
        if(isSonic)
        InvspowerUp = true;
        velocityX += 5;

    }

    void borderCheck()
    {
        //std::cout << player_y << std::endl;
        //std::cout << lastOnGround.x + lastx << " " << lastOnGround.y << std::endl;
        if (player_y < 22) {
            player_y = 22;
        }
        else if (player_y >= 720) {
           /* player_x = lastOnGround.x - 40;
            player_y = lastOnGround.y - 200;
            offset_x = lastx - 24;
            ESprite.setPosition(player_x + offset_x, player_y);*/
            hp = 0;
        }
        //std::cout << player_y << std::endl;
    }


    void update()
    {
        // clear invincibility after duration
        if (isInvincible && invClock.getElapsedTime().asSeconds() >= invDuration)
        {
            isInvincible = false;
            ESprite.setColor(sf::Color(255, 255, 255, 255));
        }
        if (InvspowerUp && KnuInvClock.getElapsedTime().asSeconds() >= 10.0)
        {
            InvspowerUp = false;
            ESprite.setColor(sf::Color(255, 255, 255, 255));
        }

    }
    virtual void hang(float x, float y, float off_x) = 0;


    int getX() const {
        return player_x;
    }
    int getY() const {
        return player_y;
    }
    void setX(float x) {
        player_x = x;
    }
    void setY(float y) {
        player_y = y;
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
    bool getIsFlying() {
        return isFlying;
    }
    int getVelocityY() const {
        return velocityY;
    }
    void SetHp(int hhp) 
    {
        hp = hhp;
    }

    virtual void punching(char** lvl, bool check) = 0;
    void setPos(float x, float y) {
        player_x = x;
        player_y = y;
    }
    sf::Vector2f getPos() {
        return Vector2f(player_x, player_y);
        // maybe add an offx to this too
    }

    void setWidth(int width) {
        totalWidth = width;
    }
    void setOffsetX(float x) {
        offset_x = x;
    }
    void setVelocityY(float y) {
        velocityY = y;
    }
    void setVelocityX(float x) {
        velocityX = x;
    }
    float getVelocityX() {
        return velocityX;
    }
    void setSpeed(float speed) {
        max_speed = speed;
    }
    float getSpeed() {
        return max_speed;
    }
    bool getDirection() {
        return left;
    }
    void setDirection(bool dir) {
        left = dir;
    }
    void setcurrentIndex(int index) 
    {
        currentIndex = index;
    }
    int getcurrentIndex() {
        return currentIndex;
    }
	void setFriction(float f) {
		friction = f;
	}
	void setGravity(float g) {
		gravity = g;
        old = g;
	}
	float getFriction() {
		return friction;
	}
	float getGravity() {
		return gravity;
	}

 static int GetHp()
    {
        return hp;
    }
    int getPower() {
        return power;
    }


};
int MySprite ::  hp = 100;
class Sonic : public MySprite
{
private:
    //bool isGliding;


public:
    Sonic() : MySprite()
    {
        SpriteTex = new SpriteSheet[13];
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
        //Hang to tails
        //left
        if (!SpriteTex[9].T.loadFromFile("Data/Sonic Hang_L.png")) {
            std::cout << "Failed to load Sonic Hang_L.png!" << std::endl;

        }
        else
        {
            SpriteTex[9].frameNum = 8;
        }
        //right
        if (!SpriteTex[10].T.loadFromFile("Data/Sonic Hang_R.png")) {
            std::cout << "Failed to load Sonic Hang_R.png!" << std::endl;

        }
        else
        {
            SpriteTex[10].frameNum = 8;
        }
        //Running
        if (!SpriteTex[11].T.loadFromFile("Data/0left_run.png")) {
            std::cout << "Failed to load 0left_run.png!" << std::endl;

        }
        else
        {
            SpriteTex[11].frameNum = 8;
        }
        if (!SpriteTex[12].T.loadFromFile("Data/0right_run.png")) {
            std::cout << "Failed to load 0right_run.png!" << std::endl;

        }
        else
        {
            SpriteTex[12].frameNum = 8;
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
        //isGliding = false;
        power = 10;
        InvspowerUp = false;
        isKnuckles = false;
        isSonic = true;


        // after you set raw_img_x = 24; raw_img_y = 35;  (or whatever your actual frame size is)
        SpriteRect = IntRect(0, 0, 49, 49);
        ESprite.setTextureRect(SpriteRect);


        // Set up SonicSprite
        ESprite.setTexture(SpriteTex[0].T);
        ESprite.setTextureRect(SpriteRect);
        ESprite.setPosition(player_x, player_y);
        ESprite.setScale(scale_x, scale_y);
    }


    void hang(float x, float y, float off_x) override {
        // Implement hanging behavior for Sonic, or leave empty if not needed
        this->player_x = x - 10;
        this->player_y = y + 64;
        this->offset_x = off_x;

        if (left) {
            currentIndex = 9;
            ESprite.setTexture(SpriteTex[currentIndex].T);
        }
        else {
            currentIndex = 10;
            ESprite.setTexture(SpriteTex[currentIndex].T);
        }


        updateTextureRectForCurrentIndex();
        AnimateSprite(true);
        ESprite.setScale(2.10, 2.30);
        ESprite.setPosition(player_x, player_y);
    }






    void AnimateSprite(bool isMoving)override
    {
        if (isMoving) {
            if (animationClock.getElapsedTime().asMilliseconds() > 80) {
                currentFrame = (currentFrame + 1) % SpriteTex[currentIndex].frameNum;
                SpriteRect.left = currentFrame * GetFrameWidth(SpriteTex[currentIndex]);
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

    void punching(char** lvl, bool check) override {

    }
};

class Knuckles : public MySprite
{
private:
    bool isGliding;
    public:
    Knuckles() : MySprite()
    {

        SpriteTex = new SpriteSheet[13];
        //  IDLE
        if (!SpriteTex[0].T.loadFromFile("Data/Knuckles_Idle_L.png")) { //40x40 => 1 frame
            std::cout << "Failed to load Knuckles_Idle_L.png!" << std::endl;

        }
        else
        {
            SpriteTex[0].frameNum = 1;
        }
        //LEFT RUN
        if (!SpriteTex[1].T.loadFromFile("Data/Knuckles_Jog_Walk_L.png")) { //493x40 => 10 frames
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
        //Left
        if (!SpriteTex[11].T.loadFromFile("Data/Knuckles_Hang_L.png")) //966x50 =>20 frames
        {

            std::cout << "Failed to load Knuckles_Hang_L.png!" << std::endl;
        }
        else
        {
            SpriteTex[11].frameNum = 20;
        }
        if (!SpriteTex[12].T.loadFromFile("Data/Knuckles_Hang_r.png")) //966x50 =>20 frames
        {

            std::cout << "Failed to load Knuckles_Hang_R.png!" << std::endl;
        }
        else
        {
            SpriteTex[12].frameNum = 20;
        }
        //Glide
        
        // Initialize variables
        velocityY = 0;
        velocityX = 0;
        player_x = 100;
        player_y = 100;
        window_x = 0; // Note: There was a duplicate assignment in the original
        window_y = 0; // Corrected to window_y
        acceleration = 1.1;
        max_speed = 12;
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
        power = 10;
        isGliding = false;
        InvspowerUp = false;
        isKnuckles = true;
        isSonic = false;

        // after you set raw_img_x = 24; raw_img_y = 35;  (or whatever your actual frame size is)
        SpriteRect = IntRect(0, 0, 40, 40);
        ESprite.setTextureRect(SpriteRect);

        ESprite.setTexture(SpriteTex[0].T);
        ESprite.setTextureRect(SpriteRect);
        ESprite.setPosition(player_x, player_y);
        ESprite.setScale(scale_x, scale_y);



    }
    void hang(float x, float y, float off_x) override
    {
        // Implement hanging behavior for Sonic, or leave empty if not needed
        this->player_x = x + 30;
        this->player_y = y + 64;
        this->offset_x = off_x;
        currentIndex = (left) ? 11 : 12;
        ESprite.setTexture(SpriteTex[currentIndex].T);
        // animationClock.restart();
        updateTextureRectForCurrentIndex();
        AnimateSprite(true);
        ESprite.setPosition(player_x, player_y);
    }

    bool movement(char** lvl, bool check, bool check2) override {
        //std::cout << player_x + offset_x << " " << player_y << std::endl;
        bool isMoving = MySprite::movement(lvl, check, check2);
        if (!isGliding && Keyboard::isKeyPressed(Keyboard::F) && check) {
            //velocityX = velocityX > 0 ? +10 : -10;
            velocityY = -3;

            isGliding = true;
        }
        if (isGliding) {
            gravity = 0.15;
        }

        if (onGround) {
            isGliding = false;
            gravity = old;
        }
        /*std::cout << gravity << std::endl;
        std::cout << velocityX << " " << velocityY << std::endl;*/

        return isMoving;
    }

    void punching(char** lvl, bool check) override
    {
        //std::cout << Punch << std::endl;
        //std::cout << ((player_x + hit_box_factor_x + Pwidth + offset_x + 15) / cell_size) << std::endl;
        if (Keyboard::isKeyPressed(Keyboard::T) && check)
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
            else if (!left && Punch)
            {
                ESprite.setTexture(SpriteTex[9].T);
                currentIndex = 9;
                updateTextureRectForCurrentIndex();
                AnimateSprite(true);
            }
        }

        else {
            Punch = false;
        }
    }



    void AnimateSprite(bool isMoving) override
    {
        if (Punch) {
            if (animationClock.getElapsedTime().asMilliseconds() > 80) {
                currentFrame++;
                if (currentFrame >= SpriteTex[currentIndex].frameNum) {
                    Punch = false;
                    currentFrame = 0;
                }
                else {
                    SpriteRect.left = currentFrame * SpriteRect.width;
                    ESprite.setTextureRect(SpriteRect);
                }
                animationClock.restart();
            }
        }
        if (InvspowerUp)
        {
            ESprite.setColor(sf::Color(255, 255, 255, 40));
        }


        if (isMoving) {

            if (animationClock.getElapsedTime().asMilliseconds() > 60)
            {
                if (currentIndex == 1 || currentIndex == 3)
                {
                    ESprite.setScale(2.3, 2.56);
                }
                else if (currentIndex == 6 || currentIndex == 7)
                {
                    ESprite.setScale(2.5, 2.5);
                }
                else if (currentIndex == 4 || currentIndex == 5)
                {
                    ESprite.setScale(2, 2.25);
                }
                else if (currentIndex == 9 || currentIndex == 10)
                {
                    ESprite.setScale(2.5, 2.4);
                }
                else if (currentIndex == 11 || currentIndex == 12)
                {
                    ESprite.setScale(2.07, 2);
                }
                else
                {
                    ESprite.setScale(2.0, 1.75);
                }
                if (currentIndex == 10)
                {
                    //currentFrame = SpriteTex[10].frameNum;

                    currentFrame = (currentFrame + 1) % SpriteTex[currentIndex].frameNum;
                    SpriteRect.left = currentFrame * SpriteRect.width;
                    ESprite.setTextureRect(SpriteRect);
                    animationClock.restart();
                    std::cout << currentFrame << std::endl;

                }
                else
                {
                    //std::cout << currentFrame << std::endl;

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
            ESprite.setScale(2.5, 2.5);
        }

    }
    void MakeInvisible()
    {

        ESprite.setColor(sf::Color(255, 255, 255, 200));

    }


};

class Tails : public MySprite
{
private:
    sf::Clock flyingClock;
    const float maxFlyTime = 7;

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
        if (!SpriteTex[4].T.loadFromFile("Data/Tail_Jump.png"))
        {
            std::cout << "Failed to load Tail_Jump.png!" << std::endl;

        }
        else
        {
            SpriteTex[4].frameNum = 6;
        }
        // Load texture for jump right 
        if (!SpriteTex[5].T.loadFromFile("Data/Tail_Jump.png"))
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
        // Load texture for injured
        if (!SpriteTex[8].T.loadFromFile("Data/Tails_Hurt.png"))
        {
            std::cout << "Failed to load Tails_Hurt.png!" << std::endl;
        }
        else
        {
            SpriteTex[8].frameNum = 5;
        }

        // Load texture for fly right
        if (!SpriteTex[9].T.loadFromFile("Data/Tail_Fly_R.png"))
        {
            std::cout << "Failed to load Tails_Fly_R.png!" << std::endl;
        }
        else
        {
            SpriteTex[9].frameNum = 4;
        }
        // Load texture for fly left
        if (!SpriteTex[10].T.loadFromFile("Data/Tail_Fly_L.png"))
        {
            std::cout << "Failed to load Tails_Fly_L.png!" << std::endl;
        }
        else
        {
            SpriteTex[10].frameNum = 4;
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
        power = 10;
        isFlying = false;
        InvspowerUp = false;
        isKnuckles = false;
        isSonic = false;

        // Set up sprite rectangle
        SpriteRect = IntRect(0, 0, 40, 40);
        ESprite.setTextureRect(SpriteRect);

        // Set up Tails sprite
        // ESprite.setTexture(SpriteTex[0].T); // Set to idle left
        ESprite.setTextureRect(SpriteRect);
        ESprite.setPosition(player_x, player_y);
        ESprite.setScale(scale_x, scale_y);
    }


    bool movement(char** lvl, bool check, bool check2) override
    {
        bool isMovingBase = MySprite::movement(lvl, check, check2);

        if (Keyboard::isKeyPressed(Keyboard::F) && check)
        {
            if (!isFlying)
            {
                isFlying = true;
                flyingClock.restart();
                std::cout << "Flying started!" << std::endl;
                isMoving = true;
                player_y -= 5;
            }

            ESprite.setPosition(player_x, player_y);
        }

        // Timeout check: stop flying after max time
        if (isFlying && flyingClock.getElapsedTime().asSeconds() >= maxFlyTime) {
            isFlying = false;
            std::cout << "Flying ended due to timeout." << std::endl;
        }

        if (isFlying)
        {
            AnimateSprite(true);
            currentIndex = left ? 10 : 9;
            ESprite.setTexture(SpriteTex[currentIndex].T);
            return true;
        }




        //if (!isFlying) {
        //    if (!onGround) {
        //        currentIndex = left ? 4 : 5;  // jump animations
        //    }
        //    else if (!isMovingBase) {
        //        currentIndex = left ? 0 : 2;  // idle animations
        //    }
        //}

        return isMovingBase;
    }




    void AnimateSprite(bool isMoving) override
    {

        if (isMoving || isFlying) {

            if (animationClock.getElapsedTime().asMilliseconds() > 80)
            {
                if (currentIndex == 1 || currentIndex == 3)
                {
                    ESprite.setScale(2.56, 2.63);
                }
                else if (currentIndex == 6 || currentIndex == 7)
                {
                    ESprite.setScale(3.205, 2.63);
                }
                else if (currentIndex == 4 || currentIndex == 5)
                {
                    ESprite.setScale(2.54, 2.63);
                }
                else if (currentIndex == 8)
                {
                    ESprite.setScale(2.56, 2.70);
                }
                else if (currentIndex == 9 || currentIndex == 10)
                {
                    ESprite.setScale(2.1, 2.63);
                }


                updateTextureRectForCurrentIndex();
                currentFrame = (currentFrame + 1) % SpriteTex[currentIndex].frameNum;
                //std::cout << "Current frame: " << currentFrame << std::endl;
                SpriteRect.left = currentFrame * SpriteRect.width;
                ESprite.setTextureRect(SpriteRect);
                animationClock.restart();


            }
        }
        else
        {
            // reset to first frame of current strip
            currentFrame = 0;
            SpriteRect.left = 0;
            ESprite.setTextureRect(SpriteRect);
            ESprite.setScale(2.5, 2.5);
        }

    }

    void punching(char** lvl, bool check) override {

    }






    void hang(float x, float y, float off_x) override
    {
        // Implement hanging behavior for Sonic, or leave empty if not needed
    }





};


class GOKU : public MySprite
{
    bool isGliding;
    sf::Clock flyingClock;
    const float maxFlyTime = 7.0f;


public:
    GOKU() : MySprite()
    {
        SpriteTex = new SpriteSheet[11];
        // Load textures into the array
        if (!SpriteTex[0].T.loadFromFile("Data/Goku_Idle_L.png")) {
            std::cout << "Failed to load Goku_Idle_L.png!" << std::endl;
        }
        else
        {
            SpriteTex[0].frameNum = 1;
        }
        if (!SpriteTex[1].T.loadFromFile("Data/Goku_Jog_L.png")) {
            std::cout << "Failed to load Goku_Jog_L.png!" << std::endl;
        }
        else
        {
            SpriteTex[1].frameNum = 4;
        }
        if (!SpriteTex[2].T.loadFromFile("Data/Goku_Idle_R.png")) {
            std::cout << "Failed to load Goku_Idle_R.png!" << std::endl;
        }
        else
        {
            SpriteTex[2].frameNum = 1;
        }
        if (!SpriteTex[3].T.loadFromFile("Data/Goku_Jog_R.png")) {
            std::cout << "Failed to load Goku_Jog_R.png!" << std::endl;
        }
        else
        {
            SpriteTex[3].frameNum = 4;
        }
        if (!SpriteTex[4].T.loadFromFile("Data/Goku_0up_L.png")) 
        {
            std::cout << "Failed to load Goku_0up_L.png!" << std::endl;
        }
        else
        {
            SpriteTex[4].frameNum = 4;
        }
        if (!SpriteTex[5].T.loadFromFile("Data/Goku_0up_R.png"))
        {
            std::cout << "Failed to load Goku_0up_R.png!" << std::endl;
        }
        else
        {
            SpriteTex[5].frameNum = 4;
        }

        //No tripping pose
        // Punching left
        if (!SpriteTex[6].T.loadFromFile("Data/Goku_Punch_L.png"))
        {
            std::cout << "Failed to load Goku_Punch_L.png!" << std::endl;
        }
        else
        {
            SpriteTex[6].frameNum = 5;
        }
        // Punching right
        if (!SpriteTex[7].T.loadFromFile("Data/Goku_Punch_R.png"))
        {
            std::cout << "Failed to load Goku_Punch_R.png!" << std::endl;
        }
        else
        {
            SpriteTex[7].frameNum = 5;
        }
        //He believes he can fly
        if (!SpriteTex[8].T.loadFromFile("Data/Goku_Fly_L.png"))
        {
            std::cout << "Failed to load Goku_Fly_L.png!" << std::endl;
        }
        else
        {
            SpriteTex[8].frameNum = 7;
        }
        //He believes he can fly
        if (!SpriteTex[9].T.loadFromFile("Data/Goku_Fly_R.png"))
        {
            std::cout << "Failed to load Goku_Fly_R.png!" << std::endl;
        }
        else
        {
            SpriteTex[9].frameNum = 7;
        }
        //Injured
        if (!SpriteTex[10].T.loadFromFile("Data/Goku_Injured_R.png"))
        {
            std::cout << "Failed to load Goku_Injured_R.png!" << std::endl;
        }
        else
        {
            SpriteTex[9].frameNum = 5;
        }
        velocityY = 0;
        velocityX = 0;
        player_x = 100;
        player_y = 100;
        window_x = 0;
        window_y = 0;
        acceleration = 1.1;
        max_speed = 12; // Sonic's speed
        isSonic = false;
      
        power = 20; // Initialize power
        isInvincible = false;
        onGround = false;
        gravity = 1;
        friction = 0.80;
        terminal_Velocity = 20;
        scale_x = 3.5;
        scale_y = 3.5;
        raw_img_x = 24;
        raw_img_y = 35;
        Pheight = raw_img_y * scale_y;
        Pwidth = raw_img_x * scale_x;
        hit_box_factor_x = 8 * scale_x;
        hit_box_factor_y = 5 * scale_y;
        offset_x = 0;
        offset_y = 0;
        isGliding = false;
        isFlying = false;
        InvspowerUp = false;
        isKnuckles = false;

        // Set up sprite rectangle
        SpriteRect = IntRect(0, 0, 40, 40);
        ESprite.setTextureRect(SpriteRect);

        // Set up Goku sprite
        ESprite.setTexture(SpriteTex[0].T); // Start with idle left
        ESprite.setTextureRect(SpriteRect);
        ESprite.setPosition(player_x, player_y);
        ESprite.setScale(scale_x, scale_y);
    }
    bool movement(char** lvl, bool check, bool check2) override
    {
        isMoving = MySprite::movement(lvl, check, check2);

        if (Keyboard::isKeyPressed(Keyboard::F) && check)
        {
            if (!isFlying)
            {
                isFlying = true;
                flyingClock.restart();
                std::cout << "Flying started!" << std::endl;
                isMoving = true;
                player_y -= 5;
            }

            ESprite.setPosition(player_x, player_y);
        }

        // Timeout check: stop flying after max time
        if (isFlying && flyingClock.getElapsedTime().asSeconds() >= maxFlyTime) {
            isFlying = false;
            std::cout << "Flying ended due to timeout." << std::endl;
        }

        if (isFlying)
        {
            AnimateSprite(true);
            currentIndex = left ? 10 : 9;
            ESprite.setTexture(SpriteTex[currentIndex].T);
            return true;
        }

        if (!isGliding && Keyboard::isKeyPressed(Keyboard::G) && check) {
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
        return isMoving;
    }
    void punching(char** lvl, bool check) override
    {
        //std::cout << Punch << std::endl;
        //std::cout << ((player_x + hit_box_factor_x + Pwidth + offset_x + 15) / cell_size) << std::endl;
        if (Keyboard::isKeyPressed(Keyboard::T) && check)
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
                ESprite.setTexture(SpriteTex[6].T);
                currentIndex = 6;
                //std::cout << currentIndex << std::endl;
                updateTextureRectForCurrentIndex();
                AnimateSprite(true);
            }
            else if (!left && Punch)
            {
                ESprite.setTexture(SpriteTex[7].T);
                currentIndex = 7;
                updateTextureRectForCurrentIndex();
                AnimateSprite(true);
            }
        }

        else {
            Punch = false;
        }
    }

    void hang(float x, float y, float off_x) {

    }
};


