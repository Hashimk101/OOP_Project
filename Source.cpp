//#include <iostream>
//#include <fstream>
//#include <cmath>
//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/Window.hpp>
#include "Header.h"
#include "Maps.h"
#include "Collectables.h"
#include "Enemies.h"

using namespace sf;
using namespace std;

int screen_x = 1200;
int screen_y = 900;
// prototypes 
void player_gravity(char** lvl, int& offset_y, int& velocityY, bool& onGround, float& gravity, int& terminal_Velocity, int& hit_box_factor_x, int& hit_box_factor_y, int& player_x, int& player_y, const int cell_size, int& Pheight, int& Pwidth);

void draw_player(RenderWindow& window, Sprite& LstillSprite, int player_x, int player_y);

void display_level(RenderWindow& window, const int height, const int width, char** lvl, Sprite& wallSprite1, const int cell_size, int offset, Sprite& background);

int main()
{

	RenderWindow window(VideoMode(screen_x, screen_y), "Sonic the Hedgehog-OOP", Style::Close);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(120);
	sf::Texture BackgroundTex;
	BackgroundTex.loadFromFile("Data/bg1.png");
	sf::Sprite BackgroundSprite(BackgroundTex);
	BackgroundSprite.setScale(1.8, 1.2);
	//BackgroundSprite.setPosition(100, 100);
	/////////////////////////////////////////////////////////////////
	// a cell is 64 by 64 pixels

	// 's' is regular space
	// 'q' is wall1 or floor1
	// 'w' is wall2 or floor2
	// 'e' is wall3 or floor3
	// 'b' is breakable wall
	// 'z' is spring

	// Uppercase for not interactable background accessories

	// C is for crystals
	MySprite sprite;
	Maps map;
	char** lvl = map.getMap();

	Texture wallTex1;
	wallTex1.loadFromFile("Data/bl.jpg");
	Sprite wallSprite1(wallTex1);
	wallSprite1.setScale(0.64, 0.64);
	RingCoin Coins(lvl);
	Diamond diamonds(lvl);
	Motobug m;
	m.setPosition(750, 650);
	
	m.setScale(3.5, 2.5);
	Coins.place();
	diamonds.place();

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear(Color::Black);
		BackgroundSprite.setPosition(-sprite.getOffsetX()/7, 0);
		window.draw(BackgroundSprite);
		display_level(window, height, width, lvl, wallSprite1, cell_size, sprite.getOffsetX(), BackgroundSprite);
		
		Coins.draw(window, sprite.getOffsetX());
		m.draw(window);
		m.animateSprite();
		m.move(sprite.getX(), sprite.getY(), sprite.getOffsetX(), sprite.getOffsetY());
		sprite.movement(lvl);
		sprite.player_gravity(lvl);
		Coins.checkCollision(sprite.getX(), sprite.getY(), sprite.getOffsetX(), sprite.getOffsetY(), sprite.gethitX(), sprite.gethitY());
		diamonds.checkCollision(sprite.getX(), sprite.getY(), sprite.getOffsetX(), sprite.getOffsetY(), sprite.gethitX(), sprite.gethitY());
		sprite.draw_player(window);
		Coins.animate();
		diamonds.animate();
		//m.update();

		Coins.draw(window, sprite.getOffsetX());
		diamonds.draw(window, sprite.getOffsetX());

		//window.draw()

		window.display();
	}


	return 0;
}


// functions

void player_gravity(char** lvl, int& offset_y, int& velocityY, bool& onGround, float& gravity,int& terminal_Velocity, int& hit_box_factor_x, int& hit_box_factor_y, int& player_x, int& player_y, const int cell_size, int& Pheight, int& Pwidth)
{
	offset_y = player_y;

	offset_y += velocityY;

	char bottom_left_down = lvl[(int)(offset_y + hit_box_factor_y + Pheight) / cell_size][(int)(player_x + hit_box_factor_x) / cell_size];
	char bottom_right_down = lvl[(int)(offset_y + hit_box_factor_y + Pheight) / cell_size][(int)(player_x + hit_box_factor_x + Pwidth) / cell_size];
	char bottom_mid_down = lvl[(int)(offset_y + hit_box_factor_y + Pheight) / cell_size][(int)(player_x + hit_box_factor_x + Pwidth / 2) / cell_size];


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
void draw_player(RenderWindow& window, Sprite& LstillSprite, int player_x, int player_y) {

	LstillSprite.setPosition(player_x, player_y);
	window.draw(LstillSprite);

}
void display_level(RenderWindow& window, const int height, const int width, char** lvl, Sprite& wallSprite1, const int cell_size, int offset, Sprite& background)
{
	for (int i = 0; i < height; i += 1)
	{
		// hell yeah, only loads the current window into memory
		// THE GREATEST PROGRAMMER THAT EVER LIVED
		for (int j = offset/64; j < (1300 + offset)/64; j += 1)
		{
			if (lvl[i][j] == 'w')
			{
				wallSprite1.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(wallSprite1);
				/*background.setPosition(0 - offset / 4, 0);*/
				/*window.draw(background);*/
			}
			/*else if (lvl[i][j] == 's') {
				window.draw();
			}*/
		}
	}
}