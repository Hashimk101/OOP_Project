// Game.h
#pragma once
#include "Header.h"
#include "Maps.h"
#include "Collectables.h"
#include "Enemies.h"
#include "Menu.h"
#include "ScoreBoard.h"

#include <SFML/Graphics.hpp>


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
    // LVL 2
    // R is rocks
    // C is crystals
    //LVL 3
    //K is black crystals
    //P is pink crystals
    //W is white crystals
    //L IS BLUE CRYSTALS
    //V IS VOILET





class Game
{
private:
    // Window settings
    const int screen_x = 1200;
    const int screen_y = 900;
    int currentLevel=3;
    // Game objects
    Maps map;
    MySprite* players[3];
    int currentPlayer;
    MySprite* player;
    //Knuckles player;
    char** lvl;

    // Collectables
    RingCoin coins;
    ExtraLife diamonds;
    SpecialBoost special;

    // Enemies
    MotoBug motoBug;
    CrabMeat crab;
    BatBrain bat;
    BuzzBomber buzz;

    // Sprites and textures
    sf::Texture backgroundTex;
    sf::Sprite backgroundSprite;
    sf::Texture wallTex1;
    sf::Texture breakableWallTex;
    //level:1
    sf::Texture greenBushTex;
    sf::Texture brownTowerTex;
    sf::Texture spikeTex;
    
    sf::Sprite wallSprite1;
    sf::Sprite breakableWallSprite;

    //Level 1
    sf::Sprite brownTowerSprite;
    sf::Sprite greenBushSprite;
    //Level:2
    Texture RockTex1, CrystalTex, JellyTex;
	Sprite Rock1, Crystal1, Jelly;
	//Level:3
   sf:: Texture BlackCrystalTex, PinkCrystalTex, WhiteCrystalTex, blueCrystalTex, voiletCrystalTex;
   sf::Sprite BlackCrystal, PinkCrystal, WhiteCrystal, BlueCrystal, voiletCrystal;

    //Common obstackle
    sf::Sprite spikeSprite;
    //Score object
    Scores score;

    // Window
    sf::RenderWindow window;
    sf::Clock playerChange;

    // Private methods
    void initWindow();
    void initTextures();
    void initGameObjects();
    void processEvents();
    void update();
    void render();
    void switchPlayer();
    //
    Menu menu;

public:
    Game();
    ~Game();
    void run();
};



Game::Game() :
    window(sf::VideoMode(screen_x, screen_y), "Sonic the Hedgehog-OOP", sf::Style::Close),
    map(3),
    coins(map),
    diamonds(map),
    special(map),
    motoBug(2600, 730, map.getMap()),
    crab(5500, 400, map.getMap()),
    bat(400, 300, map.getMap()),
    buzz(10000, 100, map.getMap()), score(window), menu(window) {

    //player = new Sonic();
    players[0] = new Sonic();
    players[1] = new Knuckles();
    players[2] = new Tails();
    currentPlayer = 0;
    player = players[currentPlayer];
    Vector2f curPos = player->getPos();
    players[1]->setPos(curPos.x - 20, curPos.y);
    players[2]->setPos(curPos.x - 30, curPos.y);
    initWindow();
    initTextures();
    initGameObjects();
}

Game::~Game() {
    // Clean up if needed
}

void Game::initWindow()
{
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(120);
}

