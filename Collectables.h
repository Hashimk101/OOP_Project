#pragma once
//#include <iostream>
////#include <fstream>
//#include <cmath>
//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/Window.hpp>
#include "Maps.h"
#include "Header.h"
using namespace sf;

class Collectables {
    static const int MAX_COINS = 100;

    struct Coin {
        int collect_x, collect_y;
        sf::Sprite sprite;
        bool active;
        sf::Vector2f position;
    };

    // Your original variables
   
    int collect_width = 16, collect_height = 16;
    int collect_hit_box_factor_x = 4, collect_hit_box_factor_y = 4;

    // Coin system variables
    Coin coins[MAX_COINS];
    int activeCoinCount = 0;
    sf::IntRect coinsRect;
    sf::Texture coinTexture;
    int currentFrame = 0;
    //float animationSpeed = 0.1f;
    sf::Clock animationClock;
    Maps map;

public:
    Collectables(Maps &m) : coinsRect(0, 0, collect_width, collect_height), map(m) {
		if (!coinTexture.loadFromFile("Data/ring.png")) {
			std::cout << "Failed to load coin texture!" << std::endl;
		}
        for (int i = 0; i < MAX_COINS; i++) {
            coins[i].active = true;
            coins[i].sprite.setTexture(coinTexture);
            coins[i].sprite.setTextureRect(coinsRect);
        }
    }
    void placeCoins(char** lvl) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (lvl[i][j] == 's' && i == 10 && activeCoinCount < MAX_COINS) {
                    lvl[i][j] = 'c';
                    activeCoinCount++;
                }
            }
        }
    }
    void removeCoin(int &i, int &j) 
    {
		char** lvl = map.getMap();
		if (lvl[i][j] == 'c')
        {
			lvl[i][j] = 's'; // Remove the coin from the level
			coins[activeCoinCount].active = false; // Mark the coin as inactive
			activeCoinCount--;
		}
    }
    void reusecoins() {
		for (int i = 0; i < MAX_COINS; i++) {
			if (!coins[i].active) {
				coins[i].active = true;
				coins[i].sprite.setTextureRect(coinsRect);
			}
		}
    }
};