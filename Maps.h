#pragma once
#include <iostream>
//#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
//#include "Header.h"

//const int cell_size = 64;
const int height = 14;
const int width = 200;

class Maps {
	// only making one map rn so there is one main class over here, else multiple classes based on the levels
	char** lvl;
public:
    Maps() {
        lvl = new char* [height];
        for (int i = 0; i < height; i++) {
            lvl[i] = new char[width];
            // Initialize all cells to empty space
            for (int j = 0; j < width; j++) {
                lvl[i][j] = 's'; // 's' for empty space
            }
        }

        // Create the ground floor (width 200)
        for (int i = 0; i < 200; i++)
        {
           
            lvl[12][i] = 'w';
            lvl[13][i] = 'w';
        }

        // Fixed platforms (manually placed)
        // Small platforms
        lvl[8][4] = 'w';
        lvl[8][5] = 'w';
        lvl[8][6] = 'w';
        lvl[8][6] = 'w';

        lvl[7][10] = 'w';
        lvl[7][11] = 'w';
        lvl[7][12] = 'w';

        lvl[5][20] = 'w';
        lvl[5][21] = 'w';
        lvl[5][22] = 'w';

        // Medium platforms
        for (int i = 30; i <= 34; i++)lvl[6][i] = 'w';
        for (int i = 50; i <= 54; i++) lvl[9][i] = 'w';

        // Long platforms
        for (int i = 70; i <= 78; i++) lvl[4][i] = 'w';
        for (int i = 90; i <= 100; i++) lvl[7][i] = 'w';

        // Staircase pattern
        lvl[5][120] = 'w'; lvl[5][121] = 'w';
        lvl[6][123] = 'w'; lvl[6][124] = 'w';
        lvl[7][126] = 'w'; lvl[7][127] = 'w';

        // Floating islands
        for (int i = 140; i <= 143; i++) lvl[3][i] = 'w';
        for (int i = 160; i <= 163; i++) lvl[5][i] = 'w';
        for (int i = 180; i <= 183; i++) lvl[7][i] = 'w';

        // Vertical pillars
        lvl[10][15] = 'w'; lvl[9][15] = 'w'; lvl[11][15] = 'w';
        lvl[10][45] = 'w'; lvl[9][45] = 'w'; lvl[11][45] = 'w';
        lvl[10][85] = 'w'; lvl[9][85] = 'w'; lvl[8][85] = 'w'; lvl[11][85] = 'w';

        // Ceiling platforms
        for (int i = 110; i <= 115; i++) lvl[2][i] = 'w';
        for (int i = 130; i <= 135; i++) lvl[1][i] = 'w';
    }
    Maps(Maps& m) {
        lvl = m.lvl;
    }
	char** getMap() {
		return lvl;
	}
	~Maps() {
		for (int i = 0; i < height; i++) {
			delete[] lvl[i];
		}
		delete[] lvl;
	}
	/*void setLevel(int a, int b) {

	}*/
};