void Game::initTextures()
{
    for (int i = 0; i < 3; i++)
    {
        players[i]->setWidth(map.GetLevelWidth());
    }
   
    // Load textures
    if (currentLevel == 1) 
    {
        if (!backgroundTex.loadFromFile("Data/bg5.png")) 
        {
            cout << "Could not load Texture. \n";
        }
        wallTex1.loadFromFile("Data/bl.jpg");
        breakableWallTex.loadFromFile("Data/brick4.png");
    }
    if (currentLevel == 2) 
    {
        backgroundTex.loadFromFile("Data/backg12.png");
        wallTex1.loadFromFile("Data/block.png");
        breakableWallTex.loadFromFile("Data/brick8.png");
    }
    if (currentLevel == 3)
    {
        backgroundTex.loadFromFile("Data/BlueSp.jpg");
        wallTex1.loadFromFile("Data/Sp_brick.png");
        breakableWallTex.loadFromFile("Data/brick9.png");
    }

	//Level 1
    greenBushTex.loadFromFile("Data/GreenBush.png");
    brownTowerTex.loadFromFile("Data/BrownTower.png");
    spikeTex.loadFromFile("Data/spike.png");
	//Level 2
    RockTex1.loadFromFile("Data/snowy_rock3.png");
    CrystalTex.loadFromFile("Data/crystal.png");
    JellyTex.loadFromFile("Data/jelly (2).png");
    //Load sprites
	Rock1.setTexture(RockTex1);
	Crystal1.setTexture(CrystalTex);
	Jelly.setTexture(JellyTex);

	//Level 3
    BlackCrystalTex.loadFromFile("Data/crystal_black.png");
    PinkCrystalTex.loadFromFile("Data/crystal_red-pink.png");
    WhiteCrystalTex.loadFromFile("Data/crystal_white-gold.png");
    blueCrystalTex.loadFromFile("Data/crystal_blue.png");
    voiletCrystalTex.loadFromFile("Data/crystal_violet.png");

	// Load sprites
    BlackCrystal.setTexture(BlackCrystalTex), PinkCrystal.setTexture(PinkCrystalTex), WhiteCrystal.setTexture(WhiteCrystalTex), BlueCrystal.setTexture(blueCrystalTex), voiletCrystal.setTexture(voiletCrystalTex);
    // Set up sprites
    backgroundSprite.setTexture(backgroundTex);
    if (currentLevel == 1) 
    {
        backgroundSprite.setScale(1.8, 1.2);
    }
    else if (currentLevel == 2) 
    {
        backgroundSprite.setScale(1, 1);
    }
    else if (currentLevel == 3) 
    {
        backgroundSprite.setScale(0.6, 0.5);
    }
  
    wallSprite1.setTexture(wallTex1);
  
    if (currentLevel == 1) 
    {
        wallSprite1.setScale(0.63, 0.63);
    }
    else
    {
        wallSprite1.setScale(1, 1);
    }


    breakableWallSprite.setTexture(breakableWallTex);
    greenBushSprite.setTexture(greenBushTex);
    brownTowerSprite.setTexture(brownTowerTex);
    spikeSprite.setTexture(spikeTex);
}

void Game::initGameObjects() {
    int thislvlWidth = map.GetLevelWidth();
    std::cout << thislvlWidth << std::endl;
    for (int i = 0; i < 3; i++) {
        players[i]->setWidth(thislvlWidth); // Set width for Sonic, Knuckles, Tails
    }
    lvl = map.getMap();
    coins.place();
    diamonds.place();
    special.place();
    playerChange.restart();
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
		menu.handleEvent(event);
    }
}

void Game::update() 
{
    // Update player
    bool ismoving = player->movement(lvl, true, false);
    
    bool isflying = player->getIsFlying();
    std::cout << currentPlayer << " " << isflying << std::endl;
    player->punching(lvl, true);
    player->player_gravity(lvl);
    player->update();

    for (int i = 0; i < 3; i++)
    {
        if (i != currentPlayer) 
        {
            players[i]->setOffsetX(player->getOffsetX());
            if (player->getDirection())
            {
                players[i]->setX(player->getX() + 40);
            }
            else
            {
                players[i]->setX(player->getX() - 40);
            }
            if (isflying) 
            {
                players[i]->hang(player->getX(), player->getY(), player->getOffsetX());
            }
            else
            {
                players[i]->movement(lvl, false, false);
            }
            players[i]->punching(lvl, false);
            players[i]->player_gravity(lvl);
            players[i]->update();

            ///////
            ///////
            ///////
            ///////

            if (currentPlayer == 0) {
                players[i]->setSpeed(12);
            }
            else if (currentPlayer == 1) {
                players[i]->setSpeed(10);
            }
            else if (currentPlayer == 2) {
                players[i]->setSpeed(8);
            }

        }
    }


    // Update enemies
    motoBug.move(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), score);
    bat.move(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), score);
    buzz.move(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), score);
    crab.move(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), score);

    // Check collisions with enemies
    int dmg = motoBug.giveDamage(player->getVelocityY(), player->getX(), player->getY(), player->getOffsetX(), score);
    if (dmg > 0) player->takeDamage(dmg);

    int dmg2 = bat.giveDamage(player->getVelocityY(), player->getX(), player->getY(), player->getOffsetX(), score);
    if (dmg2 > 0) player->takeDamage(dmg2);

    int bombdmg = buzz.giveDamage(player->getVelocityY(), player->getX(), player->getY(), player->getOffsetX(), score);
    if (bombdmg > 0) player->takeDamage(bombdmg);

    int dmg1 = crab.giveDamage(player->getVelocityY(), player->getX(), player->getY(), player->getOffsetX(), score);
    if (dmg1 > 0) player->takeDamage(dmg1);

    // Check collectable collisions
    coins.checkCollision(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), player->gethitX(), player->gethitY(), score, *player);
    diamonds.checkCollision(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), player->gethitX(), player->gethitY(), score, *player);
    special.checkCollision(player->getX(), player->getY(), player->getOffsetX(), player->getOffsetY(), player->gethitX(), player->gethitY(), score, *player);

    // Animate collectables
    coins.animate();
    diamonds.animate();
    special.animate();
}

