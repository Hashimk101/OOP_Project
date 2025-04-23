#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

int screen_x = 1200;
int screen_y = 900;

// prototypes 
void player_gravity(char** lvl, float& offset_y, float& velocityY, bool& onGround, float& gravity, float& terminal_Velocity, int& hit_box_factor_x, int& hit_box_factor_y, float& player_x, float& player_y, const int cell_size, int& Pheight, int& Pwidth);

void draw_player(RenderWindow& window, Sprite& LstillSprite, float player_x, float player_y);

void display_level(RenderWindow& window, const int height, const int width, char** lvl, Sprite& wallSprite1, const int cell_size);

int main()
{

	RenderWindow window(VideoMode(screen_x, screen_y), "Sonic the Hedgehog-OOP", Style::Close);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(120);
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

	const int cell_size = 64;
	const int height = 14;
	const int width = 110;

	char** lvl = NULL;

	Texture wallTex1;
	Sprite wallSprite1;

	Music lvlMus;

	lvlMus.openFromFile("Data/labrynth.ogg");
	lvlMus.setVolume(30);
	lvlMus.play();
	lvlMus.setLoop(true);

	lvl = new char* [height];
	for (int i = 0; i < height; i += 1)
	{
		lvl[i] = new char[width] {'\0'};
	}


	lvl[5][1] = 'w';
	lvl[11][2] = 'w';
	lvl[11][3] = 'w';

	wallTex1.loadFromFile("Data/brick1.png");
	wallSprite1.setTexture(wallTex1);
	////////////////////////////////////////////////////////
	float player_x = 100;
	float player_y = 100;

	float max_speed = 15;

	float velocityX = 0;
	float velocityY = 0;

	float jumpStrength = -20; // Initial jump velocity
	float gravity = 1;  // Gravity acceleration

	Texture LstillTex;
	Sprite LstillSprite;

	bool onGround = false;

	float offset_x = 0;
	float offset_y = 0;

	float terminal_Velocity = 20;

	float acceleration = 0.2;

	float scale_x = 2.5;
	float scale_y = 2.5;

	////////////////////////////
	int raw_img_x = 24;
	int raw_img_y = 35;

	int Pheight = raw_img_y * scale_y;
	int Pwidth = raw_img_x * scale_x;

	//only to adjust the player's hitbox

	int hit_box_factor_x = 8 * scale_x;
	int hit_box_factor_y = 5 * scale_y;

	LstillTex.loadFromFile("Data/0left_still.png");
	LstillSprite.setTexture(LstillTex);
	LstillSprite.setScale(scale_x, scale_y);

	////////////////////////////////////////////////////////

	Event ev;
	while (window.isOpen())
	{

		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				window.close();
			}

			if (ev.type == Event::KeyPressed)
			{
			}

		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		player_gravity(lvl, offset_y, velocityY, onGround, gravity, terminal_Velocity, hit_box_factor_x, hit_box_factor_y, player_x, player_y, cell_size, Pheight, Pwidth);

		window.clear();

		display_level(window, height, width, lvl, wallSprite1, cell_size);
		draw_player(window, LstillSprite, player_x, player_y);

		window.display();
	}


	return 0;
}


// functions

void player_gravity(char** lvl, float& offset_y, float& velocityY, bool& onGround, float& gravity, float& terminal_Velocity, int& hit_box_factor_x, int& hit_box_factor_y, float& player_x, float& player_y, const int cell_size, int& Pheight, int& Pwidth)
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
void draw_player(RenderWindow& window, Sprite& LstillSprite, float player_x, float player_y) {

	LstillSprite.setPosition(player_x, player_y);
	window.draw(LstillSprite);

}
void display_level(RenderWindow& window, const int height, const int width, char** lvl, Sprite& wallSprite1, const int cell_size)
{
	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			if (lvl[i][j] == 'w')
			{
				wallSprite1.setPosition(j * cell_size, i * cell_size);
				window.draw(wallSprite1);
			}
		}
	}
}