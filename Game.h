// Game.h
#pragma once
#include "Header.h"
#include "Maps.h"
#include "Collectables.h"
#include "Enemies.h"
#include "Menu.h"
#include <SFML/Graphics.hpp>

class Game {
private:
    // Window settings
    const int screen_x = 1200;
    const int screen_y = 900;

    // Game objects
    Knuckles player;
    Maps map;
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
    sf::Texture greenBushTex;
    sf::Texture brownTowerTex;
    sf::Texture spikeTex;

    sf::Sprite wallSprite1;
    sf::Sprite breakableWallSprite;
    sf::Sprite greenBushSprite;
    sf::Sprite brownTowerSprite;
    sf::Sprite spikeSprite;

    // Window
    sf::RenderWindow window;

    // Private methods
    void initWindow();
    void initTextures();
    void initGameObjects();
    void processEvents();
    void update();
    void render();

public:
    Game();
    ~Game();
    void run();
};

// Game.cpp
#include "Game.h"

Game::Game() :
    window(sf::VideoMode(screen_x, screen_y), "Sonic the Hedgehog-OOP", sf::Style::Close),
    coins(map.getMap()),
    diamonds(map.getMap()),
    special(map.getMap()),
    motoBug(2600, 730, map.getMap()),
    crab(5500, 400, map.getMap()),
    bat(400, 300, map.getMap()),
    buzz(10000, 100, map.getMap()) {

    initWindow();
    initTextures();
    initGameObjects();
}

Game::~Game() {
    // Clean up if needed
}

void Game::initWindow() {
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(120);
}

void Game::initTextures() {
    // Load textures
    backgroundTex.loadFromFile("Data/bg5.png");
    wallTex1.loadFromFile("Data/bl.jpg");
    breakableWallTex.loadFromFile("Data/brick4.png");
    greenBushTex.loadFromFile("Data/GreenBush.png");
    brownTowerTex.loadFromFile("Data/BrownTower.png");
    spikeTex.loadFromFile("Data/spike.png");

    // Set up sprites
    backgroundSprite.setTexture(backgroundTex);
    backgroundSprite.setScale(1.8, 1.2);

    wallSprite1.setTexture(wallTex1);
    wallSprite1.setScale(0.64, 0.64);

    breakableWallSprite.setTexture(breakableWallTex);
    greenBushSprite.setTexture(greenBushTex);
    brownTowerSprite.setTexture(brownTowerTex);
    spikeSprite.setTexture(spikeTex);
}

void Game::initGameObjects() {
    lvl = map.getMap();
    coins.place();
    diamonds.place();
    special.place();
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update() {
    // Update player
    player.movement(lvl);
    player.punching(lvl);
    player.player_gravity(lvl);
    player.update();

    // Update enemies
    motoBug.move(player.getX(), player.getY(), player.getOffsetX(), player.getOffsetY());
    bat.move(player.getX(), player.getY(), player.getOffsetX(), player.getOffsetY());
    buzz.move(player.getX(), player.getY(), player.getOffsetX(), player.getOffsetY());
    crab.move(player.getX(), player.getY(), player.getOffsetX(), player.getOffsetY());

    // Check collisions with enemies
    int dmg = motoBug.giveDamage(player.getVelocityY(), player.getX(), player.getY(), player.getOffsetX());
    if (dmg > 0) player.takeDamage(dmg);

    int dmg2 = bat.giveDamage(player.getVelocityY(), player.getX(), player.getY(), player.getOffsetX());
    if (dmg2 > 0) player.takeDamage(dmg2);

    int bombdmg = buzz.giveDamage(player.getVelocityY(), player.getX(), player.getY(), player.getOffsetX());
    if (bombdmg > 0) player.takeDamage(bombdmg);

    int dmg1 = crab.giveDamage(player.getVelocityY(), player.getX(), player.getY(), player.getOffsetX());
    if (dmg1 > 0) player.takeDamage(dmg1);

    // Check collectable collisions
    coins.checkCollision(player.getX(), player.getY(), player.getOffsetX(), player.getOffsetY(), player.gethitX(), player.gethitY());
    diamonds.checkCollision(player.getX(), player.getY(), player.getOffsetX(), player.getOffsetY(), player.gethitX(), player.gethitY());
    special.checkCollision(player.getX(), player.getY(), player.getOffsetX(), player.getOffsetY(), player.gethitX(), player.gethitY());

    // Animate collectables
    coins.animate();
    diamonds.animate();
    special.animate();
}

void Game::render() {
    window.clear(sf::Color::Black);

    // Draw background
    backgroundSprite.setPosition(-player.getOffsetX() / 7, 0);
    window.draw(backgroundSprite);

    // Draw level
    for (int i = 0; i < height; i += 1) {
        for (int j = player.getOffsetX() / 64; j < (1300 + player.getOffsetX()) / 64; j += 1) {
            if (lvl[i][j] == 'w') {
                wallSprite1.setPosition(j * cell_size - player.getOffsetX(), i * cell_size);
                window.draw(wallSprite1);
            }
            if (lvl[i][j] == 'B') {
                greenBushSprite.setScale(2, 2);
                greenBushSprite.setPosition(j * cell_size - player.getOffsetX(), i * cell_size);
                window.draw(greenBushSprite);
            }
            if (lvl[i][j] == 'T') {
                brownTowerSprite.setScale(4, 4);
                brownTowerSprite.setPosition(j * cell_size - player.getOffsetX(), i * cell_size);
                window.draw(brownTowerSprite);
            }
            if (lvl[i][j] == 'k') {
                spikeSprite.setScale(1.5, 1.5);
                spikeSprite.setPosition(j * cell_size - player.getOffsetX(), i * cell_size - 10);
                window.draw(spikeSprite);
            }
            if (lvl[i][j] == 'b') {
                breakableWallSprite.setPosition(j * cell_size - player.getOffsetX(), i * cell_size);
                window.draw(breakableWallSprite);
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
    player.draw_player(window);

    // Draw collectables
    coins.draw(window, player.getOffsetX());
    diamonds.draw(window, player.getOffsetX());
    special.draw(window, player.getOffsetX());

    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}