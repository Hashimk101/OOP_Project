#pragma once
#include "Maps.h"

class Collectables {
    // Your existing variables
    int collect_x, collect_y;
    int collect_width = 16, collect_height = 16;
    int collect_offset_x = 0, collect_offset_y = 0;
    int collect_hit_box_factor_x = 4, collect_hit_box_factor_y = 4;

    // Single coin sprite (replaces array)
    sf::Sprite coinSprite;
    sf::Texture coinTexture;
    sf::IntRect coinsRect;
    char** lvl;

    // Track if coin is active
    bool coinActive = false;

public:
    Collectables(char** lvl) : coinsRect(0, 0, collect_width, collect_height) {
        if (!coinTexture.loadFromFile("Data/ring.png")) {
            std::cout << "Failed to load coin texture!" << std::endl;
        }
        coinSprite.setTexture(coinTexture);
        coinSprite.setTextureRect(coinsRect);
        coinSprite.setScale(2.5, 2.5);
        this->lvl = lvl;
    }

    void placeCoins() {
        // Places one coin at first found 's' position
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (lvl[i][j] == 's' && i == 10) {
                    collect_x = j * cell_size;
                    collect_y = i * cell_size;
                    //coinSprite.setPosition(collect_x, collect_y);
                    //coinActive = true;
                    lvl[i][j] = 'c';
                    return; // Only place one coin
                }
            }
        }
    }
	Sprite& getCoinSprite() {
		return coinSprite;
	}

    void draw(sf::RenderWindow& window, int offset) {
        for (int i = 0; i < height; i++) {
            for (int j = offset/64; j < (offset + 1300)/64; j++) {
				if (lvl[i][j] == 'c') {
					coinSprite.setPosition(j * cell_size - offset, i * cell_size);
					window.draw(coinSprite);
				}
            }
        }
    }
};