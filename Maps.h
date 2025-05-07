#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
//#include "Header.h"

//const int cell_size = 64;
const int height = 14;
const int width = 250;

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
            for (int j = 0; j < width; j++)
            {
                lvl[i][j] = 's'; // 's' for empty space
            }
        }
       
        // Create the ground floor 
        for (int i = 0; i < 250; i++)
        {
            lvl[13][i] = 'w';
            lvl[12][i] = 'w';
           
        }
        lvl[11][30] = 'R';
        lvl[11][58] = 'R';
        lvl[5][98] = 'R';
        lvl[11][127] = 'R';


        // Fixed platforms (manually placed)
        // Small platforms
        lvl[8][4] = 'w';
        lvl[8][5] = 'w';
        lvl[8][6] = 'w';
        lvl[8][6] = 'w';

        lvl[5][68] = 'C';


        lvl[9][107] = 'w';
        lvl[8][107] = 'w';
        lvl[10][107] = 'w';
        lvl[11][107] = 'w';
        lvl[8][108] = 'w';
        lvl[8][109] = 'w';

        //BIGG AA
        for (int i = 107; i < 150; i++)
        {
         
            if (i == 110 || i == 120 || i == 130 || i == 140 || i == 149) 
            {
                lvl[8][i] = 'J';
                continue;
            }
            lvl[8][i] = 'w';
        }



        lvl[11][10] = 'R';

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
        lvl[7][126] = 'w'; /*lvl[7][127] = 'w';*/

   
        for (int i = 154; i <= 156; i++) {
            lvl[4][i] = 'w';
            lvl[7][i] = lvl[8][i] = lvl[9][i] = lvl[10][i] = lvl[11][i] = 'b';
        }

        for (int i = 0; i < width; i += 8) 
        {
            lvl[11][i] = 'j';  // jelly
        }

        for (int i = 0; i < width; i += 8) {
            lvl[11][i] = 'r';
        }


        lvl[10][24] = 'J';
        // Floating islands
        for (int i = 140; i <= 143; i++)
        {
            lvl[5][i] = 'w';
            
        };
        //spikes
        lvl[11][166] = 'k';
   
        lvl[11][50] = 'k';
        lvl[11][100] = 'k';
        lvl[11][150] = 'k';
        lvl[11][230] = 'k';


        for (int i = 148; i <= 150; i++) lvl[4][i] = 'w';
        for (int i = 155; i <= 160; i++) lvl[6][i] = 'w';
        for (int i = 170; i <= 180; i++) lvl[9][i] = 'w';
        for (int i = 180; i <= 183; i++) lvl[7][i] = 'w';
        for (int i = 186; i <= 190; i++) lvl[6][i] = 'w';
        for (int i = 189; i <= 191; i++)
        {
            
            lvl[4][i] = 'w';
            
           
        }
        //breakable walls
        for (int i = 0; i < 12; i++)
        {
            lvl[i][169] = 'b';
            lvl[i][169] = 'b';
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
      
        for (int i = 90; i <= 100; i++)
        {
            lvl[6][i] = 'w';
            
        }
        for (int i = 102; i < 105; i++)
        {
            lvl[7][i] = 'w';
        }

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