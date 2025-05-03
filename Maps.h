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
        for (int i = 0; i < height; i++)
        {
            lvl[i] = new char[width];
            // Initialize all cells to empty space
            for (int j = 0; j < width; j++) {
                lvl[i][j] = 's'; // 's' for empty space
            }
        }

        // Create the ground floor (width 200)
        for (int i = 0; i < 200; i++)
        {
            if (i >= 165 && i < 167)
            {
                lvl[12][i] = 'w';
                lvl[13][i] = 'w';
                continue;
            }
            if (i >= 64 && i < 66)
            {
                lvl[12][i] = 'w';
                lvl[13][i] = 'w';
                continue;
            }



            lvl[12][i] = 'w';
            lvl[11][i] = 'B';
            lvl[13][i] = 'w';
        }

        // Fixed platforms (manually placed)
        // Small platforms
        lvl[8][4] = 'w';
        lvl[8][5] = 'w';
        lvl[8][6] = 'w';
        lvl[8][6] = 'w';
        lvl[7][4] = 'B';
        lvl[7][5] = 'B';
        lvl[7][6] = 'B';



        lvl[7][10] = 'w';
        lvl[7][11] = 'w';
        lvl[7][12] = 'w';

        lvl[5][20] = 'w';
        lvl[5][21] = 'w';
        lvl[5][22] = 'w';

        // Medium platforms
        for (int i = 30; i <= 34; i++)lvl[6][i] = 'w';
        for (int i = 50; i <= 54; i++) lvl[9][i] = 'w';
        for (int i = 57; i <= 60; i++) lvl[8][i] = 'w';

        // Long platforms floating vertiacally
        for (int i = 64; i <= 66; i++) lvl[5][i] = 'w';
        lvl[6][67] = 'w';lvl[6][68] = 'w';lvl[6][69] = 'w';

        for (int i = 70; i <= 74; i++) lvl[4][i] = 'w';
        for (int i = 90; i <= 94; i++) lvl[7][i] = 'w';

        // Staircase pattern
        lvl[5][120] = 'w'; lvl[5][121] = 'w';
        lvl[6][123] = 'w'; lvl[6][124] = 'w';
        lvl[7][126] = 'w'; lvl[7][127] = 'w';

        // Floating islands
        for (int i = 140; i <= 143; i++)
        {
            lvl[5][i] = 'w';
            lvl[4][i] = 'B';
        };
        //towers
        lvl[8][26] = 'T';
        //Wall below tower
        lvl[11][27] = 'w';
        lvl[11][47] = 'w';
        lvl[11][107] = 'w';
        lvl[11][196] = 'w';
        lvl[8][46] = 'T';
        lvl[8][106] = 'T';
        lvl[8][195] = 'T';

        //spikes
        lvl[11][166] = 'k';
        lvl[11][65] = 'k';

        for (int i = 148; i <= 150; i++) lvl[4][i] = 'w';
        for (int i = 155; i <= 160; i++) lvl[6][i] = 'w';
        for (int i = 160; i <= 167; i++) lvl[8][i] = 'w';
        for (int i = 170; i <= 180; i++) lvl[9][i] = 'w';
        for (int i = 180; i <= 183; i++) lvl[7][i] = 'w';
        for (int i = 186; i <= 190; i++) lvl[6][i] = 'w';
        for (int i = 189; i <= 191; i++)
        {
            lvl[4][i] = 'w';
            lvl[3][i] = 'B';
        }
        for (int i = 191; i <= 195; i++) lvl[6][i] = 'w';

        // Vertical pillars
        //Partially a staircase
        lvl[11][15] = 'w';
        lvl[11][16] = 'w'; lvl[10][16] = 'w';
        lvl[10][17] = 'w'; lvl[9][17] = 'w';lvl[11][17] = 'w';
        lvl[8][18] = 'w'; lvl[9][18] = 'w'; lvl[10][18] = 'w';lvl[11][18] = 'w';
        lvl[10][45] = 'w'; lvl[9][45] = 'w'; lvl[11][45] = 'w';
        lvl[10][79] = 'w'; lvl[9][79] = 'w'; lvl[11][79] = 'w';
        //Staircase for upper platforms
        lvl[11][78] = 'w'; lvl[10][78] = 'w';
        lvl[11][77] = 'w';
        //Vertical
        for (int i = 80; i <= 89; i++) lvl[7][i] = 'w';
        lvl[10][45] = 'w'; lvl[9][45] = 'w';
        lvl[10][100] = 'w'; lvl[9][100] = 'w';lvl[11][100] = 'w';

        // Ceiling platforms
        for (int i = 110; i <= 115; i++) lvl[8][i] = 'w';
        for (int i = 90; i <= 105; i++)
        {
            lvl[6][i] = 'w';
            lvl[5][i] = 'B';
        };
        for (int i = 130; i <= 135; i++) lvl[7][i] = 'w';
    }
    Maps(Maps& m)
    {
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