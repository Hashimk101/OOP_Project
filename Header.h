#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include"Maps.h"

using namespace sf;
int cell_size = 64;

class MySprite { // Renamed to avoid conflict with sf::Sprite
    Texture SonicTex, TailsTex, KnucklesTex;
    float velocityY, velocityX, player_x, player_y, max_speed, acceleration;
	float offset_x, offset_y, terminal_Velocity;
    float scale_x, scale_y;
    int raw_img_x, raw_img_y;
	int Pheight, Pwidth;
	int hit_box_factor_x, hit_box_factor_y;
	float window_x, window_y;
	bool onGround;
	float gravity;
	// Sprites for different characters
	sf::Sprite SonicSprite, TailsSprite, KnucklesSprite;

public:
    // Constructor to initialize the sprite with the texture
    MySprite() {
        // Load texture and set it to the sprite (example)
        if (!SonicTex.loadFromFile("Data/0left_still.png")) {
            // Handle error if texture fails to load
            std::cout << "Failed to load texture!" << std::endl;
        }
        velocityY = 0;
        velocityX = 0;
        player_x = 100;
        player_y = 100;
        window_x = 0;
		window_x = 0;
        acceleration = 10;
        max_speed = 15;
        onGround = false;
		gravity = 1;
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

        SonicSprite.setTexture(SonicTex);
		SonicSprite.setPosition(player_x, player_y);
        SonicSprite.setScale(scale_x, scale_y);
    }

    // Function to get the sprite (for rendering, etc.)
    sf::Sprite& getSprite() {
        return SonicSprite;
    }
    void movement(char** lvl) {
        if (Keyboard::isKeyPressed(Keyboard::Left)) {

            if ((lvl[((int)(offset_y + hit_box_factor_y + Pheight) / cell_size) - 1][(int)(offset_x + player_x + hit_box_factor_x) / cell_size]) == 'w') {
                return;
            }
            velocityX = -max_speed;
            
            if (player_x <= 250) {
                //std::cout << "End of Window" << player_x << std::endl;

                offset_x -= 20;
                if (offset_x <= -30) {
					offset_x = -30;
                }
                //player_x += 20;
            }
            else {
                player_x += velocityX;
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right)) {
            if ((lvl[((int)(offset_y + hit_box_factor_y + Pheight) / cell_size) - 1][(int)(offset_x + player_x + hit_box_factor_x + Pwidth) / cell_size]) == 'w') {
                return;
            }
            velocityX = max_speed;
            if (player_x >= 950) {
                //std::cout << "End of Window" << player_x << std::endl;
                offset_x += 20;
                //player_x -= 20;
            }
            else {
                player_x += velocityX;
            }
        }
        if (onGround) {
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
            velocityY = -22;
            //std::cout << "Space pressed\n";
            }
        }
    }
    
    void player_gravity(char** lvl)
    {
        offset_y = player_y;

        offset_y += velocityY;

        char bottom_left_down = lvl[(int)(offset_y + hit_box_factor_y + Pheight) / cell_size][(int)(offset_x + player_x + hit_box_factor_x) / cell_size];
        char bottom_right_down = lvl[(int)(offset_y + hit_box_factor_y + Pheight) / cell_size][(int)(offset_x + player_x + hit_box_factor_x + Pwidth) / cell_size];
        char bottom_mid_down = lvl[(int)(offset_y + hit_box_factor_y + Pheight) / cell_size][(int)(offset_x + player_x + hit_box_factor_x + Pwidth / 2) / cell_size];
       


        if (bottom_left_down == 'w' || bottom_mid_down == 'w' || bottom_right_down == 'w')
        {
            onGround = true;
        }
        else
        {
            player_y = offset_y;
            onGround = false;
        }

        if (!onGround)
        {
            velocityY += gravity;
            if (velocityY >= terminal_Velocity) velocityY = terminal_Velocity;
        }

        else
        {
            velocityY = 0;
        }

    }

    float getX() {
        return player_x;
    }
    float getY() {
		return player_y;
    }
    void draw_player(RenderWindow& window) {

        SonicSprite.setPosition(player_x, player_y);
        window.draw(SonicSprite);

    }
	float getOffsetX() {
		return offset_x;
	}
};