void Game::render() {
    window.clear(sf::Color::Black);
    
    // Draw background
    backgroundSprite.setPosition(-player->getOffsetX() / 7, 0);
   
    window.draw(backgroundSprite);
    

    // Draw level
    for (int i = 0; i < height; i += 1) {
        for (int j = player->getOffsetX() / 64; j < (1300 + player->getOffsetX()) / 64; j += 1) {
            if (lvl[i][j] == 'w')
            {
                //wallSprite2.setScale(2, 2);
                wallSprite1.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(wallSprite1);
            }


            if (lvl[i][j] == 'B')
            {
                greenBushSprite.setScale(2, 2);
                greenBushSprite.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(greenBushSprite);
            }
            if (lvl[i][j] == 'T')
            {
                brownTowerSprite.setScale(4, 4);
                brownTowerSprite.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(brownTowerSprite);
            }
            if (lvl[i][j] == 'k')
            {
                spikeSprite.setScale(1.5, 1.5);
                spikeSprite.setPosition(j * cell_size - player->getOffsetX(), i * cell_size - 10);
                window.draw(spikeSprite);
            }
            if (lvl[i][j] == 'b')
            {
                breakableWallSprite.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(breakableWallSprite);
            }

            if (lvl[i][j] == 'R')
            {
                Rock1.setScale(1, 1);

                Rock1.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(Rock1);
            }
            if (lvl[i][j] == 'J')
            {
                Jelly.setScale(1, 1);
                Jelly.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(Jelly);
            }



            if (lvl[i][j] == 'C')
            {
                Crystal1.setScale(1, 1);
                Crystal1.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(Crystal1);
            }
            if (lvl[i][j] == 'K')
            {
                BlackCrystal.setScale(1, 1);
                BlackCrystal.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(BlackCrystal);
            }
            if (lvl[i][j] == 'P')
            {
                PinkCrystal.setScale(1, 1);
                PinkCrystal.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(PinkCrystal);
            }
            if (lvl[i][j] == 'W')
            {
                WhiteCrystal.setScale(1, 1);
                WhiteCrystal.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(WhiteCrystal);
            }
            if (lvl[i][j] == 'U')
            {
                BlueCrystal.setScale(1, 1);
                BlueCrystal.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(BlueCrystal);
            }
            if (lvl[i][j] == 'V')
            {
                voiletCrystal.setScale(1, 1);
                voiletCrystal.setPosition(j * cell_size - player->getOffsetX(), i * cell_size);
                window.draw(voiletCrystal);
            }
        }
    }

    // Draw enemies
    motoBug.draw(window);
    motoBug.animateSprite();

    bat.draw(window);
    bat.animateSprite();

    buzz.draw(window);
    buzz.animateSprite();

    crab.draw(window);


    // Draw player
    //player->draw_player(window);
    for (int i = 0; i < 3; i++) {
        if (i != currentPlayer) {
            players[i]->draw_player(window);
        }
    }
    player->draw_player(window);

    // Draw collectables
    coins.draw(window, player->getOffsetX());
    diamonds.draw(window, player->getOffsetX());
    special.draw(window, player->getOffsetX());
    score.DisplayScoreWin(*player);
  
    window.display();
}

void Game::run()
{
    while (window.isOpen()) {
        processEvents();
        update();
        render();
        switchPlayer();
    }
}

void Game::switchPlayer() {
    if (Keyboard::isKeyPressed(Keyboard::C)) {
        if (playerChange.getElapsedTime().asSeconds() >= 1.5f) {
            sf::Vector2f pos = player->getPos();
            float velocityY = player->getVelocityY();
            float offsetX = player->getOffsetX();

            // Switch to next character
            currentPlayer = (currentPlayer + 1) % 3;
            player = players[currentPlayer];

            // Apply position and state to new character
            player->setPos(pos.x, pos.y);
            player->setOffsetX(offsetX);

            // Optional: Apply velocity to maintain momentum
            player->setVelocityY(velocityY);
            playerChange.restart();
        }
    }
}