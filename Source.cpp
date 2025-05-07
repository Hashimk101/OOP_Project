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
#include "Menu.h"
#include "ScoreBoard.h"

using namespace sf;
using namespace std;

int screen_x = 1200;
int screen_y = 900;
// prototypes 
void player_gravity(char** lvl, int& offset_y, int& velocityY, bool& onGround, float& gravity, int& terminal_Velocity, int& hit_box_factor_x, int& hit_box_factor_y, int& player_x, int& player_y, const int cell_size, int& Pheight, int& Pwidth);

void draw_player(RenderWindow& window, Sprite& LstillSprite, int player_x, int player_y);

void display_level(RenderWindow& window, const int height, const int width, char** lvl, Sprite& wallSprite1, Sprite& wallSprite2, const int cell_size, int offset, Sprite& background, Sprite& Bush, Sprite& BrownTower, Sprite& spikes, Sprite& BreakableWallSprite, Sprite& rock, Sprite& J, Sprite& Crystals, Sprite& BlackCrystal, Sprite& PinkCrystal, Sprite& WhiteCrystal, Sprite& BlueCrystal, Sprite& voiletCrystal);

int main()
{

	Knuckles K;
	Tails tails;
	RenderWindow window(VideoMode(screen_x, screen_y), "Sonic the Hedgehog-OOP", Style::Close);
	//Menu menu(window);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(120);
	sf::Texture BackgroundTex1, BackgroundTex2, BackgroundTex3;
	BackgroundTex3.loadFromFile("Data/BlueSp.jpg");
	BackgroundTex2.loadFromFile("Data/Bg.jpg");
	sf::Sprite BackgroundSprite(BackgroundTex3);
	BackgroundSprite.setScale(0.4, 0.5);
	//For level 1, scale BackgroundSprite.setScale(1.8, 1.2);

	//Level 2: BackgroundSprite.setScale(1, 0.45 );


	Texture RockTex1, CrystalTex;
	RockTex1.loadFromFile("Data/snowy_rock3.png");
	CrystalTex.loadFromFile("Data/crystal.png");
	Sprite rocks(RockTex1), Crystals(CrystalTex);
	//BackgroundSprite.setPosition(100, 100);
	/////////////////////////////////////////////////////////////////
	// a cell is 64 by 64 pixels
	//'j' is for jelly
	// 's' is regular space
	// 'q' is wall1 or floor1
	// 'w' is wall2 or floor2
	// 'e' is wall3 or floor3
	// 'b' is breakable wall
	// 'z' is spring

	// Uppercase for not interactable background accessories
	// LVL:1
	//B is for greenBushes
	//T is for brown tower
	// R is rocks
	// C is crystals
	//LVL 2
	//K is black crystals
	//P is pink crystals
	//W is white crystals
	//L IS BLUE CRYSTALS
	//V IS VOILET

	Texture BlackCrystalTex, PinkCrystalTex, WhiteCrystalTex, blueCrystalTex, voiletCrystalTex;
	BlackCrystalTex.loadFromFile("Data/crystal_black.png");
	PinkCrystalTex.loadFromFile("Data/crystal_red-pink.png");
	WhiteCrystalTex.loadFromFile("Data/crystal_white-gold.png");
	blueCrystalTex.loadFromFile("Data/crystal_blue.png");
	voiletCrystalTex.loadFromFile("Data/crystal_violet.png");
	Sprite BlackCrystal(BlackCrystalTex), PinkCrystal(PinkCrystalTex), WhiteCrystal(WhiteCrystalTex), BlueCrystal(blueCrystalTex), voiletCrystal(voiletCrystalTex);

	Sonic sprite;
	Maps map;
	char** lvl = map.getMap();


	Texture wallTex1, wallTex2, wallTex3, Jelly;
	wallTex1.loadFromFile("Data/bl.jpg");
	wallTex2.loadFromFile("Data/brick6.png");
	wallTex3.loadFromFile("Data/Sp_brick.png");
	Jelly.loadFromFile("Data/jelly (2).png");
	Sprite jellySp(Jelly);
	//Level 1
	Texture BreakableWallTex1, BreakableWallTex2;
	BreakableWallTex1.loadFromFile("Data/brick4.png");

	Sprite BreakableWallSprite(BreakableWallTex1);
	BreakableWallTex2.loadFromFile("Data/brick8.png");
	Sprite BreakableWallSprite2(BreakableWallTex2);
	Sprite wallSprite1(wallTex1), wallSprite2(wallTex2), wallSprite3(wallTex3);



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
	//Menu menu(window);
	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) 
		{
			if (event.type == Event::Closed)
				
				window.close();
			
		}
		window.clear(Color::Black);

		BackgroundSprite.setPosition(-tails.getOffsetX() / 7, 0);
		window.draw(BackgroundSprite);

		display_level(window, height, width, lvl, wallSprite1, wallSprite3, cell_size, tails.getOffsetX(), BackgroundSprite, GreenBushSprite, BrownTowerSprite, spikeSprite, BreakableWallSprite2, rocks, jellySp, Crystals, BlackCrystal, PinkCrystal, WhiteCrystal, BlueCrystal, voiletCrystal);

		m.draw(window);
		m.animateSprite();
		Bat.animateSprite();
		Bat.draw(window);
		m.move(tails.getX(), tails.getY(), tails.getOffsetX(), tails.getOffsetY());
		Bat.move(tails.getX(), tails.getY(), tails.getOffsetX(), tails.getOffsetY());

		int dmg2 = Bat.giveDamage(tails.getVelocityY(), tails.getX(), tails.getY(), tails.getOffsetX());
		if (dmg2 > 0) tails.takeDamage(dmg2);

		Buzz.move(tails.getX(), tails.getY(), tails.getOffsetX(), tails.getOffsetY());
		int bombdmg = Buzz.giveDamage(tails.getVelocityY(), tails.getX(), tails.getY(), tails.getOffsetX());
		if (bombdmg > 0) tails.takeDamage(bombdmg);
		Buzz.animateSprite();
		Buzz.draw(window);

		crab.move(tails.getX(), tails.getY(), tails.getOffsetX(), tails.getOffsetY());
		crab.draw(window);

		int dmg = m.giveDamage(tails.getVelocityY(), tails.getX(), tails.getY(), tails.getOffsetX());
		if (dmg > 0) tails.takeDamage(dmg);

		int dmg1 = crab.giveDamage(tails.getVelocityY(), tails.getX(), tails.getY(), tails.getOffsetX());
		if (dmg1 > 0) tails.takeDamage(dmg1);

		tails.movement(lvl);
		tails.draw_player(window);
		tails.player_gravity(lvl);
		tails.update();

		Coins.checkCollision(tails.getX(), tails.getY(), tails.getOffsetX(), tails.getOffsetY(), tails.gethitX(), tails.gethitY());
		diamonds.checkCollision(tails.getX(), tails.getY(), tails.getOffsetX(), tails.getOffsetY(), tails.gethitX(), tails.gethitY());
		special.checkCollision(tails.getX(), tails.getY(), tails.getOffsetX(), tails.getOffsetY(), tails.gethitX(), tails.gethitY());

		Coins.animate();
		diamonds.animate();
		special.animate();

		Coins.draw(window, tails.getOffsetX());
		diamonds.draw(window, tails.getOffsetX());
		special.draw(window, tails.getOffsetX());


		// int dmg2 = Bat.giveDamage(K.getVelocityY(), K.getX(), K.getY(), K.getOffsetX());
	// if (dmg2 > 0) K.takeDamage(dmg2);
	// int bombdmg = Buzz.giveDamage(K.getVelocityY(), K.getX(), K.getY(), K.getOffsetX());
	// if (bombdmg > 0) { K.takeDamage(bombdmg); }
	// int dmg = m.giveDamage(K.getVelocityY(), K.getX(), K.getY(), K.getOffsetX());
	// if (dmg > 0) { K.takeDamage(dmg); }
	// Coins.checkCollision(K.getX(), K.getY(), K.getOffsetX(), K.getOffsetY(), K.gethitX(), K.gethitY());
	// diamonds.checkCollision(K.getX(), K.getY(), K.getOffsetX(), K.getOffsetY(), K.gethitX(), K.gethitY());
	// special.checkCollision(K.getX(), K.getY(), K.getOffsetX(), K.getOffsetY(), K.gethitX(), K.gethitY());
	// int dmg1 = crab.giveDamage(K.getVelocityY(), K.getX(), K.getY(), K.getOffsetX());
		// if (dmg1 > 0) K.takeDamage(dmg1);



		// K.movement(lvl);
	// K.draw_player(window);
	// K.punching(lvl);
	// K.player_gravity(lvl);
	// K.update();

















		
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
void display_level(RenderWindow& window, const int height, const int width, char** lvl, Sprite& wallSprite1, Sprite& wallSprite2, const int cell_size, int offset, Sprite& background, Sprite& GreenBushSprite, Sprite& BrownTowerSprite, Sprite& spikess, Sprite& BreakableWallSprite, Sprite& rock, Sprite& JELLY, Sprite& Crystals, Sprite& BlackCrystal, Sprite& PinkCrystal, Sprite& WhiteCrystal, Sprite& BlueCrystal, Sprite& voiletCrystal)
{
	for (int i = 0; i < height; i += 1)
	{
		// hell yeah, only loads the current window into memory
		// THE GREATEST PROGRAMMER THAT EVER LIVED
		for (int j = offset / 64; j < (1300 + offset) / 64; j += 1)
		{
			if (j < 0 || j >= width) continue;  // skip out-of-bounds columns

			if (lvl[i][j] == 'w')
			{
				//wallSprite2.setScale(2, 2);
				wallSprite2.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(wallSprite2);
			}
			if (lvl[i][j] == 'q') {
				wallSprite2.setScale(1, 1);
				wallSprite2.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(wallSprite2);
			}

			if (lvl[i][j] == 'B')
			{
				GreenBushSprite.setScale(2, 2);
				GreenBushSprite.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(GreenBushSprite);
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
			}

			if (lvl[i][j] == 'R')
			{
				rock.setScale(1, 1);

				rock.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(rock);
			}
			if (lvl[i][j] == 'J')
			{
				JELLY.setScale(1, 1);
				JELLY.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(JELLY);
			}



			if (lvl[i][j] == 'C')
			{
				Crystals.setScale(1, 1);
				Crystals.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(Crystals);
			}
			if (lvl[i][j] == 'K')
			{
				BlackCrystal.setScale(1, 1);
				BlackCrystal.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(BlackCrystal);
			}
			if (lvl[i][j] == 'P')
			{
				PinkCrystal.setScale(1, 1);
				PinkCrystal.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(PinkCrystal);
			}
			if (lvl[i][j] == 'W')
			{
				WhiteCrystal.setScale(1, 1);
				WhiteCrystal.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(WhiteCrystal);
			}
			if (lvl[i][j] == 'U')
			{
				BlueCrystal.setScale(1, 1);
				BlueCrystal.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(BlueCrystal);
			}
			if (lvl[i][j] == 'V')
			{
				voiletCrystal.setScale(1, 1);
				voiletCrystal.setPosition(j * cell_size - offset, i * cell_size);
				window.draw(voiletCrystal);
			}


		}

	}
}