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

void display_level(RenderWindow& window, const int height, const int width, char** lvl, Sprite& wallSprite1, const int cell_size, int offset, Sprite& background, Sprite& Bush, Sprite& BrownTower, Sprite& spikes, Sprite& BreakableWallSprite);

int main()
{

	RenderWindow window(VideoMode(screen_x, screen_y), "Sonic the Hedgehog-OOP", Style::Close);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(120);
	sf::Texture BackgroundTex;
	BackgroundTex.loadFromFile("Data/bg5.png");
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
	//B is for greenBushes
	// C is for crystals
	//T is for brown tower
	Sonic sprite;
	Maps map;
	char** lvl = map.getMap();
	Knuckles K;

	Texture wallTex1;
	wallTex1.loadFromFile("Data/bl.jpg");
	Texture BreakableWallTex;
	BreakableWallTex.loadFromFile("Data/brick4.png");
	Sprite BreakableWallSprite(BreakableWallTex);

	Sprite wallSprite1(wallTex1);
	Texture GreenBush;
	GreenBush.loadFromFile("Data/GreenBush.png");
	Sprite GreenBushSprite(GreenBush);
	Texture BrownTower;
	BrownTower.loadFromFile("Data/BrownTower.png");
	Sprite BrownTowerSprite(BrownTower);
	Texture spikeTex;
	spikeTex.loadFromFile("Data/spike.png");
	Sprite spikeSprite(spikeTex);


	wallSprite1.setScale(0.64, 0.64);
	RingCoin Coins(lvl);
	ExtraLife diamonds(lvl);
	SpecialBoost special(lvl);
	MotoBug m(2600, 730, lvl);

	CrabMeat crab(5500, 400, lvl);
	//CrabMeat crab2(10550, 470, lvl);
	BatBrain Bat(400, 300, lvl);
	BuzzBomber Buzz(10000, 100, lvl);

	Coins.place();
	diamonds.place();
	special.place();

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear(Color::Black);
		BackgroundSprite.setPosition(-sprite.getOffsetX() / 7, 0);
		window.draw(BackgroundSprite);
		display_level(window, height, width, lvl, wallSprite1, cell_size, sprite.getOffsetX(), BackgroundSprite, GreenBushSprite, BrownTowerSprite, spikeSprite, BreakableWallSprite);

		m.draw(window);
		m.animateSprite();
		Bat.animateSprite();
		Bat.draw(window);

		m.move(sprite.getX(), sprite.getY(), sprite.getOffsetX(), sprite.getOffsetY());
		Bat.move(sprite.getX(), sprite.getY(), sprite.getOffsetX(), sprite.getOffsetY());
		int dmg2 = Bat.giveDamage(sprite.getVelocityY(), sprite.getX(), sprite.getY(), sprite.getOffsetX());
		if (dmg2 > 0) sprite.takeDamage(dmg2);
		std::cout << "DMG 2" << dmg2 << std::endl;

		Buzz.move(sprite.getX(), sprite.getY(), sprite.getOffsetX(), sprite.getOffsetY());
		int bombdmg = Buzz.giveDamage(sprite.getVelocityY(), sprite.getX(), sprite.getY(), sprite.getOffsetX());
		if (bombdmg > 0) {
			sprite.takeDamage(bombdmg);
		}
		Buzz.animateSprite();
		Buzz.draw(window);

		crab.move(sprite.getX(), sprite.getY(), sprite.getOffsetX(), sprite.getOffsetY());
		crab.draw(window);
		// keep movement and drawing close to each other for better user experience :)
		int dmg = m.giveDamage(sprite.getVelocityY(), sprite.getX(), sprite.getY(), sprite.getOffsetX());
		if (dmg > 0)
		{
			sprite.takeDamage(dmg);
		}
		Bat.animateSprite();
		Bat.draw(window);

		K.movement(lvl);
		//sprite.movement(lvl);
		//sprite.player_gravity(lvl);
		K.player_gravity(lvl);

		Coins.checkCollision(sprite.getX(), sprite.getY(), sprite.getOffsetX(), sprite.getOffsetY(), sprite.gethitX(), sprite.gethitY());
		diamonds.checkCollision(sprite.getX(), sprite.getY(), sprite.getOffsetX(), sprite.getOffsetY(), sprite.gethitX(), sprite.gethitY());
		special.checkCollision(sprite.getX(), sprite.getY(), sprite.getOffsetX(), sprite.getOffsetY(), sprite.gethitX(), sprite.gethitY());
		//sprite.draw_player(window);
		K.draw_player(window);

		Coins.animate();
		diamonds.animate();
		special.animate();

		int dmg1 = crab.giveDamage(sprite.getVelocityY(), sprite.getX(), sprite.getY(), sprite.getOffsetX());
		if (dmg1 > 0) sprite.takeDamage(dmg1);

		//sprite.takeDamage(m.giveDamage(sprite.getonGround(), sprite.getX(), sprite.getY(), sprite.getOffsetX()));
		sprite.update();
		/*m.update();*/

		Coins.draw(window, sprite.getOffsetX());
		diamonds.draw(window, sprite.getOffsetX());
		special.draw(window, sprite.getOffsetX());
		//crab2.draw(window);
		//crab2.move(sprite.getX(), sprite.getY(), sprite.getOffsetX(), sprite.getOffsetY());


		//window.draw()

		window.display();
	}


	return 0;
}


// functions

void player_gravity(char** lvl, int& offset_y, int& velocityY, bool& onGround, float& gravity, int& terminal_Velocity, int& hit_box_factor_x, int& hit_box_factor_y, int& player_x, int& player_y, const int cell_size, int& Pheight, int& Pwidth)
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
void display_level(RenderWindow& window, const int height, const int width, char** lvl, Sprite& wallSprite1, const int cell_size, int offset, Sprite& background, Sprite& GreenBushSprite, Sprite& BrownTowerSprite, Sprite& spikess, Sprite& BreakableWallSprite)
{
	for (int i = 0; i < height; i += 1)
	{
		// hell yeah, only loads the current window into memory
		// THE GREATEST PROGRAMMER THAT EVER LIVED
		for (int j = offset / 64; j < (1300 + offset) / 64; j += 1)
		{
			if (lvl[i][j] == 'w')
			{
				wallSprite1.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(wallSprite1);
				/*background.setPosition(0 - offset / 4, 0);*/
				/*window.draw(background);*/
			}
			if (lvl[i][j] == 'B') // green bushes
			{
				GreenBushSprite.setScale(2, 2);
				GreenBushSprite.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(GreenBushSprite);
				/*else if (lvl[i][j] == 's') {
					window.draw();
				}*/
			}
			if (lvl[i][j] == 'T')
			{
				BrownTowerSprite.setScale(4, 4);
				BrownTowerSprite.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(BrownTowerSprite);
			}
			if (lvl[i][j] == 'k')
			{

				spikess.setScale(1.5, 1.5);
				spikess.setPosition(j * cell_size - offset, i * cell_size - 10);
				window.draw(spikess);
			}
			if (lvl[i][j] == 'b')
			{
				BreakableWallSprite.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(BreakableWallSprite);
				/*background.setPosition(0 - offset / 4, 0);*/
				/*window.draw(background);*/
			}
		}
	}
}